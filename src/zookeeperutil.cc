#include "zookeeperutil.h"
#include "mprpcapplication.h"
#include <iostream>

// 全局watcher观察器，异步连接过程，本函数由一个独立的线程进行调用
void gobal_watcher(zhandle_t* zh,int type,int state,const char *path,void *watcherCtx){
    if(type == ZOO_SESSION_EVENT){// 回调的消息类型是和会话相关的消息类型
        if(state == ZOO_CONNECTED_STATE){// zkclient和zkserver连接成功
            sem_t* sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);// 连接成功，信号量+1
        }
    }
}

// 构造函数，初始化为空
ZkClient::ZkClient():m_zhandle(nullptr){

}

ZkClient::~ZkClient(){
    if(m_zhandle != nullptr){
        zookeeper_close(m_zhandle);// 关闭句柄，释放资源，MySQL_Conn
    }
}

void ZkClient::Start(){
    std::string zookeeperip = "zookeeperip";
    std::string zookeeperport = "zookeeperport";
    std::string host = MprpcApplication::GetInstance().GetConfig().Load(zookeeperip);
    std::string port = MprpcApplication::GetInstance().GetConfig().Load(zookeeperport);
    std::string connstr = host + ":" + port;

    /*
    zookeeper_mt: 多线程版本
    zookeeper的API客户端提供了三个线程
    API调用线程
    网络IO线程， pthread_create poll,客户端程序，不需要高并发
    watcher回调线程 pthread_create
    */

    // x - 回调函数 - 会话超时时间 - x - x - x
    // 创建句柄资源
    m_zhandle = zookeeper_init(connstr.c_str(),gobal_watcher,30000,nullptr,nullptr,0);
    if(m_zhandle == nullptr){
        std::cout << "zookeeper_init error!"<<std::endl;
        exit(EXIT_FAILURE);
    }

    sem_t sem;
    sem_init(&sem,0,0);
    // 创建上下文，给指定句柄添加一些额外的信息
    zoo_set_context(m_zhandle,&sem);


    sem_wait(&sem);
    std::cout << "zookeeper_init access!" <<std::endl;
}

void ZkClient::Create(const char *path, const char *data, int datalen, int state){
    char path_buff[128];
    int bufferlen = sizeof(path_buff);
    
    // 先判断path表示的znode节点是否存在，如果存在，就不用再重复创建了
    int flag = zoo_exists(m_zhandle,path,0,nullptr);
    if(flag == ZNONODE){// 表示path的znode节点不存在
        // 创建指定path的znode节点
        flag = zoo_create(m_zhandle,path,data,datalen,&ZOO_OPEN_ACL_UNSAFE,state,path_buff,bufferlen);
        
        if(flag == ZOK){
            std::cout << "znode create success! path: "<< path << std::endl;
        }else{
            std::cout << "flag: " << flag << std::endl;
            std::cout << "znode create error... path" << path << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

std::string ZkClient::GetData(const char *path)
{
    char buffer[64];// 存放获取的结点内容
    int bufferlen = sizeof(buffer);
    int flag = zoo_get(m_zhandle,path,0,buffer,&bufferlen,nullptr);
    if(flag != ZOK){
        std::cout << "get znode error... path: "<<path<<std::endl;
        return "";
    }else{
        return buffer;
    }
}
