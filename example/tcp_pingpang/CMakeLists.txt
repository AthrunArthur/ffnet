cmake_minimum_required(VERSION 2.8)
project(test_network)

#add_definitions(-DPROTO_BUF_SUPPORT)
add_definitions(-g)
add_definitions(-DENABLE_LOG_CONSOLE)

####Specify the path of ffnet/network
set(FFNET_ROOT_DIR ${PROJECT_SOURCE_DIR}/../../network)


include_directories(${PROJECT_SOURCE_DIR} ${FFNET_ROOT_DIR})
link_directories(${FFNET_ROOT_DIR}/lib)


set(svr_src server_main.cpp)
set(clnt_src client_main.cpp)

add_executable(server ${svr_src})
target_link_libraries(server network boost_thread boost_date_time boost_system)
add_executable(client ${clnt_src})
target_link_libraries(client network boost_thread boost_date_time boost_system)
