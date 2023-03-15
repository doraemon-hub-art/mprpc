#include "rpcprovider.h"
#include <mprpcapplication.h>
#include <functional>
#include <google/protobuf/descriptor.h>
#include "rpcheader.pb.h"
#include "logger.h"

/*

service name => service描述 => service指针 =>记录服务对象
*/

void RpcProvider::NotifyService(google::protobuf::Service * service){

    ServiceInfo service_info;

    // 获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor *pserviceDesc = service->GetDescriptor();
    // 获取服务的名字
    std::string service_name = pserviceDesc->name();
    // 获取服务对象service的方法数量
    int methodCnt = pserviceDesc->method_count();

    //std::cout << "service_name:"<<service_name <<std::endl;
    LOG_INFO("service_name: %s",service_name.c_str());
    

    for(int i = 0; i < methodCnt;i++){
        // 获取了服务对象的指定下标的服务方法的描述(抽象描述)
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        // 服务名 & 相关描述
        service_info.m_methodMap.insert({method_name,pmethodDesc});

        //std::cout<<"method_name:"<<method_name<<std::endl;
        LOG_INFO("method_name: %s",method_name.c_str());
    }

    service_info.m_service = service;
    m_serviceMap.insert({service_name,service_info});
}

void RpcProvider::Run(){
    // 组合了TcpServer
    //std::unique_ptr<muduo::net::TcpServer>m_tcpserverPtr;

    std::string rsip = "rpcserverip";
    std::string scpt = "rpcserverport";
    //std::cout<<MprpcApplication::GetConfig().Load(scpt)<<std::endl;
    std::string ip = MprpcApplication::GetConfig().Load(rsip);
    uint16_t port = atoi(MprpcApplication::GetConfig().Load(scpt).c_str());

    muduo::net::InetAddress address(ip,port);

    // 创建TcpServer对象
    muduo::net::TcpServer server(&m_eventLoop,address,"RpcProvider");
    
    // 绑定连接回调和消息读写回调方法 
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection,this,std::placeholders::_1));

    server.setMessageCallback(std::bind(&RpcProvider::OnMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));

    // 设置muduo库的线程数量
    server.setThreadNum(4);

    // 显示信息
    

    // 启动网络服务
    server.start();

    m_eventLoop.loop();// epoll_wait
}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn){
    // rpc类似于http短链接，完成调用数据返回后，该连接就会断开
    if(!conn->connected()) {
        // rpc连接断开
        conn->shutdown();
    }
}

/*
在框架内部，RpcProvider和RpcConsumerx应该协商好通信用的protobuf数据类型
定义proto的Message类型，进行数据的头序列化和反序列化

为了防止粘包，还需要一个额外的参数用来记录方法参数的个数
*/
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn,muduo::net::Buffer* buffer,muduo::Timestamp ){
    // 网络上接收的远程rpc调用请求的字符流方法名，参数
    std::string recv_buf = buffer->retrieveAllAsString();

    // headersize + header_str + args_str
    
    // 从字符流中读取前四个字节的内容
    uint32_t header_size  = 0;
    // 从recv_buf中0字节开始拷贝四个字节到header_size中
    recv_buf.copy((char*)&header_size,4,0);

    // 根据header_size读取数据头的原始字符流,反序列化数据得到rpc请求的详细信息
    std::string rpc_header_str = recv_buf.substr(4,header_size);

    mprpc::RpcHeader rpcHeader;

    std::string service_name;
    std::string method_name;
    uint32_t args_size;
    if(rpcHeader.ParseFromString(rpc_header_str)){
        // 数据头反序列成功

        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();

    }else{
        // 数据头反序列失败
        // 记录日志
        //std::cout<< "rpc_header_str:" << rpc_header_str<<"parse error!"<<std::endl;
        LOG_ERROR("rpc_header_str: %s ,parse error!",rpc_header_str.c_str());
        return ;
    }

    // 方法参数
    std::string args_str = recv_buf.substr(4+header_size,args_size);
    // 打印调试信息
    // std::cout <<"============"<<std::endl;
    // std::cout <<"header_size: "<<header_size <<std::endl;
    // std::cout <<"service_name: "<< service_name <<std::endl;
    // std::cout <<"method_name: "<< method_name <<std::endl;
    // std::cout <<"args_str: "<< args_str << std::endl;
    // std::cout <<"============"<<std::endl;

    LOG_INFO("========rpc request info========");
    LOG_INFO("header_size: %d",header_size);
    LOG_INFO("service_name: %s",service_name.c_str());
    LOG_INFO("method_name: %s",method_name.c_str());
    LOG_INFO("args_str: %s",args_str.c_str());
    LOG_INFO("==============end===============");


    // 获取service对象和method对象
    auto it = m_serviceMap.find(service_name);
    if(it == m_serviceMap.end()){// 没有这个服务
        //std::cout << service_name <<"is not exist"<<std::endl;
        LOG_ERROR("service_name: %s ,is not exist!",service_name.c_str());
        return ;
    }

    
    
    auto mit = it->second.m_methodMap.find(method_name);
    if(mit == it->second.m_methodMap.end()){
        //std::cout<<service_name<<":"<<method_name<<" is not exist!"<<std::endl;
        LOG_ERROR("service_name: %s 's %s ,is not exist!",service_name.c_str(),method_name.c_str());
        return;
    }

    google::protobuf::Service *service = it->second.m_service;// 取对象service
    const google::protobuf::MethodDescriptor *method = mit->second;// 取方法method
    
    // 生成rpc方法的请求request和响应response参数
    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    if(!request->ParseFromString(args_str)){
        //std::cout<<"request parse error,content: "<<args_str<<std::endl;
        LOG_ERROR("request parse error,content: %s",args_str.c_str());
        return ;
    }
    google::protobuf::Message *response = service->GetResponsePrototype(method).New();

    //给下面的method的方法的调用，绑定一个closeure回调
    // 强制指定模板类型实参，防止无法识别
    google::protobuf::Closure* done = 
    google::protobuf::NewCallback<RpcProvider,
                                const muduo::net::TcpConnectionPtr&,
                                google::protobuf::Message*>
                                (this,
                                &RpcProvider::SendRpcResponse,
                                conn,response);

    // 根据参数信息调用由框架调用具体函数
    // 在框架上根据远端rpc请求，调用当前rpc节点上发布的方法
    service->CallMethod(method,nullptr,request,response,done);

}

 void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr& conn,google::protobuf::Message* response){
    std::string response_str;
    if(response->SerializeToString(&response_str)){// response进行序列号，结果放到response_str中
        // 序列化成功后，通过网络把rpc方法执行的结果发送回rpc方法的调用方
        conn->send(response_str);
        // 数据返回，断开链接，相当于http的短链接
        conn->shutdown();
        return ;
    }else{
        //std::cout<<"serialize response_str error!"<<std::endl;
        LOG_ERROR("serialize response_str error!");
    }
    conn->shutdown();// prcprivoder主动断开连接
 }