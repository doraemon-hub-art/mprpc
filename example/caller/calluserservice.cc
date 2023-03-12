#include <iostream>
#include "mprpcapplication.h" // 我们框架的头文件
#include "user.pb.h"
#include "mprpcchannel.h"

int main(int argc,char **argv){

    // 整个程序启动后，想使用mprpc框架来调用rpc服务

    // 先初始化
    MprpcApplication::Init(argc,argv);
    
    // 远程调用发布的rpc方法Login
    fixbug::UserServiceRpc_Stub stub(new MpRpcChannel());
    // RpcChannel->RpcChannel::callMethod集中来做所有rpc方法调用的参数序列化和网络发送
    // rpchannel是个抽象类，我们需要将其继承然后重写其中的callmethod方法

    // rpc方法的请求参数
    fixbug::LoingRequest request;
    fixbuf::set_name("zhang san");
    request.set_pwd("123");
    // rpc方法的调用 同步的rpc调用
    fixbuf::LoginResponse response;
    stub.Login(nullptr,&request,&response,nullptr);

    // 一次rpc调用的完成，读调用的结果
    if(response.result().errcode() == 0){
        std::cout<<"rpc login response:" <<response.success() <<std::endl;
    }else{
        std::cout<<"rpc login response error :"<<response.result().errmsg()<<std::endl;
    }
    
    return 0;
}