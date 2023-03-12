# 设置头文件搜索路径
set(SRC_LIST calluserservice.cc ../user.pb.cc)
# 生成可执行文件
add_executable(consumer ${SRC_LIST})
# 链接动态库
target_link_libraries(consumer mprpc protobuf)