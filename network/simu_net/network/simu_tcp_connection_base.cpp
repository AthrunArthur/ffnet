#include "simu_net/network/simu_tcp_connection_base.h"
#include "network/events.h"
#include "common/defines.h"
#include "framework/flags.h"

namespace ffnet{
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

  simu_tcp_connection_base(asio::io_service & ioservice, pkg_packer * bs,
      event_handler * eh, const std::vector<tcp_pkg_handler *> & rph)
  : tcp_connection_base(ioservice, bs, eh, rph){

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
    /*
    tcp_endpoint simu_ep(asio::ip::address_v4::from_string(FLAGS_simu_server_addr), FLAGS_simu_server_port);
    std::vector<tcp_pkg_handler *> ths{mp_hub->get_tcp_pkg_handler()};
    mp_conn = new net_tcp_client(m_oIOService, mp_packer, mp_eh, ths, simu_ep);
    */
  }

  simu_tcp_connection_base::~simu_tcp_connection_base(){
    if(mp_eh){
      delete mp_eh;
      mp_eh = NULL;
    }
    if(mp_hub){
      delete mp_hub;
      mp_hub = NULL;
    }
    if(mp_packer){
      delete mp_packer;
      mp_packer = NULL;
    }
  }

  simu_tcp_connection_base::send(const package_ptr & pkg){
    if(m_iPointState != state_valid){
      LOG(ERROR)<<"simu_conn is not established";
      return ;
    }
    std::shared_ptr<simu_tcp_send_pkg> p(new simu_udp_send_pkg(mo_self_endpoint, endpoint, pkg));
    mp_conn->send(p);
    simu_tcp
  }
}
