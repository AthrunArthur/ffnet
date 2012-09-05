project(test_network)


include_directories(/home/athrun/projects/network/test_network/ /home/athrun/projects/network/network/ /home/athrun/boost_1_46_1/)
link_directories(/home/athrun/boost_1_46_1/stage/lib/ /home/athrun/projects/network/network/build/)
#add_definitions(-DENABLE_LOG_CONSOLE)

set(svr_src ping_pong/server_main.cpp
		common/message.cpp)
set(clnt_src ping_pong/client_main.cpp
		common/message.cpp)

add_executable(server ${svr_src})
target_link_libraries(server network boost_thread boost_date_time boost_system)
add_executable(client ${clnt_src})
target_link_libraries(client network boost_thread boost_date_time boost_system)
add_subdirectory(ping_pong)