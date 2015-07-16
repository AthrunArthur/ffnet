#pragma once

#include "common.h"
#include <boost/program_options.hpp>

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
      void  run_rountine();

      std::vector<routine *>  m_routines;
      boost::function<void ()>  m_to_run_func;
      net_mode m_nm;
      std::string m_routine_name;
      std::vector<std::string> m_routine_args;
      std::string m_app_name;
      std::string m_usage;

      boost::program_options::options_description m_desc;
  };//end class application
}
