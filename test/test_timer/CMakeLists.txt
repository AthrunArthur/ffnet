aux_source_directory(. SRC_LIST)

add_executable(test_timer ${SRC_LIST})
#target_link_libraries(test_timer ${LINK_LIBS})

add_test(test_timer test_timer)
