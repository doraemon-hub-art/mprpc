#pragma once
#include <google/protobuf/service.h>
#include <string>

// controller可以携带一些rpc在调用过程中的错误信息

class MprpcController:public google::protobuf::RpcController{

public:
    MprpcController();
    void Reset();
    bool Failed()const;
    std::string ErrorText()const;
    void SetFailed(const std::string& reason);

    // 目前未实现的一些功能,因为是纯虚函数，子类必须要重写
    void StartCancel();
    bool IsCanceled()const;
    void NotifyOnCancel(google::protobuf::Closure* callback);

private:
    bool m_failed; // RPC方法执行过程中的状态
    std::string m_errText; //RPC方法执行过程中的错误信息
};