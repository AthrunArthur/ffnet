project(test_serialize)
cmake_minimum_required(VERSION 2.8)

set(project_path ${PROJECT_SOURCE_DIR})
include_directories(${project_path}../../network)
link_directories(${project_path}../../network/lib)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

