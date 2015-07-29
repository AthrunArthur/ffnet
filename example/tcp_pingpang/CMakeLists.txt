cmake_minimum_required(VERSION 2.8)
project(test_network)

#add_definitions(-DPROTO_BUF_SUPPORT)
add_definitions(-g)
add_definitions(-std=c++11)
#add_definitions(-DENABLE_LOG_CONSOLE)

####Specify the path of ffnet/network
set(FFNET_ROOT_DIR ${PROJECT_SOURCE_DIR}/../../network)


include_directories(${PROJECT_SOURCE_DIR} ${FFNET_ROOT_DIR})
include_directories(${FFNET_ROOT_DIR}/third_party_libs/build/include)
link_directories(${FFNET_ROOT_DIR}/lib)
link_directories(${FFNET_ROOT_DIR}/third_party_libs/build/lib)

message(${FFNET_ROOT_DIR}/lib)


set(svr_src server_main.cpp)
set(clnt_src client_main.cpp)

add_executable(server ${svr_src})
target_link_libraries(server network glog gflags)
add_executable(client ${clnt_src})
target_link_libraries(client network glog gflags)
