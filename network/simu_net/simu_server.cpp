#include "simu_net/simu_server.h"
#include "simu_net/net_simu_base.h"
#include "common/defines.h"
#include "framework/flags.h"

namespace ffnet{
  void simu_server::init(simu_net_base * p_simu_net){
    CHECK(p_simu_net)<<" p_simu_net cannot be NULL";
    mp_simu_net = p_simu_net;
    m_pkghub.tcp_to_recv_pkg<simu_udp_open_pkg>(std::bind(&simu_server::on_recv_udp_open, this, std::placeholders::_1, std::placeholders::_2));
    m_pkghub.tcp_to_recv_pkg<simu_udp_close_pkg>(std::bind(&simu_server::on_recv_udp_close, this, std::placeholders::_1, std::placeholders::_2));
    m_pkghub.tcp_to_recv_pkg<simu_udp_send_pkg>(std::bind(&simu_server::on_recv_udp_send, this, std::placeholders::_1, std::placeholders::_2));
  }
  void simu_server::run(){
    m_nn.add_pkg_hub(m_pkghub);
    m_nn.add_tcp_server(FLAGS_simu_server_addr, FLAGS_simu_server_port);
    m_nn.get_event_handler()->listen<ffnet::event::tcp_lost_connection>(
        std::bind(&simu_server::on_lost_tcp_connection, this, std::placeholders::_1));
    m_nn.run();
  }

  void simu_server::on_recv_udp_open(std::shared_ptr<simu_udp_open_pkg> pkg, tcp_connection_base * from){
    udp_endpoint ep = pkg->endpoint();
    mo_udp_conns[ep] = from;
  }

  void simu_server::on_recv_udp_close(std::shared_ptr<simu_udp_close_pkg> pkg, tcp_connection_base * from){
    for(udp_2_simu_conn_t::iterator it = mo_udp_conns.begin(); it != mo_udp_conns.end(); ++it){
      tcp_connection_base * pconn = it->second;
      if(pconn == from){
        mo_udp_conns.erase(it);
        break;
      }
    }
  }
  void simu_server::on_recv_udp_send(std::shared_ptr<simu_udp_send_pkg> pkg, tcp_connection_base * from){
    mp_simu_net->forward_pkg(pkg);
  }
  void simu_server::on_lost_tcp_connection(tcp_connection_base * pconn){
    for(udp_2_simu_conn_t::iterator it = mo_udp_conns.begin(); it != mo_udp_conns.end(); ++it){
      tcp_connection_base * p = it->second;
      if(pconn == p){
        mo_udp_conns.erase(it);
        break;
      }
    }
  }
  void simu_server::forward_pkg(std::shared_ptr<simu_udp_send_pkg> pkg){
    udp_endpoint to_ep = pkg->to_endpoint();
    udp_2_simu_conn_t::iterator it = mo_udp_conns.find(to_ep);
    if(it == mo_udp_conns.end()){
      LOG(ERROR)<<"cannot find dest udp endpoint "<<to_ep.address().to_string()<<":"<<to_ep.port();
      return ;
    }
    tcp_connection_base * pconn = it->second;
    pconn->send(pkg);
  }
}
