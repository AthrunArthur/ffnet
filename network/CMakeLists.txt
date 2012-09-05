cmake_minimum_required(VERSION 2.8)
project(network)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

#####Specify the path of boost
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/home/athrun/boost_1_46_1")
set(CMAKE_LIBRARY_PATH${CMAKE_LIBRARY_PATH} "/home/athrun/boost_1_46_1/stage/lib")


find_package(Boost COMPONENTS thread system date_time)
if(Boost_FOUND)
	message(STATUS "boost include directory: ${Boost_INCLUDE_DIR}")
	message(STATUS "boost library directory: ${Boost_LIBRARY_DIRS}")
	message(STATUS "source code directory: ${PROJECT_SOURCE_DIR}")
	include_directories(${PROJECT_SOURCE_DIR} ${Boost_INCLUDE_DIR})
	link_directories(${Boost_LIBRARY_DIRS})
else(Boost_FOUND)
	message(FATAL_ERROR " Cannot find boost library. You can install boost or hack ${PROJECT_SOURCE_DIR}/CMakeList.txt to specify boost path")
endif(Boost_FOUND)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#####Open logs for network
#add_definitions(-DENABLE_LOG_CONSOLE)

set(src_list 	common/shared_buffer.cpp
	configure/nervure_config.cpp
	framework/global_connections.cpp
	framework/net_nervure.cpp
	handler/conn_holder_handler.cpp
	middleware/net_dispatcher.cpp
	middleware/length_bonder_splitter.cpp
	nervures/typed_net_nervure.cpp
	nervures/net_nervure_from_file.cpp
	network/asio_connection.cpp
	network/endpoint_data.cpp
	network/net_buffer.cpp
	network/tcp_connection_base.cpp
	network/tcp_client.cpp
	network/tcp_server.cpp
	network/udp_point.cpp
	package/package.cpp
	log.cpp
)
	
add_library(network_dynamic SHARED ${src_list})
add_library(network_static STATIC ${src_list})
target_link_libraries(network_dynamic boost_system boost_date_time boost_thread)
target_link_libraries(network_static boost_system boost_date_time boost_thread)


set_target_properties(network_dynamic PROPERTIES OUTPUT_NAME "network")
set_target_properties(network_static PROPERTIES OUTPUT_NAME "network")
set_target_properties(network_dynamic PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(network_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

