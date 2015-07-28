#pragma once

#include "network/end_point.h"

namespace ffnet{

  class simu_server;

  class simu_net_base{
    public:
      simu_net_base(simu_server * svr);

      void send_pkg(boost::shared_ptr<simu_udp_send_pkg> & pkg);
      virtual void forward_pkg(boost::shared_ptr<simu_udp_send_pkg> & pkg);

    protected:
      simu_server * mp_svr;
  };//end class simu_net_base
}
