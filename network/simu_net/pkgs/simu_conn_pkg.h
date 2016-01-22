#pragma once
#include "common.h"
#include "middleware/package.h"
#include "network/end_point.h"

namespace ffnet{
  template <int pkg_id>
    class simu_conn_pkg : public package{
      public:
        simu_conn_pkg(): package(pkg_id){}

        simu_endpoint_pkg(const tcp_endpoint & from, const tcp_endpoint & to)
          : package(pkg_id)
          , m_from_addr(from.address().to_string())
          , m_from_port(from.port())
          , m_to_addr(to.address().to_string())
          , m_to_port(to.port()){}

        virtual void archive(marshaler & ar){
          ar.archive(m_from_addr);
          ar.archive(m_from_port);
          ar.archive(m_to_addr);
          ar.archive(m_to_port);
        }

        tcp_endpoint from() const {
          tcp_endpoint ep(asio::ip::address::from_string(m_from_addr), m_from_port);
          return ep;
        }
        tcp_enpoint to() const{
          tcp_endpoint ep(asio::ip::address::from_string(m_to_addr), m_to_port);
          return ep;
        }
      protected:
        std::string m_from_addr;
        uint16_t m_from_port;
        std::string m_to_addr;
        uint16_t m_to_port;
    };//end class simu_conn_pkg

  typedef simu_conn_pkg<simu_tcp_conn_type> simu_tcp_conn_pkg;
  typedef simu_conn_pkg<simu_tcp_conn_ack_type> simu_tcp_conn_ack_pkg;
  typedef simu_conn_pkg<simu_tcp_conn_reject_type> simu_tcp_conn_reject_pkg;
  typedef simu_conn_pkg<simu_tcp_close_type> simu_tcp_close_pkg;
}//end namespace ffnet
