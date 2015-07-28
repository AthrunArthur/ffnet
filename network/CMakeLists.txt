cmake_minimum_required(VERSION 2.8)
project(network)


add_definitions(-g)
#add_definitions(-O2)
#add_definitions(-DENABLE_LOG_CONSOLE)

set(THIRD_PARTY_LIB_DIR ${PROJECT_SOURCE_DIR}/third_party_libs/)

if(NOT EXISTS ${THIRD_PARTY_LIB_DIR}/glog.zip)
  message(STATUS "require glog, downloading glog...")
  file(DOWNLOAD https://github.com/google/glog/archive/master.zip ${THIRD_PARTY_LIB_DIR}/glog.zip)
  exec_program(unzip ${THIRD_PARTY_LIB_DIR} ARGS glog.zip)
  exec_program(./configure ${THIRD_PARTY_LIB_DIR}/glog-master ARGS --prefix=${THIRD_PARTY_LIB_DIR}/build)
  exec_program(make ${THIRD_PARTY_LIB_DIR}/glog-master ARGS)
  exec_program(make ${THIRD_PARTY_LIB_DIR}/glog-master ARGS install)
endif(NOT EXISTS ${THIRD_PARTY_LIB_DIR}/glog.zip)

include_directories(${THIRD_PARTY_LIB_DIR}/build/include)
link_directories(${THIRD_PARTY_LIB_DIR}/build/lib)


include_directories(${PROJECT_SOURCE_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


set(src_list     common/shared_buffer.cpp
    common/mout.cpp
    middleware/typed_pkg_hub.cpp
    middleware/net_nervure.cpp
    middleware/length_packer.cpp
    network/asio_point.cpp
    network/net_buffer.cpp
    network/tcp_connection_base.cpp
    network/tcp_client.cpp
    network/tcp_server.cpp
    network/udp_point.cpp
    common/archive.cpp
    framework/application.cpp
    framework/routine.cpp
)

set(lib_list boost_system boost_date_time boost_thread boost_program_options)

find_package(Protobuf QUIET)
if(PROTOBUF_FOUND)
  #message(STATUS "found protobuf, and include protobuf support")
  #add_definitions(-DPROTO_BUF_SUPPORT)
  #include_directories(${PROTOBUF_INCLUDE_DIRS})
  #set(src_list ${src_list} extension/protobuf/proto_buf_wrapper_pkg.cpp
  #  extension/protobuf/protobuf_pkg_hub.cpp)
  #set(lib_list ${lib_list} ${PROTOBUF_LIBRARIES})
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

