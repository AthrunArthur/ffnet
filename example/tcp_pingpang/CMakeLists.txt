cmake_minimum_required(VERSION 2.8)
project(test_network)

add_definitions(-DPROTO_BUF_SUPPORT)
add_definitions(-g)

####Specify the path of ffnet/network
set(FFNET_ROOT_DIR ${PROJECT_SOURCE_DIR}/../../network)

#####Specify the path of boost
#set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/athrun/boost_trunk/stage/include")
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/athrun/boost_trunk/stage/lib")

set(Boost_DIR /home/athrun/boost_1_53_0)
set(CMAKE_MODULE_PATH ${FFNET_ROOT_DIR}/cmake)
include_directories(${PROJECT_SOURCE_DIR} ${Boost_DIR} ${FFNET_ROOT_DIR})
link_directories(${Boost_DIR}/stage/lib ${FFNET_ROOT_DIR}/lib)


set(svr_src server_main.cpp)
set(clnt_src client_main.cpp)

add_executable(server ${svr_src})
target_link_libraries(server network boost_thread protobuf boost_date_time boost_system)
add_executable(client ${clnt_src})
target_link_libraries(client network boost_thread  protobuf boost_date_time boost_system)
