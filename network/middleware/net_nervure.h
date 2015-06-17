#pragma once
#include "common.h"
#include "network/end_point.h"
#include "middleware/pkg_packer.h"
#include "middleware/event_handler.h"
#include "middleware/pkg_handler.h"
#include "network/tcp_connection_base.h"
#include "network/events.h"

namespace ffnet {
    class tcp_server;

    class udp_point;


    typedef boost::shared_ptr<tcp_server> tcp_server_ptr;
    typedef boost::shared_ptr<udp_point> udp_point_ptr;

    using boost::asio::io_service;
    using namespace event;

    class net_nervure {
    public:

        net_nervure(net_mode nm = real_net);

        void add_pkg_handler(tcp_pkg_handler * p_tcp_handler, udp_pkg_handler * p_udp_handler);

        template <class HT_>
        void add_pkg_hub(HT_ & ht){
            if(ht.get_tcp_pkg_handler() != NULL){
                m_pTCPHandler.push_back(ht.get_tcp_pkg_handler());
            }
            if(ht.get_udp_pkg_handler() != NULL){
                m_pUDPHandler.push_back(ht.get_udp_pkg_handler());
            }
        }

        virtual ~net_nervure();

        void run();

        void stop();

        tcp_server * add_tcp_server(const std::string &ip, uint16_t iTCPPort);

        tcp_server * add_tcp_server(const tcp_endpoint &ep);

        udp_point * add_udp_point(const std::string &ip, uint16_t iUDPPort);

        udp_point * add_udp_point(const udp_endpoint &ep);

        tcp_connection_base * add_tcp_client(const std::string &ip, uint16_t iTCPPort);

        tcp_connection_base * add_tcp_client(const tcp_endpoint &ep);


        inline io_service &ioservice() {
            return m_oIOService;
        }

        inline pkg_packer * get_pkg_packer() {
            return m_pBS;
        }

        inline event_handler *get_event_handler() {
            return m_pEH;
        }

        inline std::vector<tcp_pkg_handler *> get_tcp_pkg_handler() {
            return m_pTCPHandler;
        }

        inline std::vector<udp_pkg_handler *> get_udp_pkg_handler() { return m_pUDPHandler; }

    protected:
        void on_tcp_server_accept_connect(tcp_connection_base_ptr pConn);

        void on_tcp_client_get_connect(tcp_connection_base * pClient);

        void on_conn_recv_or_send_error(tcp_connection_base *pConn, boost::system::error_code ec);

    protected:
        pkg_packer *m_pBS;
        event_handler *m_pEH;
        std::vector<tcp_pkg_handler *> m_pTCPHandler;
        std::vector<udp_pkg_handler *> m_pUDPHandler;
        io_service m_oIOService;

        net_mode mi_mode;
        typedef std::vector<tcp_server_ptr> tcp_servers_t;
        typedef std::vector<tcp_connection_base_ptr> tcp_clients_t;
        typedef std::vector<udp_point_ptr> udp_points_t;
        typedef std::vector<tcp_connection_base_ptr> tcp_connections_t;

        tcp_servers_t m_oServers;
        tcp_clients_t m_oClients;
        udp_points_t m_oUDPPoints;
        tcp_connections_t m_oTCPConnections;
    };//end class NetNervure
}

