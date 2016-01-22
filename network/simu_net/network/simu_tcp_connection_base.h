#pragma once
#include "common.h"
#include "network/tcp_client.h"
#include "simu_net/simu_pkgs.h"
#include "middleware/typed_pkg_hub.h"
#include "network/tcp_connection_base.h"

namespace ffnet{
  class simu_tcp_connection_base : public tcp_connection_base{
    public:
      virtual ~simu_tcp_connection_base();

      virtual void send(const package_ptr & pkg);

      virtual void close();

    protected:
      simu_tcp_connection_base(asio::io_service & ioservice, pkg_packer * bs,
          event_handler * eh, const std::vector<tcp_pkg_handler *> &  rph);

      void    on_conn_succ(ffnet::tcp_connection_base *pConn);
      void    on_conn_error(tcp_connection_base *pConn, std::error_code ec);

      void    on_simu_tcp_send_pkg(std::shared_ptr<simu_tcp_send_pkg> pkg, tcp_connection_base * from);

      void    on_simu_tcp_conn_ack_pkg(std::shared_ptr<simu_tcp_conn_ack_pkg> pkg, tcp_connection_base * from);
      void    on_simu_tcp_conn_reject_pkg(std::shared_ptr<simu_tcp_conn_reject_pkg> pkg, tcp_connection * from);


      void    on_simu_tcp_close_pkg(std::shared_ptr<simu_tcp_close_pkg> pkg, tcp_connection_base * from);

    protected:
      //friend class simu_tcp_server;
      //
      tcp_endpoint mo_from;
      tcp_endpoint mo_to;
      event_handler * mp_eh;
      typed_pkg_hub * mp_hub;
      pkg_packer * mp_packer;

  };//end class simu_tcp_conneciton_base

}//end namespace ffnet
