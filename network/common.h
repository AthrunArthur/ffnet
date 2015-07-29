#pragma once
#include <stddef.h>
#include <string>
#include <cstdio>
#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <asio.hpp>
#include <memory>
#include <functional>
#include <cstdint>
#include <thread>
#include <type_traits>
//#include <boost/format.hpp>
#include <boost/any.hpp>
#include <boost/noncopyable.hpp>



namespace ffnet {
  typedef std::string String;
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

}//end namespace ffnet;

