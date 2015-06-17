#pragma once
#include "common.h"
#include "network/tcp_connection_base.h"
#include "network/end_point.h"

namespace ffnet {
    class tcp_server;

//////////////////////////////////////////////////////
    class net_tcp_connection : public net_tcp_connection_base {
    public:
        net_tcp_connection(io_service &ioservice, pkg_packer *bs,
                      event_handler *eh, const std::vector<tcp_pkg_handler *> & rph, tcp_server *pSvr);


        inline tcp_server * get_tcp_server() { return m_pTCPServer; }

    protected:
        friend class net_tcp_server;

        void start();

        tcp_server *m_pTCPServer;
    };

    //end class TCPConnection
    //typedef boost::shared_ptr<tcp_connection> tcp_connection_ptr;

//////////////////////////////////////////////////////
    class tcp_server : public boost::noncopyable {
    public:
        tcp_server(io_service &ioservice, pkg_packer *bs,
                  event_handler *eh, const std::vector<tcp_pkg_handler *> & rph, const tcp_endpoint & ep);


        inline boost::asio::io_service &ioservice() {
            return m_oIOService;
        }

        virtual void start_accept() = 0;

        virtual void close() = 0;

    protected:
        io_service & m_oIOService;
        tcp_endpoint m_oAcceptEP;
        pkg_packer *m_pBS;
        event_handler *m_pEH;
        std::vector<tcp_pkg_handler *> m_pRPH;
    };
    typedef boost::shared_ptr<tcp_server> tcp_server_ptr;

    class net_tcp_server : public tcp_server {
    public:
        net_tcp_server(io_service &ioservice, pkg_packer *bs,
                   event_handler *eh, const std::vector<tcp_pkg_handler *> & rph, const tcp_endpoint & ep);

        inline boost::asio::ip::tcp::acceptor &acceptor() {
            return m_oAcceptor;
        }

        virtual void start_accept();

        virtual void close();

    protected:

        void handle_accept(tcp_connection_base_ptr pNewConn, const boost::system::error_code &error);

    protected:
        boost::asio::ip::tcp::acceptor m_oAcceptor;
    };
}//end namespace ffnet

