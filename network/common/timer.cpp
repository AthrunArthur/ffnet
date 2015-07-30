#include "common/timer.h"

namespace ffnet{
  deadline_timer * deadline_timer::sp_timer = NULL;

  deadline_timer::deadline_timer(){}
  deadline_timer::~deadline_timer(){
    mb_exit = true;
    if(m_thrd){
      m_thrd->join();
    }
  }

  void deadline_timer::start(){
    mb_exit = false;
    m_thrd = new std::thread([this](){
        while(!mb_exit){
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
begin:
        time_point n = std::chrono::steady_clock::now();
        mo_mutex.lock();
        for(std::list<tf_t>::iterator it = mo_funcs.begin(); it != mo_funcs.end(); ++it){
          time_point e = it->first;
          if(n <= e || std::chrono::duration_cast<std::chrono::milliseconds>(n - e).count() < 5){
          time_func_t f = it->second;
          f();
          mo_funcs.erase(it);
          mo_mutex.unlock();
          goto begin;
          }
        }
        mo_mutex.unlock();
        }
        });
  }

  void deadline_timer::register_func(const time_func_t f, int32_t milliseconds){
    if(sp_timer == NULL){
      sp_timer = new deadline_timer();
    }
    if(milliseconds == 0){
      f();
    }
    time_point n = std::chrono::steady_clock::now() + std::chrono::milliseconds(milliseconds);
    tf_t p(n, f);
    sp_timer->mo_mutex.lock();
    sp_timer->mo_funcs.push_back(p);
    sp_timer->mo_mutex.unlock();
  }

}
