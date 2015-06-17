#pragma once
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
#include <queue>
#include <boost/cstdint.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/bind.hpp>
#include <boost/type_traits.hpp>

#include <stddef.h>

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
        real_net,
        simu_net,
        single_net,
    };

}//end namespace ffnet;

