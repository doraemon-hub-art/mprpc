#include <iostream>

// rpc调用方需要包含的类
#include "mprpcapplication.h"
#include "friend.pb.h"


int main(int argc,char** argv){

    MprpcApplication::Init(argc,argv);

    fixbug::FriendServiceRpc_Stub stub(new MprpcChannel());

    fixbug::GetFriendsListRequest request;// 请求参数
    uint32_t userid = 20061128;
    request.set_userid(20061128);// 设置请求参数
    
    fixbug::GetFriendsListResponse response;// 接收参数
    
    // 发起调用,同步等待调用结束
    MprpcController controller;
    stub.GetFriendsList(&controller,&request,&response,nullptr);

    // 调用完成，验证结果
    // 实现controller之后，要先判断在其执行过程中有没有出现什么错误
    if(!controller.Failed()){
        if(response.result().errcode() == 0){// 成功
            std::cout<<"rpc GetFriendsList response success! "<< std::endl;
            // 查看返回的好友
            std::cout<< userid <<"'s friends: "<<std::endl;
            for(int i = 0; i < response.friends_size();i++){
               std::cout << "index: "<< i+1 <<": "<< response.friends(i) <<std::endl;
            }
        }else{// 失败
            std::cout << "rpc GetFriendsList error! errmsg: "<<response.result().errmsg()<< std::endl;
        }
    }else{// 不是一次正确的执行
        std::cout << controller.ErrorText() << std::endl;
    }
    
    

    return 0;
}