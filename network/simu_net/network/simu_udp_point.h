#pragma once
#include "common.h"
#include "network/udp_point.h"
#include "network/tcp_client.h"

namespace ffnet{
  class simu_udp_point: public udp_point{
    public:
      simu_udp_point(io_service & ioservice, pkg_packer * bs,
        event_handler * eh, const std::vector<udp_pkg_handler *> & rph,
        const udp_endpoint & ep);

      virtual ~simu_udp_point();

      virtual void send(const package_ptr & pkg, const udp_endpoint & endpoint);
      virtual void close();
    protected:
        udp_endpoint m_oRecvEndPoint;
        udp_endpoint m_oSendEndpoint;
        net_tcp_client * mp_conn;
        event_handler * mp_eh;
        pkg_packer * mp_packer;
  };//end class simu_udp_point
}
