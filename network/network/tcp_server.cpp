#include "network/tcp_server.h"
#include "network/events.h"
#include "middleware/event_handler.h"

namespace ffnet {

    using namespace ::ffnet::event;
    using namespace ::ffnet::event::more;

    net_tcp_connection::net_tcp_connection(io_service &ioservice, pkg_packer *bs,
                                 event_handler *eh, const std::vector<tcp_pkg_handler *> & rph, tcp_server *pSvr)
            : net_tcp_connection_base(ioservice, bs, eh, rph), m_pTCPServer(pSvr) {
        m_iPointState = state_valid;
    }

    void net_tcp_connection::start() {
        m_oRemoteEndpoint = m_oSocket.remote_endpoint();
        start_recv();
    }


    /////////
    tcp_server::tcp_server(io_service &ioservice, pkg_packer *bs,
                           event_handler *eh, const std::vector<tcp_pkg_handler *> & rph, const tcp_endpoint & ep)
            : m_oAcceptEP(ep), m_pBS(bs), m_pEH(eh), m_pRPH(rph), m_oIOService(ioservice) {}

    net_tcp_server::net_tcp_server(io_service &ioservice, pkg_packer *bs,
                         event_handler *eh, const std::vector<tcp_pkg_handler *> & rph, const tcp_endpoint & ep)
            : tcp_server(ioservice, bs, eh, rph, ep)
            , m_oAcceptor(ioservice, ep) {}

    void net_tcp_server::start_accept() {
        net_tcp_connection * ntc = new net_tcp_connection(m_oAcceptor.get_io_service(), m_pBS, m_pEH, m_pRPH, this);
        tcp_connection_base_ptr pNewConn(ntc);

        m_pEH->triger<tcp_server_start_listen>(m_oAcceptor.local_endpoint());

        m_oAcceptor.async_accept(ntc->m_oSocket,
                                 boost::bind(&net_tcp_server::handle_accept, this, pNewConn,
                                             boost::asio::placeholders::error)
        );
    }

    void net_tcp_server::handle_accept(tcp_connection_base_ptr pNewConn, const boost::system::error_code &error) {
        if (!error) {
            net_tcp_connection * ntc = (net_tcp_connection *)pNewConn.get();
            ntc->start();
            m_pEH->triger<tcp_server_accept_connection>(pNewConn);
            start_accept();
        } else {
            m_pEH->triger<tcp_server_accept_error>(m_oAcceptEP, error);
        }
    }

    void net_tcp_server::close() {
        //m_oAcceptor.cancel();
        m_oAcceptor.close();
    }
}//end namespace ffnet
