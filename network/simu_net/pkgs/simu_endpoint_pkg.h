#pragma once
#include "common.h"
#include "middleware/package.h"
#include "network/end_point.h"


namespace ffnet{
  template<int pkg_id, class EP>
  class simu_endpoint_pkg: public package{
    public:
      simu_endpoint_pkg() : package(pkg_id){}

      simu_endpoint_pkg(const EP & self_ep)
        : package(pkg_id)
        , m_self_ep(self_ep.address().to_string())
        , m_self_port(self_ep.port()){}

      virtual void archive(marshaler & ar){
        ar.archive(m_self_ep);
        ar.archive(m_self_port);
      }

      EP endpoint(){
        EP ep(asio::ip::address::from_string(m_self_ep), m_self_port);
        return ep;
      }

    protected:
      std::string m_self_ep;
      uint16_t m_self_port;
  };//end class simu_endpoint_pkg

  typedef simu_endpoint_pkg<simu_udp_open_type, udp_endpoint> simu_udp_open_pkg;
  typedef simu_endpoint_pkg<simu_udp_close_type, udp_endpoint> simu_udp_close_pkg;
  typedef simu_endpoint_pkg<simu_tcp_listen_type, tcp_endpoint> simu_tcp_listen_pkg;
  typedef simu_endpoint_pkg<simu_tcp_listen_ack_type, tcp_endpoint> simu_tcp_listen_ack_pkg;
  typedef simu_endpoint_pkg<simu_tcp_listen_reject_type, tcp_endpoint> simu_tcp_listen_reject_pkg;
}
