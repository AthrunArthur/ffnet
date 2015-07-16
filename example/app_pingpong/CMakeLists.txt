cmake_minimum_required(VERSION 2.8)
project(test_network)

#add_definitions(-DPROTO_BUF_SUPPORT)
add_definitions(-g)
#add_definitions(-DENABLE_LOG_CONSOLE)

####Specify the path of ffnet/network
set(FFNET_ROOT_DIR ${PROJECT_SOURCE_DIR}/../../network)


include_directories(${PROJECT_SOURCE_DIR} ${FFNET_ROOT_DIR})
link_directories(${FFNET_ROOT_DIR}/lib)
message(${FFNET_ROOT_DIR}/lib)



add_executable(pingpong main.cpp)
target_link_libraries(pingpong network boost_thread boost_date_time boost_system boost_program_options)
