#pragma once
#include "common.h"

namespace ffnet{
  class routine{
    public:
      routine(const std::string & name);

      virtual void initialize(const std::string & args);

      virtual void run(net_mode nm) = 0;

      inline std::string get_name() const{
        return m_name;
      }

    protected:
      std::string m_name;
  };//end class routine
}
