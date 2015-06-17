#include "network/tcp_client.h"
#include "network/events.h"
#include "common/defines.h"
#include "middleware/event_handler.h"

namespace ffnet {
    using namespace ::ffnet::event;
    using namespace ::ffnet::event::more;

    net_tcp_client::net_tcp_client(io_service &ioservice, pkg_packer * bs,
                         event_handler *eh, const std::vector<tcp_pkg_handler *> & rph, const tcp_endpoint &ep)
            : net_tcp_connection_base(ioservice, bs, eh, rph) {
        boost::system::error_code ec;
        m_pEH->triger<tcp_client_start_connection>(ep);
        m_oSocket.async_connect(ep, boost::bind(&net_tcp_client::handle_connected,
                                                 this, boost::asio::placeholders::error()));
    }

    void net_tcp_client::handle_connected(const boost::system::error_code &ec) {
        if (!ec) {
            LOG_TRACE(tcp_client) << "Get connection succ!";
            m_iPointState = state_valid;
            m_oRemoteEndpoint = m_oSocket.remote_endpoint();
            m_pEH->triger<tcp_client_get_connection_succ>(this);
            start_recv();
        } else {
            m_iPointState = state_error;
            LOG_DEBUG(tcp_client) << "Get connection error!";
            m_pEH->triger<tcp_client_conn_error>(this, ec);
        }
    }
}//end namespace ffnet
