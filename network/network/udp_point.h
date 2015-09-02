#pragma once
#include "common.h"
#include "network/asio_point.h"
#include "middleware/package.h"
#include "middleware/pkg_handler.h"
#include "network/net_buffer.h"
#include "network/end_point.h"


namespace ffnet {
    class udp_pkg_handler;

    class udp_point: public asio_point{
    public:
        udp_point(io_service & ioservice,
                  pkg_packer * bs,
                  event_handler * eh,
                  const std::vector<udp_pkg_handler *> & rph,
                  const udp_endpoint & ep);

        virtual ~udp_point();

        virtual void send(const package_ptr & pkg, const udp_endpoint & ep) = 0;

        virtual void close() = 0;

    protected:
        udp_endpoint mo_self_endpoint;
        std::vector<udp_pkg_handler *> m_pRPH;
    };

    class net_udp_point : public udp_point {
    public:
        net_udp_point(io_service &ioservice, pkg_packer *bs,
                 event_handler *eh, const std::vector<udp_pkg_handler *> & rph, const udp_endpoint &ep);

        virtual ~net_udp_point();

        virtual void send(const package_ptr &pkg, const udp_endpoint &endpoint);

        virtual void close();

        void ticktack();

    protected:
        void start_send();

        void handle_pkg_sent(const std::error_code &ec, std::size_t bytes_transferred);

        void actual_send_pkg(const package_ptr &pkg, const udp_endpoint &endpoint);

        void start_recv();

        void handle_received_pkg(const std::error_code &error, size_t bytes_transferred);

        void slice_and_dispatch_pkg(net_buffer *pBuf, const udp_endpoint &ep);

    protected:
        typedef std::function<void()> func_t;
        typedef std::queue<func_t> tasks_t;
        typedef std::map<udp_endpoint, net_buffer *> recv_buffer_t;

        asio::ip::udp::socket m_oSocket;
        udp_endpoint m_oRecvEndPoint;
        udp_endpoint m_oSendEndpoint;
        tasks_t m_oSendTasks;
        net_buffer m_oSendBuffer;
        net_buffer m_oTempBuffer;
        recv_buffer_t m_oRecvBuffer;
        std::map<uint32_t, udp_pkg_handler *> m_oRPHCache;
        bool m_bIsSending;
    };

    //end class UDPPoint
    typedef std::shared_ptr<udp_point> udp_point_ptr;
}//end namespace ffnet
