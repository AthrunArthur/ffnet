#pragma once
#include "middleware/net_nervure.h"
#include "middleware/typed_pkg_hub.h"
#include "simu_net/simu_pkgs.h"

namespace ffnet{
  class simu_net_base;

  class simu_server{
    public:
      void init(simu_net_base * p_simu_net);
      void run();

    protected:
      void on_recv_udp_open(std::shared_ptr<simu_udp_open_pkg> pkg, tcp_connection_base * from);
      void on_recv_udp_close(std::shared_ptr<simu_udp_close_pkg> pkg, tcp_connection_base * from);
      void on_recv_udp_send(std::shared_ptr<simu_udp_send_pkg> pkg, tcp_connection_base * from);
      void on_lost_tcp_connection(tcp_connection_base * pconn);

      void forward_pkg(std::shared_ptr<simu_udp_send_pkg> pkg);


    protected:
      friend class simu_net_base;
      simu_net_base * mp_simu_net;
      typed_pkg_hub m_pkghub;
      net_nervure m_nn;
      typedef std::map<udp_endpoint, tcp_connection_base *> udp_2_simu_conn_t;
      typedef std::map<tcp_endpoint, tcp_connection_base *> tcp_2_simu_conn_t;
      udp_2_simu_conn_t mo_udp_conns;
  };
}
