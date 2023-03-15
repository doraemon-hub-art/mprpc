#include "logger.h"
#include <iostream>
#include <time.h>




Logger& Logger::GetInstance(){
    static Logger logger;
    return logger;
}

void Logger::SetLogLevel(int level){
    m_loglevel = level;
}


void Logger::Log(std::string msg){
    // 将日志信息写入logqueue缓冲区中，给外部用的
    m_logQue.Push(msg);
}

std::string Logger::GetLogLevel(){
    return LogLevelStr[m_loglevel];
}

Logger::Logger(){
    // 启动专门写日志的线程
    std::thread writeLogTask([&](){
        for(;;){
            // 获取当天的日期，然后获取日志信息，写入对应的日志文件中
            time_t now = time(nullptr);
            tm* nowtm = localtime(&now);

            char file_name[128];
            sprintf(file_name,"%d-%d-%d-log.txt",nowtm->tm_year+1900,nowtm->tm_mon+1,nowtm->tm_mday);
             
            FILE *pf = fopen(file_name,"a+");
            if(pf == nullptr){
                std::cout << "logger file : " << file_name << "open error!" <<std::endl;
                exit(EXIT_FAILURE);
            }
            std::string msg = m_logQue.Pop();// 插入到日志队列的时候并不会读取时间，这样是有问题的，现已修改

            // time_t now = time(nullptr);
            // tm* nowtm = localtime(&now);
            // // 给消息加上具体的时分秒信息
            // char time_buf[128] = {0};
            // sprintf(time_buf,"%04d-%02d-%02d %02d:%02d:%02d [%s] =>: ",nowtm->tm_year+1900,
            //                                                         nowtm->tm_mon+1,
            //                                                         nowtm->tm_mday,
            //                                                         nowtm->tm_hour,
            //                                                         nowtm->tm_min,
            //                                                         nowtm->tm_sec);      
            // msg.insert(0,time_buf);

            //msg.append("\n");// 换行
        

            fputs(msg.c_str(),pf);
            fclose(pf);
        }
    });
    // 设置分离线程，守护线程
    // ps: 开始写错位置了，写到lambda函数里面去了,导致子线程还没执行就被销毁了
    writeLogTask.detach();
}

