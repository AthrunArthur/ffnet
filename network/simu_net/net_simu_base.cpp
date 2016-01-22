#include "simu_net/net_simu_base.h"
#include "simu_net/simu_server.h"
#include <chrono>

namespace ffnet{
  simu_net_base::simu_net_base(simu_server * svr)
    : mp_svr(svr){}

  simu_net_base::~simu_net_base(){}

  void simu_net_base::deliver_pkg(std::shared_ptr<simu_udp_send_pkg> &pkg, int32_t delay_milliseconds){
    if(delay_milliseconds == 0){
      mp_svr->forward_pkg(pkg);
      return ;
    }
    asio::deadline_timer dt(mp_svr->m_nn.ioservice());
    dt.expires_from_now(boost::posix_time::milliseconds(delay_milliseconds));
    dt.async_wait([this, pkg](const asio::error_code & error){
      mp_svr->forward_pkg(pkg);
        });
  }

  void simu_net_base::forward_pkg(std::shared_ptr<simu_udp_send_pkg> pkg){}

  simu_net_default::simu_net_default(simu_server * svr)
    : simu_net_base(svr){}
  simu_net_default::~simu_net_default(){}

  void simu_net_default::forward_pkg(std::shared_ptr<simu_udp_send_pkg> pkg){
    deliver_pkg(pkg, 0);
  }
}
