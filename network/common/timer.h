#pragma once

#include <list>
#include <functional>
#include <chrono>
#include <thread>
#include <mutex>

namespace ffnet{

  class deadline_timer
  {
    public:
      static void register_func(const std::function<void ()> f, int32_t milliseconds);
    protected:
      deadline_timer();
      ~deadline_timer();
      void start();
    protected:
      std::thread *m_thrd;
      typedef std::chrono::steady_clock::time_point time_point;
      typedef std::function<void ()> time_func_t;
      typedef std::pair<time_point, time_func_t > tf_t;
      std::list<tf_t> mo_funcs;
      std::mutex mo_mutex;
      std::atomic_bool mb_exit;

      static deadline_timer * sp_timer;
  };//end class timer
}
