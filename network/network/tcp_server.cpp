#include "network/tcp_server.h"
#include "network/endpoint_data.h"
#include "framework/net_nervure.h"
#include "log.h"


namespace ffnet
{
namespace details
{
TCPConnection::TCPConnection(NetNervure *pNervure, TCPServer *pSvr)
: TCPConnectionBase(pNervure)
, m_pTCPServer(pSvr)
{
}



void TCPConnection::start()
{
	startRecv();
}

TCPServer::TCPServer(NetNervure *pNervure, uint16_t iPort)
: m_oAcceptor(pNervure->getIOService(), tcp::endpoint(tcp::v4(), iPort))
, m_pNervure(pNervure)
{
	startAccept();
}

void TCPServer::startAccept()
{
    TCPConnectionPtr_t pNewConn(new TCPConnection(m_pNervure, this));

    //m_pHandler->onStartListening(this);
	FFNET_DEBUG(
	log_tcp_server("TCPServer", "startAccept(), listening on %s:%d", ffnet::toString(m_oAcceptor.local_endpoint()).c_str(), m_oAcceptor.local_endpoint().port());
	)
    m_oAcceptor.async_accept(pNewConn->getSocket(),
                             boost::bind(&TCPServer::handleAccept, this, pNewConn,
                                         boost::asio::placeholders::error)
                            );
}

void TCPServer::handleAccept(TCPConnectionPtr_t pNewConn, const boost::system::error_code &error)
{
    if(!error) {
	FFNET_DEBUG(
		log_tcp_server("TCPServer", 
					   "handleAccept(), got a connection from %s:%d with pNervure:%d", 
					   ffnet::toString(pNewConn->getSocket().remote_endpoint()).c_str(), 
					   pNewConn->getSocket().remote_endpoint().port(), 
					   pNewConn->nervure());
	)
        //m_pHandler->onGotConnection(this, pNewConn);
        pNewConn->start();
    } else {
        //m_pHandler->onConnectionError(this, error);
    }
    startAccept();
}
}//end namespace details
}//end namespace ffnet
