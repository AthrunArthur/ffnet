#pragma once
#include "common.h"


namespace ffnet
{
    typedef asio::ip::tcp::endpoint tcp_endpoint;
    typedef asio::ip::udp::endpoint udp_endpoint;
    typedef std::shared_ptr<tcp_endpoint> tcp_endpoint_ptr;
    typedef std::shared_ptr<udp_endpoint> udp_endpoint_ptr;
}
