#pragma once

#include "common.h"
#include "network/net_buffer.h"
#include "network/end_point.h"
#include "network/asio_point.h"
#include "middleware/pkg_handler.h"
#include "middleware/package.h"


namespace ffnet {

    class tcp_pkg_handler;

    class tcp_connection_base : public asio_point  {
    public:
        tcp_connection_base(boost::asio::io_service &ioservice, pkg_packer * bs,
                            event_handler *eh, const std::vector<tcp_pkg_handler *> & rph);

        virtual ~tcp_connection_base();

        virtual void send(const package_ptr &pkg) = 0;

        virtual void close() = 0;

        inline const tcp_endpoint &remote_endpoint() const { return m_oRemoteEndpoint; }

    protected:
        std::vector<tcp_pkg_handler *> m_pRPH;
        tcp_endpoint m_oRemoteEndpoint;
    };

    class net_tcp_connection_base : public tcp_connection_base,public boost::enable_shared_from_this<net_tcp_connection_base> {
    public:
        virtual ~net_tcp_connection_base();

        virtual void send(const package_ptr &pkg);

        virtual void close();


    protected:
        net_tcp_connection_base(boost::asio::io_service &ioservice, pkg_packer * bs,
                          event_handler *eh, const std::vector<tcp_pkg_handler *> & rph);

        void start_send();

        void start_recv();

        void handle_pkg_sent(const boost::system::error_code &ec, std::size_t bytes_transferred);

        void handle_received_pkg(const boost::system::error_code &error, size_t bytes_transferred);

        void slice_and_dispatch_pkg();

    protected:
        friend class net_tcp_server;

        typedef std::queue<package_ptr> pkgs_t;
        boost::asio::ip::tcp::socket m_oSocket;
        net_buffer m_oRecvBuffer;
        net_buffer m_oSendBuffer;
        pkgs_t m_oToSendPkgs;
        bool m_bIsSending;
        std::map<uint32_t, tcp_pkg_handler *> m_oRPHCache;
    };

    typedef boost::shared_ptr<tcp_connection_base> tcp_connection_base_ptr;

}//end namespace ffnet

