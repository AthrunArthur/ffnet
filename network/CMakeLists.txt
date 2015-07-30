cmake_minimum_required(VERSION 2.8)
project(network)


add_definitions(-g)
add_definitions(-std=c++11)
#add_definitions(-O2)
#add_definitions(-DENABLE_LOG_CONSOLE)

set(THIRD_PARTY_LIB_DIR ${PROJECT_SOURCE_DIR}/third_party_libs/)

function(download_lib libname url)
  if(NOT EXISTS ${THIRD_PARTY_LIB_DIR}/${libname}.zip)
    message(STATUS "require " ${libname} ", downloading...")
    file(DOWNLOAD ${url} ${THIRD_PARTY_LIB_DIR}/${libname}.zip)
    exec_program(unzip ${THIRD_PARTY_LIB_DIR} ARGS ${libname}.zip)
    if(EXISTS ${THIRD_PARTY_LIB_DIR}/${libname}-master/configure)
      exec_program(./configure ${THIRD_PARTY_LIB_DIR}/${libname}-master ARGS --prefix=${THIRD_PARTY_LIB_DIR}/build)
      exec_program(make ${THIRD_PARTY_LIB_DIR}/${libname}-master ARGS)
      exec_program(make ${THIRD_PARTY_LIB_DIR}/${libname}-master ARGS install)
    else()
      if(EXISTS ${THIRD_PARTY_LIB_DIR}/${libname}-master/CMakeLists.txt)
        exec_program(cmake ${THIRD_PARTY_LIB_DIR}/${libname}-master ARGS -DCMAKE_INSTALL_PREFIX=${THIRD_PARTY_LIB_DIR}/build)
        exec_program(make ${THIRD_PARTY_LIB_DIR}/${libname}-master)
        exec_program(make ${THIRD_PARTY_LIB_DIR}/${libname}-master ARGS install)
      else()
        message(WARNING "cannot build " ${libname})
      endif()
    endif()
  endif(NOT EXISTS ${THIRD_PARTY_LIB_DIR}/${libname}.zip)
endfunction(download_lib)


download_lib(glog https://github.com/google/glog/archive/master.zip)
download_lib(gflags https://github.com/gflags/gflags/archive/master.zip)

download_lib(asio https://github.com/chriskohlhoff/asio/archive/master.zip)
exec_program(cp ${THIRD_PARTY_LIB_DIR}/asio-master/asio/include ARGS -rf * ${THIRD_PARTY_LIB_DIR}/build/include/)


include_directories(${THIRD_PARTY_LIB_DIR}/build/include)
link_directories(${THIRD_PARTY_LIB_DIR}/build/lib)

include_directories(${PROJECT_SOURCE_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


set(src_list     common/shared_buffer.cpp
  common/mout.cpp
  common/archive.cpp
  common/timer.cpp
  middleware/typed_pkg_hub.cpp
  middleware/net_nervure.cpp
  middleware/length_packer.cpp
  network/asio_point.cpp
  network/net_buffer.cpp
  network/tcp_connection_base.cpp
  network/tcp_client.cpp
  network/tcp_server.cpp
  network/udp_point.cpp
  framework/application.cpp
  framework/flags.cpp
  framework/routine.cpp
  simu_net/simu_server.cpp
  simu_net/net_simu_base.cpp
  simu_net/network/simu_udp_point.cpp
  )

set(lib_list glog gflags)

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

