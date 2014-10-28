project(test_serialize)
cmake_minimum_required(VERSION 2.8)

set(project_path ${PROJECT_SOURCE_DIR})
set(ffnet_path ${project_path}/../../network)
message(${ffnet_path})

add_definitions(-DBOOST_TEST_DYN_LINK)
include_directories(${ffnet_path})
link_directories(${ffnet_path}/lib)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} network boost_unit_test_framework boost_system)

