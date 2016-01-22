#pragma once

#include "network/end_point.h"
#include "simu_net/simu_pkgs.h"

namespace ffnet{

  class simu_server;

  class simu_net_base{
    public:
      simu_net_base(simu_server * svr);
      virtual ~simu_net_base();

      void deliver_pkg(std::shared_ptr<simu_udp_send_pkg> & pkg, int32_t delay_milliseconds = 0);
      virtual void forward_pkg(std::shared_ptr<simu_udp_send_pkg> pkg);

    protected:
      simu_server * mp_svr;
  };//end class simu_net_base

  class simu_net_default : public simu_net_base{
    public:
      simu_net_default(simu_server * svr);

      virtual ~simu_net_default();
      virtual void forward_pkg(std::shared_ptr<simu_udp_send_pkg> pkg);
  };//end class simu_net_default
}
