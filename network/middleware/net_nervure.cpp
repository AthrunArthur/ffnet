#include "middleware/net_nervure.h"
#include "network/asio_point.h"
#include "network/udp_point.h"
#include "network/end_point.h"
#include "network/tcp_client.h"
#include "network/tcp_server.h"
#include "network/events.h"
#include "middleware/event_handler.h"
#include "common/defines.h"
#include "simu_net/network/simu_udp_point.h"


namespace ffnet {
    using namespace event;

    net_nervure::net_nervure(net_mode nm)
            : m_oIOService()
            , mi_mode(nm)
            , m_safe_to_stop(true){
        m_pEH = new event_handler();
        m_pBS = new length_packer();
        m_pEH->listen < event::more::tcp_server_accept_connection >
        (std::bind(&net_nervure::on_tcp_server_accept_connect, this, std::placeholders::_1));
        m_pEH->listen < event::more::tcp_client_get_connection_succ >
        (std::bind(&net_nervure::on_tcp_client_get_connect, this, std::placeholders::_1));
        m_pEH->listen < event::more::tcp_recv_stream_error >
        (std::bind(&net_nervure::on_conn_recv_or_send_error, this, std::placeholders::_1, std::placeholders::_2));
        m_pEH->listen < event::more::tcp_send_stream_error >
        (std::bind(&net_nervure::on_conn_recv_or_send_error, this, std::placeholders::_1, std::placeholders::_2));
    }

    void net_nervure::add_pkg_handler(tcp_pkg_handler * p_tcp_handler, udp_pkg_handler * p_udp_handler)
    {
        if(p_tcp_handler)
            m_pTCPHandler.push_back(p_tcp_handler);
        if(p_udp_handler)
            m_pUDPHandler.push_back(p_udp_handler);
    }

    net_nervure::~net_nervure() {
        if (m_pEH != NULL) {
            delete m_pEH;
            m_pEH = NULL;
        }
        if (m_pBS){
            delete m_pBS;
            m_pBS = NULL;
        }
    }

    tcp_connection_base *net_nervure::add_tcp_client(const std::string &ip, uint16_t iTCPPort) {
        tcp_endpoint ep(asio::ip::address_v4::from_string(ip), iTCPPort);
        return add_tcp_client(ep);
    }

    tcp_connection_base *net_nervure::add_tcp_client(const tcp_endpoint &ep) {
        tcp_connection_base * p = NULL;
        switch(mi_mode){
            case real_net:
                p = new net_tcp_client(m_oIOService, m_pBS, m_pEH, m_pTCPHandler, ep);
                break;
            default:
                assert(0 && "not impl");
                break;
        }
        tcp_connection_base_ptr pclient(p);
        m_oClients.push_back(pclient);
        return pclient.get();
    }

    tcp_server *net_nervure::add_tcp_server(const std::string &ip, uint16_t iTCPPort) {
        tcp_endpoint ep(asio::ip::address_v4::from_string(ip), iTCPPort);
        return add_tcp_server(ep);
    }

    tcp_server *net_nervure::add_tcp_server(const tcp_endpoint &ep) {
        tcp_server * p = NULL;
        switch (mi_mode){
            case real_net:
                p = new net_tcp_server(m_oIOService, m_pBS, m_pEH, m_pTCPHandler, ep);
                p->start_accept();
                break;
            default:
                assert(0 && "not impl");
                break;
        }
        tcp_server_ptr pserver(p);
        m_oServers.push_back(pserver);
        return pserver.get();
    }

    udp_point *net_nervure::add_udp_point(const std::string &ip, uint16_t iUDPPort) {
        udp_endpoint ep(asio::ip::address_v4::from_string(ip), iUDPPort);
        return add_udp_point(ep);
    }

    udp_point *net_nervure::add_udp_point(const udp_endpoint &ep) {
        udp_point * p = NULL;
        switch(mi_mode){
            case real_net:
                p = new net_udp_point(m_oIOService, m_pBS, m_pEH, m_pUDPHandler, ep);
                break;
            case simu_net:
                p = new simu_udp_point(m_oIOService, m_pBS, m_pEH, m_pUDPHandler, ep);
                break;
            default:
                assert(0 && "not impl");
                break;
        }
        udp_point_ptr ppoint(p);
        m_oUDPPoints.push_back(ppoint);
        return ppoint.get();
    }

    void net_nervure::on_conn_recv_or_send_error(tcp_connection_base *pConn, std::error_code ec) {
        //todo, should revise the connections.
        for (tcp_clients_t::iterator it = m_oClients.begin(); it != m_oClients.end(); ++it) {
            tcp_connection_base_ptr p = *it;
            if (p.get() == pConn) {
                pConn->close();
                m_oClients.erase(it);
                m_pEH->triger<tcp_lost_connection>(pConn);
                break;
            }
        }
        for (tcp_connections_t::iterator it = m_oTCPConnections.begin(); it != m_oTCPConnections.end(); ++it) {
            tcp_connection_base_ptr p = *it;
            if (p.get() == pConn) {
                pConn->close();
                m_oTCPConnections.erase(it);
                m_pEH->triger<tcp_lost_connection>(pConn);
                break;
            }
        }
    }

    void net_nervure::on_tcp_client_get_connect(tcp_connection_base *pClient) {
        m_pEH->triger<tcp_get_connection>(pClient);
    }

    void net_nervure::on_tcp_server_accept_connect(tcp_connection_base_ptr pConn) {
        m_oTCPConnections.push_back(pConn);
        m_pEH->triger<tcp_get_connection>(pConn.get());
    }

    void net_nervure::stop() {
      std::thread::id cid = std::this_thread::get_id();
      if(m_safe_to_stop) return ;

      if(cid == m_io_service_thrd){
        internal_stop();
      }
      else{
        std::unique_lock<std::mutex> _lock(m_stop_mutex);
        m_oIOService.post(std::bind(&net_nervure::internal_stop, this));
        while(!m_safe_to_stop){
          std::cout<<"waiting..."<<std::endl;
          m_stop_cond.wait(_lock);
        }
      }
    }

    void net_nervure::internal_stop(){
        m_oIOService.stop();
        for (tcp_servers_t::iterator it = m_oServers.begin(); it != m_oServers.end(); ++it) {
            tcp_server_ptr p = *it;
            p->close();
        }
        m_oServers.clear();
        for (tcp_connections_t::iterator it = m_oTCPConnections.begin(); it != m_oTCPConnections.end(); ++it) {
            tcp_connection_base_ptr p = *it;
            p->close();
        }
        m_oTCPConnections.clear();
        for (tcp_clients_t::iterator it = m_oClients.begin(); it != m_oClients.end(); ++it) {
            tcp_connection_base_ptr p = *it;
            p->close();
        }
        m_oClients.clear();
        std::unique_lock<std::mutex> _lock(m_stop_mutex);
        m_safe_to_stop = true;
        m_stop_cond.notify_one();
    }

    void net_nervure::run() {
        m_safe_to_stop= false;
        m_io_service_thrd = std::this_thread::get_id();
        m_oIOService.run();
    }


}//end namespace ffnet
