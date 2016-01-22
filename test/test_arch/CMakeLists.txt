 
aux_source_directory(. SRC_LIST)
add_executable(test_arch ${SRC_LIST})
target_link_libraries(test_arch ${LINK_LIBS})

add_test(test_archieve test_arch)
