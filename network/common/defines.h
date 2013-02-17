#ifndef NETWORK_COMMON_DEFINES_H_
#define NETWORK_COMMON_DEFINES_H_

#include "handler/event.h"
#include "common/log.h"

ENABLE_HOOK_EVENT(::ffnet::event::tcp_get_connection)
ENABLE_HOOK_EVENT(::ffnet::event::tcp_lost_connection)
//ENABLE_HOOK_EVENT(::ffnet::event::udp_send_recv_exception)


//!Please uncomment the following events to enable hooking them.

//ENABLE_HOOK_EVENT(::ffnet::event::more::tcp_server_start_listen)
//ENABLE_HOOK_EVENT(::ffnet::event::more::tcp_server_accept_connection)
//ENABLE_HOOK_EVENT(::ffnet::event::more::tcp_server_accept_error)
//ENABLE_HOOK_EVENT(::ffnet::event::more::tcp_start_recv_stream)
//ENABLE_HOOK_EVENT(::ffnet::event::more::tcp_start_send_stream)
//ENABLE_HOOK_EVENT(::ffnet::event::more::tcp_client_start_connection)
//ENABLE_HOOK_EVENT(::ffnet::event::more::tcp_client_get_connection_succ)
//ENABLE_HOOK_EVENT(::ffnet::event::more::tcp_client_conn_error)
//ENABLE_HOOK_EVENT(::ffnet::event::more::connect_sent_stream_succ)
//ENABLE_HOOK_EVENT(::ffnet::event::more::connect_sent_stream_error)
//ENABLE_HOOK_EVENT(::ffnet::event::more::connect_recv_stream_succ)
//ENABLE_HOOK_EVENT(::ffnet::event::more::connect_recv_stream_error)


DEF_LOG_MODULE(app)
DEF_LOG_MODULE(conf)
DEF_LOG_MODULE(tcp_server)
DEF_LOG_MODULE(udp_server)
DEF_LOG_MODULE(tcp_client)
DEF_LOG_MODULE(connection)
DEF_LOG_MODULE(frmwk)

ENABLE_LOG_MODULE(app)
//!Please uncomment the following log to enable logging them.
//ENABLE_LOG_MODULE(conf)
//ENABLE_LOG_MODULE(tcp_server)
//ENABLE_LOG_MODULE(udp_server)
//ENABLE_LOG_MODULE(tcp_client)
//ENABLE_LOG_MODULE(connection)
//ENABLE_LOG_MODULE(frmwk)

#endif
