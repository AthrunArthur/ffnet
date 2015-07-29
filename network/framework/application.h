#pragma once

#include "common.h"
#include "framework/flags.h"

namespace ffnet{
  class routine;
  class application{
    public:
      application(const std::string & app_name);

      void initialize(int argc, char * argv[]);

      void register_routine(routine * rp);

      void run();

    protected:
      void  print_help();
      void  list_routines();
      void  run_routine();
      void  start_routine(routine * r);

      std::vector<routine *>  m_routines;
      std::function<void ()>  m_to_run_func;
      net_mode m_nm;
      std::vector<std::string> m_routine_name;
      std::vector<std::string> m_routine_args;
      std::string m_app_name;
  };//end class application
}
