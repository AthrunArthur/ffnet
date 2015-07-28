#pragma once
#include "middleware/net_nervure.h"
#include "simu_net/simu_pkgs.h"

namespace ffnet{
  class simu_server{
    public:
      void init(){
        m_pkghub.tcp_to_recv_pkg<simu_udp_open_pkg>(boost::bind(&simu_server::on_recv_udp_open, this));
        m_pkghub.tcp_to_recv_pkg<simu_udp_close_pkg>(boost::bind(&simu_server::on_recv_udp_close, this));
        m_pkghub.tcp_to_recv_pkg<simu_udp_send_pkg>(boost::bind(&simu_server::on_recv_udp_send, this));
      }

      void run(){
        m_nn.add_pkg_hub(m_pkghub);
        m_nn.add_tcp_server(simu_server_addr, simu_server_port);
        m_nn.get_event_handler()->listen<ffnet::event::tcp_lost_connection>(
            boost::bind(&simu_server::on_lost_tcp_connection, this));
        m_nn.run();
      }

    protected:
      void on_recv_udp_open(boost::shared_ptr<simu_udp_open_pkg> pkg, ffnet::tcp_connection_base * from);
      void on_recv_udp_close(boost::shared_ptr<simu_udp_close_pkg> pkg, ffnet::tcp_connection_base * from);
      void on_recv_udp_send(boost::shared_ptr<simu_udp_send_pkg> pkg, ffnet::tcp_connection_base * from);


    protected:
      friend class simu_net_base;
      ffnet::typed_pkg_hub m_pkghub;
      ffnet::net_nervure m_nn;
  };
}
