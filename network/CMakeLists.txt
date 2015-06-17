cmake_minimum_required(VERSION 2.8)
project(network)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#add_definitions(-DPROTO_BUF_SUPPORT)
add_definitions(-g)
#add_definitions(-O2)
#add_definitions(-DENABLE_LOG_CONSOLE)


include_directories(${PROJECT_SOURCE_DIR})

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


set(src_list     common/shared_buffer.cpp
    common/log/logger.cpp
    common/log/logwriter.cpp
    common/log.cpp
    #framework/nervure_config.cpp
    #framework/global_connections.cpp
    #framework/NetNervure.cpp
    #middleware/net_dispatcher.cpp
    middleware/typed_pkg_hub.cpp
    #nervures/net_nervure_from_file.cpp
    middleware/net_nervure.cpp
    #nervures/thread_nervure.cpp
    #nervures/proto_buf_nervure.cpp
    network/asio_point.cpp
    network/net_buffer.cpp
    network/tcp_connection_base.cpp
    network/tcp_client.cpp
    network/tcp_server.cpp
    network/udp_point.cpp
    #Package/proto_buf_wrapper_pkg.cpp
    common/archive.cpp
)

set(lib_list boost_system boost_date_time boost_thread)

find_package(Protobuf QUIET)
if(PROTOBUF_FOUND)
message(STATUS "found protobuf, and include protobuf support")
add_definitions(-DPROTO_BUF_SUPPORT)
include_directories(${PROTOBUF_INCLUDE_DIRS})
set(src_list ${src_list} extension/protobuf/proto_buf_wrapper_pkg.cpp
    extension/protobuf/protobuf_pkg_hub.cpp)
set(lib_list ${lib_list} ${PROTOBUF_LIBRARIES})
else()
message(STATUS "not found protobuf, gonna ignore protobuf support")
endif()
    
add_library(network_dynamic SHARED ${src_list})
#add_library(network_static STATIC ${src_list})
target_link_libraries(network_dynamic ${lib_list})
#target_link_libraries(network_static protobuf boost_system boost_date_time boost_thread)


set_target_properties(network_dynamic PROPERTIES OUTPUT_NAME "network")
#set_target_properties(network_static PROPERTIES OUTPUT_NAME "network")
set_target_properties(network_dynamic PROPERTIES CLEAN_DIRECT_OUTPUT 1)
#set_target_properties(network_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

