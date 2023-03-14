#include <iostream>
#include <string>
#include <vector>

// rpc发布方包含文件
#include "mprpcapplication.h"       
#include "friend.pb.h"
#include "rpcprovider.h"

class FriendService:public fixbug::FriendServiceRpc{ 
        
public:
    std::vector<std::string> GetFriendsList(uint32_t userid){// 模拟本地业务
        std::cout << "do GetFriendsList service! userid: "<<userid<<std::endl;
        std::vector<std::string>ret;
        ret.push_back("ke lai");
        ret.push_back("ling hua");
        ret.push_back("sha tang");
        return ret;
    }
     
    // 重写rpc基类方法， 最终由框架进行调用
    void GetFriendsList(::google::protobuf::RpcController* controller,
                    const ::fixbug::GetFriendsListRequest* request,
                    ::fixbug::GetFriendsListResponse* response,
                    ::google::protobuf::Closure* done){
        uint32_t userid = request->userid();
        
        std::vector<std::string> friendsList = GetFriendsList(userid);// 获取好友列表
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");

        for(std::string &name : friendsList){
            std::string *p  = response->add_friends();// 原声明: repeated bytes friends = 2;
            *p = name;// 修改其中内容
        }
        
        done->Run();
    }
};

int main(int argc,char** argv){

    // 框架初始化
    MprpcApplication::Init(argc,argv);

    RpcProvider provider;
    provider.NotifyService(new FriendService());
    provider.Run();

    return 0;
}