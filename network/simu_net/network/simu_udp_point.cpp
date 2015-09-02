#include "simu_net/network/simu_udp_point.h"
#include "network/events.h"
#include "common/defines.h"
#include "framework/flags.h"
#include "simu_net/simu_pkgs.h"

namespace ffnet{
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

  simu_udp_point::simu_udp_point(io_service & ioservice, pkg_packer * bs,
      event_handler * eh, const std::vector<udp_pkg_handler * > & rph,
      const udp_endpoint & ep)
  : udp_point(ioservice, bs, eh, rph, ep){
    mp_eh = new event_handler();
    mp_packer = new length_packer();
    mp_hub = new typed_pkg_hub();
    mp_eh->listen<event::more::tcp_client_get_connection_succ>(
        std::bind(&simu_udp_point::on_conn_succ, this, _1));
    mp_eh->listen<event::more::tcp_recv_stream_error>(
        std::bind(&simu_udp_point::on_conn_error, this, _1, _2));
    mp_eh->listen<event::more::tcp_send_stream_error>(
        std::bind(&simu_udp_point::on_conn_error, this, _1, _2));

    mp_hub->tcp_to_recv_pkg<simu_udp_send_pkg>(std::bind(&simu_udp_point::on_simu_udp_send_pkg, this, _1, _2));
    tcp_endpoint simu_ep(asio::ip::address_v4::from_string(FLAGS_simu_server_addr), FLAGS_simu_server_port);
    std::vector<tcp_pkg_handler *> ths{mp_hub->get_tcp_pkg_handler()};
    mp_conn = new net_tcp_client(m_oIOService, mp_packer, mp_eh, ths, simu_ep);
  }

  simu_udp_point::~simu_udp_point(){
    if(mp_eh){
      delete mp_eh;
      mp_eh = NULL;
    }
    if(mp_packer){
      delete mp_packer;
      mp_packer = NULL;
    }
    if(mp_conn){
      delete mp_conn;
      mp_conn = NULL;
    }
  }

  void simu_udp_point::send(const package_ptr & pkg, const udp_endpoint & endpoint){
    if(m_iPointState != state_valid){
      LOG(WARNING)<<"simu_conn is not established, ignore package to "<<endpoint.address().to_string();
      return ;
    }
    std::shared_ptr<simu_udp_send_pkg> p(new simu_udp_send_pkg(mo_self_endpoint, endpoint, pkg));
    mp_conn->send(p);
  }

  void simu_udp_point::close(){
    if(m_iPointState == state_valid){
      delete mp_conn;
      mp_conn = NULL;
    }
  }

  void simu_udp_point::on_conn_succ(tcp_connection_base * pConn){
    m_iPointState = state_valid;
    std::shared_ptr<simu_udp_open_pkg> p(new simu_udp_open_pkg(mo_self_endpoint));
    mp_conn->send(p);
  }

  void simu_udp_point::on_conn_error(tcp_connection_base *pConn, std::error_code ec){
    LOG(WARNING)<<"simu_conn get error "<<ec.message();
    close();
  }

  void simu_udp_point::on_simu_udp_send_pkg(std::shared_ptr<simu_udp_send_pkg> pkg, tcp_connection_base * ){
    udp_endpoint from = pkg->from_endpoint();
    shared_buffer sb(pkg->pkg_content(), pkg->pkg_content_len());

    for(size_t i = 0; i < m_pRPH.size(); ++i){
      udp_pkg_handler * p = m_pRPH[i];
      p->handle_pkg(this, sb, from);
    }
  }
}
