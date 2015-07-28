#include "simu_net/network/simu_udp_point.h"
#include "network/events.h"

namespace ffnet{

  simu_udp_point::simu_udp_point(io_service & ioservice, pkg_packer * bs,
      event_handler * eh, const std::vector<udp_pkg_handler * > & rph,
      const udp_endpoint & ep)
  : udp_point(ioservice, bs, eh, rph, ep){
    mp_eh = new event_handler();
    mp_packer = new length_packer();
  }
}
