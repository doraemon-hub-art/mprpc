#include "rpcprovider.h"

#include <mprpcapplication.h>
#include <functional>
#include <google/protobuf/descriptor.h>
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

    std::cout << "service_name:"<<service_name <<std::endl;

    for(int i = 0; i < methodCnt;i++){
        // 获取了服务对象的指定下标的服务方法的描述(抽象描述)
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        // 服务名 & 相关描述
        service_info.m_methodMap.insert({method_name,pmethodDesc});

        std::cout<<"method_name:"<<method_name<<std::endl;
    }

    service_info.m_service = service;
    m_serviceMap.insert({service_name,service_info});
}

void RpcProvider::Run(){
    // 组合了TcpServer
    //std::unique_ptr<muduo::net::TcpServer>m_tcpserverPtr;

    std::string rsip = "rpcserverip";
    std::string scpt = "rpcserverport";
    std::cout<<MprpcApplication::GetConfig().Load(scpt)<<std::endl;
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
    std::cout<<"RpcProvider start service at ip:"<<ip<<" port:"<<port<<std::endl;

    // 启动网络服务
    server.start();

    m_eventLoop.loop();// epoll_wait
}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn){

}

void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr&,muduo::net::Buffer*,muduo::Timestamp){

}