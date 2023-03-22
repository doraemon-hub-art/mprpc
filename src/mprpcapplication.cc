#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>

MprpcConfig MprpcApplication::m_config;

void ShowArgsHelp(){
    std::cout<<"format: commad -i < confgfile >"<<std::endl;
}

void MprpcApplication::Init(int argc,char **argv){
    if(argc < 2){
        ShowArgsHelp();// 静态成员只能调用静态成员函数
        exit(EXIT_FAILURE);
    }
    int c = 0;
    std::string config_file;
    while((c = getopt(argc,argv,"i:")) != -1){
        switch (c)
        {
        case 'i':
            config_file = optarg;
            break;
        case '?':
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        case ':':
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }
    // 开始加载配置文件
    // rpcserver_ip rpcserver_port zookeeper_ip zookeeper_port
    m_config.LoadConfigFile(config_file.c_str());

    // test
    std::string rpcserverip = "rpcserverip";
    std::cout<<m_config.Load(rpcserverip) <<std::endl;
}

MprpcApplication& MprpcApplication::GetInstance(){
    static MprpcApplication app;
    return app;
}

MprpcConfig& MprpcApplication::GetConfig(){
    return m_config;
}