#include <iostream>
#include "../include/mprpcapplication.h"
#include "../test.pb.h"

int main(int argc,char **argv){
    MprpcApplication::Init(argc,argv);
    fixbug::Factorial_ServiceRpc_Stub stub(new MprpcChannel());

    // 准备请求参数
    fixbug::Factorial_Request request;
    uint32_t num = 4;// 测试4的阶乘
    request.set_num(num);
    
    // 请求响应
    fixbug::Factorial_Response response;

    // 发起调用，同步等待调用结束
    MprpcController controller;
    stub.Factorial_Use(&controller,&request,&response,nullptr);

    // 验证结果
    // 成功发送RPC请求
    if(!controller.Failed()){  
        // 业务处理正确
        if(response.result().errcode() == 0){
            std::cout << "Rpc Factorial_Use response success!" << std::endl;
            // 查看返回内容
            std::cout << response.value() << std::endl;
        }else{// 业务处理中发生错误
            std::cout << "Rpc Factorial_Use error! errmsg: "<<response.result().errmsg()<<std::endl;
        }
    }else{// 发送请求过程中遇到错误
        std::cout << "Rpc Factoriak_Use error! errmsg: "<<response.result().errmsg()<<std::endl;
    }

    return 0;
}