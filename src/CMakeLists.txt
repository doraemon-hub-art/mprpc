# aux_source_directory(. SRC_LIST)
# set(SRC_LIST mprpcapplication.cc mprpcconfig.cc rpcheader.pb.cc rpcprovider.pb.cc)

set(SRC_LIST 
    mprpcapplication.cc 
    mprpcconfig.cc 
    rpcheader.pb.cc 
    rpcprovider.cc 
    mprpcchannel.cc
    mprpccontroller.cc
    logger.cc
    zookeeperutil.cc)

add_library(mprpc ${SRC_LIST})
# muduo_base依赖 net，所以注意链接顺序
target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt) 