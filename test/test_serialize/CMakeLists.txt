
aux_source_directory(. SRC_LIST)
add_executable(serialize ${SRC_LIST})
target_link_libraries(serialize ${LINK_LIBS})

add_test(test_serialize serialize)

