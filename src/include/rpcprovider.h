#pragma once 
#include <google/protobuf/service.h>
//#include <memory>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <string>
#include <unordered_map>

// 框架提供的专门发布rpc服务的网络对象类
class RpcProvider{

public:
    // 这里是框架提供给外部使用的，可以发送rpc方法的函数接口
    void NotifyService(google::protobuf::Service * service);

    // 启动Rpc服务节点，开始提供rpc远程网络调用服务
    void Run();

private:
    
    // 组合EventLoop
    muduo::net::EventLoop m_eventLoop;

    struct ServiceInfo{
        const google::protobuf::Service *m_service;// 保存服务对象
        std::unordered_map<std::string,const google::protobuf::MethodDescriptor*>m_methodMap;//保存服务方法
    };
    
    // 存储注册成功服务服务对象和其服务方法的所有信息
    std::unordered_map<std::string,ServiceInfo> m_serviceMap;


private:
    // 新的socket连接回调
    void OnConnection(const muduo::net::TcpConnectionPtr &conn);

    // 读写以建立连接用户的读写事件回调
    void OnMessage(const muduo::net::TcpConnectionPtr&,muduo::net::Buffer*,muduo::Timestamp);
};