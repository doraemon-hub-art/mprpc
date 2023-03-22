#include <iostream>
#include "mprpcapplication.h" // 我们框架的头文件
#include "user.pb.h"
#include "mprpcchannel.h"

int main(int argc,char **argv){

    // 整个程序启动后，想使用mprpc框架来调用rpc服务

    // 先初始化
    MprpcApplication::Init(argc,argv);

    // 远程调用发布的rpc方法Login
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    // RpcChannel->RpcChannel::callMethod集中来做所有rpc方法调用的参数序列化和网络发送
    // rpchannel是个抽象类，我们需要将其继承然后重写其中的callmethod方法

    // rpc方法的请求参数
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123");
    // rpc方法的调用 同步的rpc调用
    fixbug::LoginResponse response;
    stub.Login(nullptr,&request,&response,nullptr);

    // 一次rpc调用的完成，读调用的结果
    if(response.result().errcode() == 0){
        std::cout<<"rpc login response:" <<response.success() <<std::endl;
    }else{
        std::cout<<"rpc login response error :"<<response.result().errmsg()<<std::endl;
    }
    
    // 调用rpc方法 Register
    fixbug::RegisterRequest req;
    req.set_id(20061128);
    req.set_name("zyx");
    req.set_pwd("123");
    fixbug::RegisterResponse rsp;
    stub.Register(nullptr,&req,&rsp,nullptr);

    // 以同步的方式发起rpc请求，等待返回结果
    if(rsp.result().errcode() == 0){
        std::cout << "rpc register response success! "<<rsp.success()<<std::endl;
    }else{
        std::cout << "rpc register response failed! "<<rsp.result().errmsg()<<std::endl;
    }
    return 0;
}