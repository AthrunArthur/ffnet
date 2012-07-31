project(network)

include_directories(/home/athrun/projects/network/network/ /home/athrun/boost_1_46_1/)
link_directories(/home/athrun/boost_1_46_1/stage/lib/)

add_definitions(-DENABLE_LOG_CONSOLE)

set(src_list 	common/shared_buffer.cpp
	configure/nervure_config.cpp
	framework/global_connections.cpp
	framework/net_nervure.cpp
	handler/conn_holder_handler.cpp
	middleware/net_dispatcher.cpp
	middleware/length_bonder_splitter.cpp
	nervures/typed_net_nervure.cpp
	nervures/net_nervure_from_file.cpp
	network/asio_connection.cpp
	network/endpoint_data.cpp
	network/net_buffer.cpp
	network/tcp_connection_base.cpp
	network/tcp_client.cpp
	network/tcp_server.cpp
	network/udp_point.cpp
	package/package.cpp
	log.cpp
)
	
add_library(network ${src_list})

target_link_libraries(network boost_system boost_date_time boost_thread)

