#include "network/tcp_server.h"
#include "network/endpoint_data.h"
#include "network/events.h"

namespace ffnet
{

using namespace ::ffnet::event;
using namespace ::ffnet::event::more;
TCPConnection::TCPConnection(io_service & ioservice, BonderSplitter *bs,
                             EventHandler * eh, RawPkgHandler * rph, TCPServer *pSvr)
    : TCPConnectionBase(ioservice, bs, eh, rph)
    , m_pTCPServer(pSvr)
{
    m_iConnectionState.store(s_valid);
}



void TCPConnection::start()
{
    m_oRemoteEndpoint = EndpointPtr_t(new Endpoint(m_oSocket.remote_endpoint()));
    startRecv();
}

TCPServer::TCPServer(io_service & ioservice, BonderSplitter *bs,
                     EventHandler * eh, RawPkgHandler * rph, boost::asio::ip::tcp::endpoint ep)
    : m_oAcceptor(ioservice, ep)
    , m_pAcceptEP()
    , m_pBS(bs)
    , m_pEH(eh)
    , m_pRPH(rph)
{
    m_pAcceptEP = EndpointPtr_t(new Endpoint(m_oAcceptor.local_endpoint()));
    startAccept();
}

void TCPServer::startAccept()
{
    TCPConnectionPtr_t pNewConn(new TCPConnection(m_oAcceptor.get_io_service(),
                                                  m_pBS, m_pEH, m_pRPH, this));

    //m_pHandler->onStartListening(this);
    m_pEH->triger<tcp_server_start_listen>(m_oAcceptor.local_endpoint());

    m_oAcceptor.async_accept(pNewConn->m_oSocket,
                             boost::bind(&TCPServer::handleAccept, this, pNewConn,
                                         boost::asio::placeholders::error)
                            );
}

void TCPServer::handleAccept(TCPConnectionPtr_t pNewConn, const boost::system::error_code &error)
{
    if(!error) {
        pNewConn->start();
        m_pEH->triger<tcp_server_accept_connection>(pNewConn);
        startAccept();
    } else {
        m_pEH->triger<tcp_server_accept_error>(m_pAcceptEP, error);
    }
}
void TCPServer::close()
{
    //m_oAcceptor.local_endpoint().shutdown();
    m_oAcceptor.cancel();
    m_oAcceptor.close();
}
}//end namespace ffnet
