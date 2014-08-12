#include "network/tcp_server.h"
#include "network/endpoint_data.h"
#include "framework/net_nervure.h"
#include "framework/event.h"

namespace ffnet
{

using namespace ::ffnet::event;
using namespace ::ffnet::event::more;
TCPConnection::TCPConnection(NetNervure *pNervure, TCPServer *pSvr)
    : TCPConnectionBase(pNervure)
    , m_pTCPServer(pSvr)
{
    m_iConnectionState.store(s_valid);
}



void TCPConnection::start()
{
    m_oRemoteEndpoint = EndpointPtr_t(new Endpoint(m_oSocket.remote_endpoint()));
    startRecv();
}

TCPServer::TCPServer(NetNervure *pNervure, const std::string & ip, uint16_t iPort)
    : m_oAcceptor(pNervure->getIOService(), tcp::endpoint(ip::address::from_string(ip.c_str()), iPort))
    , m_pNervure(pNervure)
    , m_pAcceptEP()
{
    m_pAcceptEP = EndpointPtr_t(new Endpoint(m_oAcceptor.local_endpoint()));
    startAccept();
}

void TCPServer::startAccept()
{
    TCPConnectionPtr_t pNewConn(new TCPConnection(m_pNervure, this));

    //m_pHandler->onStartListening(this);
    Event<tcp_server_start_listen>::triger(nervure(),boost::bind(
            tcp_server_start_listen::event, m_oAcceptor.local_endpoint(), _1
                                           ));

    m_oAcceptor.async_accept(pNewConn->m_oSocket,
                             boost::bind(&TCPServer::handleAccept, this, pNewConn,
                                         boost::asio::placeholders::error)
                            );
}

void TCPServer::handleAccept(TCPConnectionPtr_t pNewConn, const boost::system::error_code &error)
{
    if(!error) {
        pNewConn->start();
        Event<tcp_server_accept_connection>::triger(nervure(),
            boost::bind(tcp_server_accept_connection::event,
                        pNewConn, _1));
        startAccept();
    } else {
        Event<tcp_server_accept_error>::triger(nervure(),
            boost::bind(tcp_server_accept_error::event, m_pAcceptEP, error, _1)
        );
    }
}
void TCPServer::close()
{
    //m_oAcceptor.local_endpoint().shutdown();
    m_oAcceptor.cancel();
    m_oAcceptor.close();
}
}//end namespace ffnet
