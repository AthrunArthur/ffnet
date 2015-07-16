#include "framework/routine.h"

namespace ffnet{
  routine::routine(const std::string & name)
    : m_name(name){}

  void routine::initialize(net_mode nm, const std::vector<std::string> & args){}
}
