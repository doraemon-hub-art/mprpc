# mprpc分布式通信框架

## 文件结构

- bin： 可执行文件
- build: 项目编译文件
- src：源代码
- test: 测试代码
- example: 框架代码使用范例
- CMakeLists.txt: 顶层的cmake文件
- README.md: 项目自述文件
- autobuild.sh: 编译脚本

***

## 项目相关库配置
- cmake
- muduo
    - boost
- protobuf
- zookeeper
    - openjdk 

***
## 编译
```bash
cd build
cmake ..
```


***
    
## 框架使用
> 项目依赖
- protubuf
- muduo
    - muduo依赖boost所以要先安装boost
- zookeeper
    - 
- 拿到lib目录生成的头文件include和库文件libmprpc.a

### RPC服务发布者

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








### RPC服务器调用者