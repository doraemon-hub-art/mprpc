# 设置头文件搜索路径
set(SRC_LIST userservice.cc ../user.pb.cc)
# 生成可执行文件
add_executable(provider ${SRC_LIST})
# 链接动态库
target_link_libraries(provider mprpc protobuf)