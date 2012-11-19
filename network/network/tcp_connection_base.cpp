#include "network/tcp_connection_base.h"
#include "middleware/net_dispatcher.h"
#include "archive/archive.h"
#include "framework/global_connections.h"
#include "framework/net_nervure.h"
#include "log.h"

namespace ffnet
{
namespace details
{
EndpointPtr_t TCPConnectionBase::getRemoteEndpointPtr()
{
	return EndpointPtr_t(new Endpoint(m_oSocket.remote_endpoint()));
}

void TCPConnectionBase::startRecv()
{
	FFNET_DEBUG(	log_connection("TCPConnectionBase", "startRecv(), start receiving..." );)
	
    m_oSocket.async_read_some(boost::asio::buffer(m_oRecvBuffer.writeable()),
                              boost::bind(&TCPConnectionBase::handlReceivedPkg, shared_from_this(), boost::asio::placeholders::error(),
                                          boost::asio::placeholders::bytes_transferred()));
}
TCPConnectionBase::TCPConnectionBase(NetNervure *pNervure)
: ASIOConnection(pNervure)
, m_oSocket(pNervure->getIOService())
{
}
TCPConnectionBase::~TCPConnectionBase()
{
}

void TCPConnectionBase::send(PackagePtr_t pkg, EndpointPtr_t pEndpoint)
{
    m_oMutex.lock();
	m_pBonderSplitter->bond(m_oSendBuffer, pkg);

    if(!m_bIsSending) {
        m_bIsSending = true;
        m_oMutex.unlock();
        startSend();
    } else {
        m_oMutex.unlock();
    }
}
void TCPConnectionBase::close()
{
	m_oSocket.close();
}

void TCPConnectionBase::startSend()
{
    m_oMutex.lock();
    if(m_oSendBuffer.filled() != 0) {
	FFNET_DEBUG(
		log_connection("TCPConnectionBase", "startSend(), send buf:%s", 
					   printBuf(boost::asio::buffer_cast<const char *>( m_oSendBuffer.readable()),
								boost::asio::buffer_size(m_oSendBuffer.readable())).c_str());)
    
	m_oSocket.async_write_some(boost::asio::buffer(m_oSendBuffer.readable()),
                                   boost::bind(&TCPConnectionBase::handlePkgSent, shared_from_this(),
                                               boost::asio::placeholders::error,
                                               boost::asio::placeholders::bytes_transferred));
    } else {
        m_bIsSending = false;
    }
    m_oMutex.unlock();
}
}//end namespace details
}//end namespace ffnet