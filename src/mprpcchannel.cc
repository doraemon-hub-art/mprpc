#include "mprpcchannel.h"
#include <string>
#include "rpcheader.pb.h"
#include <sys/types.h>
#include <sys/socket.h>

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
    std::string method_name = sd->name();

    // 获取参数的序列化字符串长度 args_size
    std::string args_str;
    uint32_t args_size = 0;
    if(request->SerializePartialToString(&args_str)){
        args_size = args_str.size();
    }else{
        std::cout<<"serialize request error!"<<std::endl;
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
        std::cout<<"serialize rpc header error!"<<std::endl;
    }

    // 组装待发送的rpc请求的字符串
    std::string send_rpc_str;
    send_rpc_str.insert(0,std::string((char*)&header_size),4);// 将整型以二进制形式存储到string中
    send_rpc_str += rpc_header_str;
    send_rpc_str += args_str;

    std::cout << "================"<<std::endl;
    std::cout <<"send_rpc_str: "<<send_rpc_str << std::endl;
    std::cout << "================"<<std::endl;

    // 使用tcp编程，完成rpc方法的远程调用

    int client_fd = socket(AF_INET,SOCK_STREAM,0);
    if(client_fd == -1){
        std::cout << "create socket error! errno: "<<errno<<std::endl;
        exit(EXIT_FAILURE);
    }
    
}
