#ifndef _NETWORK_EVENTS_H
#define _NETWORK_EVENTS_H
#include "common.h"
#include <boost/function.hpp>
#include "network/tcp_connection_base.h"
#include "network/tcp_client.h"
#include "network/tcp_server.h"
#include "network/udp_point.h"


namespace ffnet
{
namespace event
{
struct tcp_get_connection
{
    const static size_t identifier=1;
    typedef ffnet::TCPConnectionBase TCPConnectionBase;
    typedef boost::function<void (TCPConnectionBase *)> Handler_t;
};//end tcp_get_connection
struct tcp_lost_connection
{
    const static size_t identifier=2;
    typedef ffnet::EndpointPtr_t EndpointPtr_t;
    typedef boost::function<void (EndpointPtr_t)> Handler_t;
};//end tcp_get_connection

//! This happens because the socked is invalid due to lost connection or errors
struct pkg_send_failed
{
    const static size_t identifier=3;
    typedef ffnet::EndpointPtr_t EndpointPtr_t;
    typedef ffnet::PackagePtr_t PackagePtr_t;
    typedef boost::function<void (PackagePtr_t , EndpointPtr_t) > Handler_t;
};//end pkg_send_failed
struct udp_send_recv_exception
{
    const static size_t identifier=4;
    typedef ffnet::Endpoint endpoint;
    //TODO, what's the event_handler we need?
};//end udp_send_recv_exception

namespace more
{
struct tcp_server_start_listen
{
    const static size_t identifier=5;
    typedef boost::asio::ip::tcp::endpoint endpoint;
    typedef boost::function<void (endpoint) >     Handler_t;
};//end struct tcp_server_start_listen

struct tcp_server_accept_connection
{
    const static size_t identifier=6;
    typedef boost::shared_ptr<ffnet::TCPConnection> TCPConnectionPtr_t;
    typedef boost::function<void (TCPConnectionPtr_t) > Handler_t;
};//end struct tcp_server_accept_connection
struct tcp_server_accept_error
{
    const static size_t identifier=7;
    typedef ffnet::EndpointPtr_t EndpointPtr_t;
    typedef boost::system::error_code error_code;
    typedef boost::function<void (EndpointPtr_t, error_code)> Handler_t;
};//end tcp_server_accept_error
struct tcp_start_recv_stream {
    const static size_t identifier=8;
    typedef boost::asio::ip::tcp::endpoint endpoint;
    typedef boost::function<void (endpoint, endpoint)> Handler_t;
};//end tcp_start_recv_stream
struct tcp_start_send_stream {
    const static size_t identifier=9;
    typedef ffnet::TCPConnectionBase * TCPConnectionBasePtr;
    typedef boost::function<void (TCPConnectionBasePtr,  const char *, size_t)> Handler_t;
};//end tcp_start_send_stream
struct tcp_client_start_connection {
    const static size_t identifier=10;
    typedef boost::asio::ip::tcp::endpoint endpoint;
    typedef boost::function<void (endpoint) > Handler_t;
};//end tcp_client_start_connection
struct tcp_client_get_connection_succ {
    const static size_t identifier=11;
    typedef ::ffnet::TCPClient *TCPClientPtr_t;
    typedef boost::function<void (TCPClientPtr_t)> Handler_t;
};//end tcp_client_get_connection
struct tcp_client_conn_error {
    const static size_t identifier=12;
    typedef ::ffnet::TCPClient *TCPClientPtr_t;
    typedef boost::system::error_code error_code;
    typedef boost::function<void (TCPClientPtr_t, error_code)> Handler_t;
};//end tcp_client_conn_error
struct connect_sent_stream_succ {
    const static size_t identifier=13;
    typedef ::ffnet::ASIOConnection * ASIOConnPtr_t;
    typedef boost::function<void (ASIOConnPtr_t, size_t)> Handler_t;
};//end connect_sent_stream
struct connect_sent_stream_error {
    const static size_t identifier=14;
    typedef ::ffnet::ASIOConnection *ASIOConnPtr_t;
    typedef boost::system::error_code    error_code;
    typedef boost::function<void (ASIOConnPtr_t, error_code)> Handler_t;
};//end connect_sent_stream_error
struct connect_recv_stream_succ {
    const static size_t identifier=15;
    typedef ::ffnet::ASIOConnection * ASIOConnPtr_t;
    typedef boost::function<void (ASIOConnPtr_t, size_t)> Handler_t;
};//end connect_recv_stream_succ
struct connect_recv_stream_error {
    const static size_t identifier=16;
    typedef ::ffnet::ASIOConnection *ASIOConnPtr_t;
    typedef boost::function<void (ASIOConnPtr_t, boost::system::error_code)> Handler_t;
};//end connect_recv_stream_error
}//end namespace more
}
}
#endif // EVENTS_H

