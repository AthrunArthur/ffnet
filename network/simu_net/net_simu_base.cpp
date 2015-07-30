#include "simu_net/net_simu_base.h"
#include "simu_net/simu_server.h"
#include <chrono>
#include "common/timer.h"

namespace ffnet{
  simu_net_base::simu_net_base(simu_server * svr)
    : mp_svr(svr){}

  simu_net_base::~simu_net_base(){}

  void simu_net_base::deliver_pkg(std::shared_ptr<simu_udp_send_pkg> &pkg, int32_t delay_milliseconds){
    if(delay_milliseconds == 0){
      mp_svr->forward_pkg(pkg);
      return ;
    }
    deadline_timer::register_func([this, pkg](){
        mp_svr->m_nn.ioservice().post([this, pkg](){
            mp_svr->forward_pkg(pkg);
          });
        }, delay_milliseconds);
  }

  void simu_net_base::forward_pkg(std::shared_ptr<simu_udp_send_pkg> pkg){}

  simu_net_default::simu_net_default(simu_server * svr)
    : simu_net_base(svr){}
  simu_net_default::~simu_net_default(){}

  void simu_net_default::forward_pkg(std::shared_ptr<simu_udp_send_pkg> pkg){
    deliver_pkg(pkg, 0);
  }
}
