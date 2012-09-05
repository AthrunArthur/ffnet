cmake_minimum_required(VERSION 2.8)
project(test_network)

####Specify the path of ffnet/network
set(FFNET_ROOT_DIR ${PROJECT_SOURCE_DIR}/../../network)

#####Specify the path of boost
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/athrun/boost_1_46_1")
set(CMAKE_LIBRARY_PATH${CMAKE_LIBRARY_PATH} "/home/athrun/boost_1_46_1/stage/lib")


set(CMAKE_MODULE_PATH ${FFNET_ROOT_DIR}/cmake)


find_package(Boost COMPONENTS thread system date_time)
if(Boost_FOUND)
	message(STATUS "boost include directory: ${Boost_INCLUDE_DIR}")
	message(STATUS "boost library directory: ${Boost_LIBRARY_DIRS}")
	message(STATUS "source code directory: ${PROJECT_SOURCE_DIR}")
	include_directories(${PROJECT_SOURCE_DIR} ${Boost_INCLUDE_DIR} ${FFNET_ROOT_DIR})
	link_directories(${Boost_LIBRARY_DIRS} ${FFNET_ROOT_DIR}/lib)
else(Boost_FOUND)
	message(FATAL " Cannot find boost library. You can install boost or hack ${PROJECT_SOURCE_DIR}/CMakeList.txt to specify boost path")
endif(Boost_FOUND)


#add_definitions(-DENABLE_LOG_CONSOLE)

set(svr_src server_main.cpp)
set(clnt_src client_main.cpp)

add_executable(server ${svr_src})
target_link_libraries(server network boost_thread boost_date_time boost_system)
add_executable(client ${clnt_src})
target_link_libraries(client network boost_thread boost_date_time boost_system)