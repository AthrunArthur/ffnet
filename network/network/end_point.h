#pragma once
#include "common.h"


namespace ffnet
{
    typedef boost::asio::ip::tcp::endpoint tcp_endpoint;
    typedef boost::asio::ip::udp::endpoint udp_endpoint;
    typedef boost::shared_ptr<tcp_endpoint> tcp_endpoint_ptr;
    typedef boost::shared_ptr<udp_endpoint> udp_endpoint_ptr;
}
