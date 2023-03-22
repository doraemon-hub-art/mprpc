#pragma once

#include "logqueue.h"
#include <unordered_map>
#include <string>
#include <cstring>


// MpRpc日志系统

// enum  LogLevel{
//     INFO, // 普通信息
//     ERROR,// 错误信息
//     WARN // 警告信息
// };

#define INFO 1
#define ERROR 2
#define WARN 3

// 存储对应的日志级别与其string形式的意义
// 加完static 后不报错了
static std::unordered_map<int,std::string>LogLevelStr{{INFO,"INFO"},{ERROR,"ERROR"},{WARN,"WARN"}};

class Logger{

public:
    // 获取单例句柄
    static Logger& GetInstance();
    // 设置日志级别
    void SetLogLevel(int level);
    // 写日志
    void Log(std::string msg);
    // 获取当前日志级别的string形式
    std::string GetLogLevel();

private:
    // 设置成单例模式
    Logger();
    Logger(const Logger &) = delete;
    Logger(Logger&&) = delete;

    // 获取当前的时间
private:
    int m_loglevel;// 日志级别
    LockQueue<std::string> m_logQue; // 日志缓冲队列
    
};

// 组合日志前缀 + 日志消息
static void  CombinedStr(char* msg,char* ret){
    time_t now = time(nullptr);
    tm* nowtm = localtime(&now);
    // 给消息加上具体的时分秒信息
    sprintf(ret,"%04d-%02d-%02d %02d:%02d:%02d [%s] =>: ",nowtm->tm_year+1900,
                                                            nowtm->tm_mon+1,
                                                            nowtm->tm_mday,
                                                            nowtm->tm_hour,
                                                            nowtm->tm_min,
                                                            nowtm->tm_sec,
                                                            Logger::GetInstance().GetLogLevel().c_str());
    strcat(ret,msg);
    char tmp[2] = "\n";// 追加一个换行
    strcat(ret,tmp);
}

// 定义日志宏
#define LOG_INFO(logmsgformat,...)\
    do \
    { \
        Logger &logger = Logger::GetInstance();\
        logger.SetLogLevel(INFO); \
        char c[1024] = {0}; \
        char ret[1024] = {0}; \
        snprintf(c,1024,logmsgformat,##__VA_ARGS__);\
        CombinedStr(c,ret); \
        logger.Log(ret); \
    }while(0);

#define LOG_ERROR(logmsgformat,...)\
    do \
    { \
        Logger &logger = Logger::GetInstance();\
        logger.SetLogLevel(ERROR); \
        char c[1024] = {0}; \
        char ret[1024] = {0}; \
        snprintf(c,1024,logmsgformat,##__VA_ARGS__);\
        CombinedStr(c,ret); \
        logger.Log(ret); \
    }while(0);

#define LOG_WARN(logmsgformat,...)\
    do \
    { \
        Logger &logger = Logger::GetInstance();\
        logger.SetLogLevel(WARN); \
        char c[1024] = {0}; \
        char ret[1024] = {0}; \
        snprintf(c,1024,logmsgformat,##__VA_ARGS__);\
        CombinedStr(c,ret); \
        logger.Log(ret); \
    }while(0);
