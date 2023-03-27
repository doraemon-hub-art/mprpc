# mprpc分布式通信框架

## 项目结构

- bin： 可执行文件 & 日志文件
- build: 项目编译文件
- example: 使用示例(未脱离框架代码)
- lib: 框架静态库输出目录
- src：源代码
- standalone_example: 独立测试，完整的RPC调用方 & 提供方测试。
- test: 测试代码(无用)
- example: 框架代码使用范例
- CMakeLists.txt: 框架顶层的CMake文件
- README.md: 项目自述文件
- autobuild.sh: 编译脚本

***
    
## 框架使用
### 环境配置
> 项目依赖
- CMake
- Protubuf
- Muduo
    - 同时安装其依赖boost库
- Zookeeper
    - 同时安装其依赖openjdk
- 拿到lib目录生成的头文件include和库文件libmprpc.a
***
### RPC服务发布者
> 注： 以下代码为standalone_example中的示例代码
> 先编写对应的protoc文件,这里我们叫做test.proto

- 根据你的需要，定义要发布的方法以及其相关参数的proto文件
    - 例如下面我们创建一个方法返回数字从1到n的阶乘,创建了一个名为test.proto的文件

```proto
syntax = "proto3";

package fixbug;

// C++编译选项
option cc_generic_services = true;

message ResultCode{
    int32 errcode = 1;
    bytes errmsg = 2;
}

message Factorial_Request{
    bytes num = 2;
}

message Factorial_Response{
    ResultCode result = 1;
    bool success = 2;
    int32 value = 3; 
}

// 要发布一个方法，需要让rpc的调用方看到你是怎么描述这个方法的
// 包括方法名，参数类型，响应类类型，即在Proto文件中定义
service Factorial_ServiceRpc{
    // 请求 & 响应
    rpc Factorial_Use(Factorial_Request) returns(Factorial_Request);
}
```

- 编译proto文件，生成对应的C++文件，等下我们要用到的对应的test.pb.h文件

```bash
protoc test.proto --cpp_out=./
```

> 开始编写RPC方法的定义与实现文件,这里我们叫做provider_test.cc

- 如下代码所示: 
    - 我们继承了刚才在protoc文件中定义的方法所生成的类，并重写了对应的方法。
    - 详见注释
```C++

#include "./include/mprpcapplication.h"
#include "test.pb.h"
#include "./include/rpcprovider.h"
#include "./include/logger.h" // 日志文件
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
```
> 对应CMakeLists.txt文件

```CMake
# 设置CMake最低版本和项目名称
cmake_minimum_required(VERSION 3.0)
# 设置项目名称
project(mprpc_provider_example)

# 设置支持C++版本
set(CMAKE_CXX_STANDARD 17)

# 设置头文件查找路径
include_directories(../include)

# 添加静态库搜素路径
link_directories(../lib)

# 添加要编译的文件
add_executable(mprpc_provider_example ../test.pb.cc provider_test.cc)

# 添加库 - 注意muduo_net 和 muduo_base的链接顺序
target_link_libraries(mprpc_provider_example 
                      libmprpc.a 
                      protobuf 
                      muduo_net 
                      muduo_base 
                      zookeeper_mt 
                      pthread)

# 将生成的可执行文件放到 bin 目录下
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
```
***
### RPC服务器调用者
> 开始编写RPC的调用方,这里我们叫做consumer_test.cc

- 如下代码所示:

```C++
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
```
***
### 编译 & 运行
> 成功返回结果。

```bash
Rpc Factorial_Use response success!
24
```
***

## 压力测试

