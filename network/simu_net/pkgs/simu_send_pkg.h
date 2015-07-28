#pragma once
#include "common.h"
#include "middleware/package.h"

#include "network/end_point.h"

namespace ffnet{
  template<int pkg_id, class EP>
  class simu_send_pkg : public package{
    public:
      simu_send_pkg()
        : package(pkg_id)
        , m_pkg_content(NULL){}

      simu_send_pkg(const EP & self_ep, const EP & remote_ep, package_ptr pkg)
        : package(pkg_id)
        , m_from_addr(self_ep.address().to_string())
        , m_from_port(self_ep.port())
        , m_to_addr(remote_ep.address().to_string())
        , m_to_port(remote_ep.port())
        , m_pkg_content(NULL){
          marshaler lr(marshaler::length_retriver);
          pkg->arch(lr);
          m_pkg_len = lr.getLength();
          m_pkg_content = new char[m_pkg_len];
          marshaler sr(m_pkg_content, m_pkg_len, marshaler::seralizer);
          pkg->arch(sr);
        }

      virtual void archive(marshaler & ar){
        ar.archive(m_from_addr);
        ar.archive(m_from_port);
        ar.archive(m_to_addr);
        ar.archive(m_to_port);
        ar.archive(m_pkg_len);
        if(ar.is_deserializer()){
          m_pkg_content = new char [m_pkg_len];
        }
        ar.archive(m_pkg_content, m_pkg_len);
      }

      EP from_endpoint(){
        EP ep(boost::asio::ip::address::from_string(m_from_addr), m_from_port);
        return ep;
      }
      EP to_endpoint(){
        EP ep(boost::asio::ip::address::from_string(m_to_addr), m_to_port);
        return ep;
      }

    protected:
      std::string m_from_addr;
      uint16_t m_from_port;
      std::string m_to_addr;
      uint16_t m_to_port;
      int32_t m_pkg_len;
      char * m_pkg_content;
  };//end class simu_send_pkg

  typedef simu_send_pkg<simu_udp_send_pkg, udp_endpoint> simu_udp_send_pkg;
}
