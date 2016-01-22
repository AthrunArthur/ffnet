cmake_minimum_required(VERSION 2.8)
project(basic_tcp_pingpong)

#add_definitions(-DPROTO_BUF_SUPPORT)
add_definitions(-g)
add_definitions(-DENABLE_LOG_CONSOLE)

####Specify the path of ffnet/network
set(FFNET_ROOT_DIR ${PROJECT_SOURCE_DIR}/../../network)


include_directories(${PROJECT_SOURCE_DIR} ${FFNET_ROOT_DIR})
link_directories(${FFNET_ROOT_DIR}/lib)
message(${FFNET_ROOT_DIR}/lib)


add_executable(tcp_server tcp_server.cpp)
target_link_libraries(tcp_server network boost_thread boost_date_time boost_system)
add_executable(udp_server udp_server.cpp)
target_link_libraries(udp_server network boost_thread boost_date_time boost_system)
add_executable(tcp_echo tcp_echo.cpp)
target_link_libraries(tcp_echo network boost_thread boost_date_time boost_system)
add_executable(udp_echo udp_echo.cpp)
target_link_libraries(udp_echo network boost_thread boost_date_time boost_system)
