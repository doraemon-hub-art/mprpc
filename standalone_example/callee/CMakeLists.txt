# 设置CMake最低版本和项目名称
cmake_minimum_required(VERSION 3.0)
# 设置项目名称
project(mprpc_provider_example)

# 设置支持C++版本
set(CMAKE_CXX_STANDARD 17)

# 设置头文件查找路径
include_directories(../include)

# 添加静态库搜素路径
link_directories(../lib)

# 添加要编译的文件
add_executable(mprpc_provider_example ../test.pb.cc provider_test.cc)

# 添加库 - 注意muduo_net 和 muduo_base的链接顺序
target_link_libraries(mprpc_provider_example 
                      libmprpc.a 
                      protobuf 
                      muduo_net 
                      muduo_base 
                      zookeeper_mt 
                      pthread)

# 将生成的可执行文件放到 bin 目录下
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../bin)