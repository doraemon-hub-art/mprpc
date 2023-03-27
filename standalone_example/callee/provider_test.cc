
#include "../include/mprpcapplication.h"
#include "../test.pb.h"
#include "../include/rpcprovider.h"
#include "../include/logger.h" // 日志文件

#include <iostream>

uint32_t factorial(uint32_t n){
    if(n == 1)return 1;
    return n * factorial(n-1);
}

// 继承我们刚才在rpc方法中定义的方法所生成的类
class Factorial_Service:public fixbug::Factorial_ServiceRpc{

public:
    // 重写我们定义的对应方法，找到对应的方法名
    void Factorial_Use(::google::protobuf::RpcController* controller,
                       const ::fixbug::Factorial_Request* request,
                       ::fixbug::Factorial_Response* response,
                       ::google::protobuf::Closure* done){
        // 获取请求参数 
        uint32_t num = request->num();
        // 业务处理
        uint32_t ret = factorial(num);
        
        // 返回结果,将响应写入
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_value(ret);

        // 执行回调操作
        done->Run();
    }

};

int main(int argc,char **argv){

    // 调用框架的初始化功能
    MprpcApplication::Init(argc,argv);

    // 发布一个Rpc方法
    RpcProvider provider;
    provider.NotifyService(new Factorial_Service());

    // 启动一个rpc服务发布节点
    provider.Run();
    return 0;
}