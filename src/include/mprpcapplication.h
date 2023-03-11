#pragma once

#include "mprpcconfig.h"

// mprpc框架的基础类
class MprpcApplication{

public:
    
    // 初始化
    static void Init(int argc,char **argv);
    // 单例模式
    static MprpcApplication& GetInstance();
private:
    MprpcApplication(){}
    MprpcApplication(const  MprpcApplication&) = delete;// 声明为delete，即虽然声明但是不能使用
    MprpcApplication(MprpcApplication&&) = delete;
private:
    static MprpcConfig m_config;
}; 