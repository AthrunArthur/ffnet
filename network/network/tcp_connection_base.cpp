#include "network/tcp_connection_base.h"
#include "common/archive.h"
#include "common/defines.h"
#include "network/events.h"

namespace ffnet
{
using namespace ::ffnet::event;
using namespace ::ffnet::event::more;
    
EndpointPtr_t TCPConnectionBase::getRemoteEndpointPtr()
{
    return m_oRemoteEndpoint;
}

void TCPConnectionBase::startRecv()
{
    try{
        m_pEH->triger<tcp_start_recv_stream>(m_oSocket.local_endpoint(),
                                         m_oSocket.remote_endpoint());
        LOG_DEBUG(connection)<<"TCPConnectionBase::startRecv() on " <<getRemoteEndpointPtr()->to_str();
    }catch(boost::system::system_error se)
    {
        LOG_DEBUG(connection)<<"TCPConnectionBase::startRecv(), remote_endpoint is disconnected!";
    }
    
    m_oSocket.async_read_some(boost::asio::buffer(m_oRecvBuffer.writeable()),
                              boost::bind(&TCPConnectionBase::handlReceivedPkg, shared_from_this(), boost::asio::placeholders::error(),
                                          boost::asio::placeholders::bytes_transferred()));
}
TCPConnectionBase::TCPConnectionBase(io_service & ioservice, BonderSplitter *bs,
                                     EventHandler *eh, RawPkgHandler * rph)
: ASIOConnection(ioservice, bs, eh, rph)
, m_oSocket(ioservice)
{
}
TCPConnectionBase::~TCPConnectionBase()
{
}


void TCPConnectionBase::send(const PackagePtr_t & pkg)
{
    if (m_iConnectionState.load() != s_valid)
    {
      m_pEH->triger<pkg_send_failed>(pkg, getRemoteEndpointPtr());
      return ;
    }

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

#ifdef PROTO_BUF_SUPPORT
void TCPConnectionBase::send(const boost::shared_ptr<google::protobuf::Message> & pMsg){
    boost::shared_ptr<Package> pPkg(new ::ffnet::ProtoBufWrapperPkg(pMsg));
    send(pPkg);
}
#endif

void TCPConnectionBase::close()
{
    m_oSocket.close();
    m_iConnectionState.store(s_closed);
}
bool TCPConnectionBase::isFree()
{
    if(m_oSendBuffer.filled() == 0)
    {
        return true;
    }
    return false;
}


void TCPConnectionBase::startSend()
{
    LOG_DEBUG(connection)<<"TCPConnectionBase  enter startSend";
    m_oMutex.lock();
    if(m_oSendBuffer.filled() != 0) {
        m_pEH->triger<tcp_start_send_stream>(this, boost::asio::buffer_cast<const char *>(m_oSendBuffer.readable()),
                                             boost::asio::buffer_size(m_oSendBuffer.readable()));

    
        m_oSocket.async_write_some(boost::asio::buffer(m_oSendBuffer.readable()),
             boost::bind(&TCPConnectionBase::handlePkgSent, shared_from_this(),
                 boost::asio::placeholders::error,
                 boost::asio::placeholders::bytes_transferred));
    } else {
        m_bIsSending = false;
    }
    m_oMutex.unlock();
    LOG_DEBUG(connection)<<"TCPConnectionBase exit startSend";
}

}//end namespace ffnet
