#pragma once 

#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>

// 为什么要使用zookeeper?
// rpc服务提供端，要在rpc服务结点启动之前，要把它上面所发布的服务网zk上注册；
// zk在项目中作为一个服务配置中心
// services_name作为一个永久结点，下面为它所提供的方法method_name,临时性结点
// 在这个临时性结点上，存储的这个服务所在的ip地址和端口号
// rpc方调用方法时，需要知道对应的方法在哪台服务器上运行
// zk类似于linux中的文件系统，有根目录

// util一本就是工具类的意思

// 封装的zk客户端类
class ZkClient{
public:
    ZkClient();
    ~ZkClient();
    // 启动连接zkServer
    void Start();
    // 在zkServer根据指定的path创建指定的结点，0位永久性结点
    void Create(const char* path,const char* data,int datalen,int state = 0);
    // 根据参数指定的znode结点路径，获取znode结点的值
    std::string GetData(const char* path);

private:
    // zk的的客户端句柄
    zhandle_t* m_zhandle;

};