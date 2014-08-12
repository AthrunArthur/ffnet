#ifndef _NETWORK_COMMON_H_
#define _NETWORK_COMMON_H_

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <string>
#include <cstdio>
#include <iostream>
#include <boost/cstdint.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/bind.hpp>

#include <stddef.h>

typedef std::string        String;
typedef boost::int64_t    int64_t;
typedef boost::uint64_t    uint64_t;
typedef boost::int32_t     int32_t;
typedef boost::uint32_t    uint32_t;
typedef boost::int16_t    int16_t;
typedef boost::uint16_t    uint16_t;
typedef boost::int8_t    int8_t;
typedef boost::uint8_t    uint8_t;
typedef boost::asio::ip::tcp::endpoint TCPEndPoint;
typedef boost::asio::ip::udp::endpoint UDPEndPoint;
typedef boost::shared_ptr<TCPEndPoint> TCPEndPointPtr_t;
typedef boost::shared_ptr<UDPEndPoint> UDPEndPointPtr_t;

#endif