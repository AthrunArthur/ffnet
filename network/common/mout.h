#pragma once

#include "common.h"
#include <boost/thread/mutex.hpp>
#include <sstream>

namespace ffnet{
  class mout_stream{
    public:
      class mout_internal_stream{
        public:
          typedef std::basic_ostream<char, std::char_traits<char> > ostream_t;

          mout_internal_stream();

          ~mout_internal_stream();
          template<class T>
            mout_internal_stream & operator <<(
                const T & t){
              (*m_ss)<<t;
              return *this;
            }
          inline mout_internal_stream & operator<<(ostream_t & ( * pfn)(ostream_t &)){
            (*m_ss)<<pfn;
            return *this;
          }
        protected:
          std::stringstream * m_ss;
          static boost::mutex s_out_mutex;
      };//end class mout_internal_stream
      template <class T>
        mout_internal_stream operator << (const T & t){
          mout_internal_stream tm;
          tm<<t;
          return tm;
        }
  };
  extern mout_stream mout;
}
