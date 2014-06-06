cmake_minimum_required(VERSION 2.8)
project(udp_pingpong)

add_definitions(-DPROTO_BUF_SUPPORT)
add_definitions(-g)


####Specify the path of ffnet/network
set(FFNET_ROOT_DIR ${PROJECT_SOURCE_DIR}/../../network)

#####Specify the path of boost
#set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/athrun/boost_trunk/stage/include")
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/home/athrun/boost_trunk/stage/lib")

include_directories(${PROJECT_SOURCE_DIR} ${FFNET_ROOT_DIR})
link_directories(${FFNET_ROOT_DIR}/lib)


set(svr_src server_main.cpp message.pb.cc)
set(clnt_src client_main.cpp message.pb.cc)

add_executable(server ${svr_src})
target_link_libraries(server network boost_thread boost_date_time boost_system boost_serialization pthread protobuf)
add_executable(client ${clnt_src})
target_link_libraries(client network boost_thread  boost_date_time boost_system  boost_serialization pthread protobuf)
