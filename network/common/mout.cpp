#include "common/mout.h"

namespace ffnet{
  mout_stream::mout_internal_stream::mout_internal_stream(){
    m_ss = new std::stringstream;
  }
  mout_stream::mout_internal_stream::~mout_internal_stream(){
    s_out_mutex.lock();
    std::cout<<m_ss->str();
    s_out_mutex.unlock();
    if(m_ss){
      delete m_ss;
      m_ss = NULL;
    }
  }

  std::mutex mout_stream::mout_internal_stream::s_out_mutex;
  mout_stream mout;
}
