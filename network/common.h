#pragma once
#include <stddef.h>
#include <string>
#include <cstdio>
#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/any.hpp>
#include <boost/cstdint.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/bind.hpp>
#include <boost/type_traits.hpp>



namespace ffnet {
    typedef std::string String;
    typedef boost::int64_t int64_t;
    typedef boost::uint64_t uint64_t;
    typedef boost::int32_t int32_t;
    typedef boost::uint32_t uint32_t;
    typedef boost::int16_t int16_t;
    typedef boost::uint16_t uint16_t;
    typedef boost::int8_t int8_t;
    typedef boost::uint8_t uint8_t;

    enum net_mode{
        real_net = 1,
        simu_net,
        single_net,
    };

    enum {
      protobuf_wrapper_pkg_type = 1,
      retrans_pkg_type = 10,

      simu_udp_open,
      simu_udp_send_pkg,
      simu_udp_close,

      ffnet_internal_reserve,
    };

    const static std::string simu_server_addr("127.0.0.1");
    const static uint16_t simu_server_port = 8150;

}//end namespace ffnet;

