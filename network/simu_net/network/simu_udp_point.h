#pragma once
#include "common.h"
#include "network/udp_point.h"
#include "network/tcp_client.h"
#include "simu_net/simu_pkgs.h"
#include "middleware/typed_pkg_hub.h"

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

      void    on_conn_succ(ffnet::tcp_connection_base *pConn);
      void    on_conn_error(tcp_connection_base *pConn, std::error_code ec);

      void    on_simu_udp_send_pkg(std::shared_ptr<simu_udp_send_pkg> pkg, tcp_connection_base * from);
    protected:
        udp_endpoint m_oRecvEndPoint;
        udp_endpoint m_oSendEndpoint;
        net_tcp_client * mp_conn;
        event_handler * mp_eh;
        typed_pkg_hub * mp_hub;
        pkg_packer * mp_packer;
  };//end class simu_udp_point
}
