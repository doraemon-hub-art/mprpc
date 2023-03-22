#include "mprpcchannel.h"
#include <string>
#include "rpcheader.pb.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "mprpcapplication.h"
#include <netinet/in.h>
#include <unistd.h>
#include "zookeeperutil.h"

void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method,
                             google::protobuf::RpcController *controller, 
                             const google::protobuf::Message *request, 
                             google::protobuf::Message *response, 
                             google::protobuf::Closure *done){
    
    // 数据格式: header_size + service_name + method_name + args_size + args

    // 获取服务对象
    const google::protobuf::ServiceDescriptor* sd = method->service(); 
    // service_name
    std::string service_name = sd->name();
    // method_name
    std::string method_name = method->name();

    // 获取参数的序列化字符串长度 args_size
    std::string args_str;
    uint32_t args_size = 0;
    if(request->SerializePartialToString(&args_str)){
        args_size = args_str.size();
    }else{
        //std::cout<<"serialize request error!"<<std::endl;
        // 实现controller之后，就可以在return出错的时候收集错误信息
        controller->SetFailed("serialize request error!");// 失败
        return ;
    }

    // 定义rpc请求header
    mprpc::RpcHeader rpcheader;
    rpcheader.set_service_name(service_name);
    rpcheader.set_method_name(method_name);
    rpcheader.set_args_size(args_size);

    uint32_t header_size = 0;
    std::string rpc_header_str;
    if(rpcheader.SerializePartialToString(&rpc_header_str)){
        header_size = rpc_header_str.size();
    }else{
        //std::cout<<"serialize rpc header error!"<<std::endl;
        controller->SetFailed("serialize rpc header error!");// 失败
        return ;
    }

    // 组装待发送的rpc请求的字符串
    std::string send_rpc_str;
    //send_rpc_str.insert(0,std::string((char*)&header_size),4);// 将整型以二进制形式存储到string中
    send_rpc_str.insert(0,std::string((char*)&header_size,4));// 将整型以二进制形式存储到string中
    send_rpc_str += rpc_header_str;
    send_rpc_str += args_str;

    // std::cout << "================"<<std::endl;
    // std::cout <<"send_rpc_str: "<<send_rpc_str << std::endl;
    // std::cout << "================"<<std::endl;

    // 使用tcp编程，完成rpc方法的远程调用

    int client_fd = socket(AF_INET,SOCK_STREAM,0);// 可以定义一个智能指针，自定义它的删除器
    if(client_fd == -1){
        //std::cout << "create socket error! errno: "<<errno<<std::endl;
        //exit(EXIT_FAILURE);
        char err_buf[512] = {0};
        sprintf(err_buf,"create socket error! errno: %d",errno);
        controller->SetFailed(err_buf);// 失败
        return ;
    }
    
    

    // 读取配置文件的rpcserver的信息
    // std::string rpcserverip = "rpcserverip";
    // std::string rpcserverport = "rpcserverport";
    // std::string ip = MprpcApplication::GetInstance().GetConfig().Load(rpcserverip);
    // uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load(rpcserverport).c_str());

    // ===============================

    // rpc调用方想用service_name的method_name服务，需要查询zk上该服务所在的host信息
    ZkClient zkCli;
    zkCli.Start();
    std::string method_path = "/" + service_name + "/" + method_name;
    std::string host_data = zkCli.GetData(method_path.c_str());
    if(host_data == ""){
        controller->SetFailed(method_path + " is not exist!");
        return ;
    }
    int idx = host_data.find(":");
    if(idx == -1){
        controller->SetFailed(method_path +" adress is invalid!");
        return ;
    }
    std::string ip = host_data.substr(0,idx);
    uint16_t port = atoi(host_data.substr(idx+1,host_data.size()-idx).c_str());

    // ===============================

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    
    // 目前是直连状态，后期将会使用zookeeper找到所在结点
    if(connect(client_fd,(struct sockaddr*)&server_addr,sizeof(server_addr)) == -1){
        // 连接失败
        //std::cout << "connect error! errno: "<<errno<<std::endl;   
        close(client_fd);
        char err_buf[512] = {0};
        sprintf(err_buf,"connect error! errno: %d",errno);
        controller->SetFailed(err_buf);// 失败
        //exit(EXIT_FAILURE);
        return ;
    }

    // 发送RPC请求
    if(send(client_fd,send_rpc_str.c_str(),send_rpc_str.size(),0) == -1){
        //std::cout << "send error! errno: "<<errno << std::endl;
        close(client_fd);
        char err_buf[512] = {0};
        sprintf(err_buf,"send error! errno: %d",errno);
        controller->SetFailed(err_buf);// 失败
        return ;
    }

    // 接收RPC请求的响应值
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if((recv_size = recv(client_fd,recv_buf,1024,0)) == -1){
        //std::cout << "recv error! errno: "<<errno << std::endl;
        close(client_fd);
        char err_buf[512] = {0};
        sprintf(err_buf,"recv error! errno: %d",errno);
        controller->SetFailed(err_buf);// 失败
        return ;
    }

    // 反序列化rpc调用的响应数据
    //std::string response_str(recv_buf,0,recv_size);// string遇到\0结束，引起bug

    //if(!response->ParseFromString(response_str)){
    if(!response->ParseFromArray(recv_buf,recv_size)){
        //std::cout<<"parse error! response_str:"<<response_str << std::endl;
        close(client_fd);
        char err_buf[2048] = {0};
        sprintf(err_buf,"parse error! response_str: %s",recv_buf);
        return ;
    }
    // 无论是否出错，都要将socket fd关闭
    close(client_fd);
}
