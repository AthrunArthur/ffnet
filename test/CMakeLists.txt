cmake_minimum_required(VERSION 2.8)
project(test)

add_definitions(-g)
enable_testing()

####Specify the path of ffnet/network
set(FFNET_ROOT_DIR ${PROJECT_SOURCE_DIR}/../network)
include_directories(${FFNET_ROOT_DIR})
link_directories(${FFNET_ROOT_DIR}/lib)

find_package(Boost)            
if(Boost_FOUND)
  message(STATUS "found boost: ${Boost_LIBRARY_DIRS}")
  include_directories(${Boost_INCLUDE_DIR})
  link_directories(${Boost_LIBRARY})
  SET(LINK_LIBS boost_test_exec_monitor network boost_thread boost_date_time boost_system boost_serialization pthread)
else (Boost_FOUND)
  message(FATAL_ERROR "can't find boost")
endif (Boost_FOUND)

add_subdirectory(test_arch)
add_subdirectory(test_serialize)


add_executable(event_handler event_handler.cpp)
target_link_libraries(event_handler ${LINK_LIBS})
add_test(test_event_handler event_handler)

