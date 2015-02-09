#include "network/asio_connection.h"
#include "network/events.h"
#include "common/defines.h"
#include "middleware/raw_pkg_handler.h"
#ifdef PROTO_BUF_SUPPORT
#include "package/proto_buf_wrapper_pkg.h"
#endif

namespace ffnet
{
using namespace ::ffnet::event;
using namespace ::ffnet::event::more;

ASIOConnection::ASIOConnection(io_service & ioservice, BonderSplitter * bs, EventHandler * eh, RawPkgHandler * rph)
    : m_oIOService(ioservice)
    , m_pBonderSplitter(bs)
    , m_pEH(eh)
    , m_pRPH(rph)
    , m_oRecvBuffer()
    , m_oSendBuffer()
    , m_oMutex()
    , m_bIsSending(false)
    , m_iConnectionState(s_init)
{
}

ASIOConnection::~ASIOConnection()
{
    LOG_DEBUG(connection)<<"ASIOConnection::~ASIOConnection(), deconstructed!";
}

void ASIOConnection::handlePkgSent(const boost::system::error_code &ec, std::size_t bytes_transferred)
{
    if(!ec) {
        m_pEH->triger<connect_sent_stream_succ>(this, bytes_transferred);
        m_oSendBuffer.eraseBuffer(bytes_transferred);
        LOG_DEBUG(connection)<<"pkg sent "<< bytes_transferred<<" bytes, to ";//<<getRemoteEndpointPtr()->to_str();
        startSend();
    } else {
      m_iConnectionState.store(s_error);
        LOG_DEBUG(connection)<<"ASIOConnection::handlePkgSent(), Get error "<<ec.message();
        m_pEH->triger<connect_sent_stream_error>(this, ec);
    }
}
void ASIOConnection::handlReceivedPkg(const boost::system::error_code &error, size_t bytes_transferred)
{
    if(!error) {
        m_pEH->triger<connect_recv_stream_succ>(this, bytes_transferred);
        m_oRecvBuffer.filled() += bytes_transferred;
        LOG_DEBUG(connection)<<"recv pkg: "<<bytes_transferred<<" bytes, from " << getRemoteEndpointPtr()->to_str();
        sliceAndDispatchPkg();
        startRecv();
    } else    {
        m_iConnectionState.store(s_error);
        LOG_DEBUG(connection)<<"ASIOConnection::handlReceivedPkg(), Get error "<<error.message() <<" from " << getRemoteEndpointPtr()->to_str();
        m_pEH->triger<connect_recv_stream_error>(this, error);
    }
}

void ASIOConnection::sliceAndDispatchPkg()
{
    std::list<SharedBuffer> sbs = m_pBonderSplitter->split(m_oRecvBuffer);
    for(std::list<SharedBuffer>::iterator it = sbs.begin();
            it != sbs.end(); ++it) {
        m_pRPH->handle_raw_pkg(this, *it);
    }
}

/*
void ASIOConnection::send(const PackagePtr_t & pkg, const EndpointPtr_t & pEndpoint)
{
    assert(false && "This can only be called in UDP connection!");
}
void ASIOConnection::send(const PackagePtr_t & pkg)
{
    assert(false && "This can only be called in TCP connection!");
}
#ifdef PROTO_BUF_SUPPORT
void ASIOConnection::send(const boost::shared_ptr< google::protobuf::Message > & pMsg, const EndpointPtr_t & ep);
{
    assert(false && "This can only be called in UDP connection!");
}
void ASIOConnection::send(const boost::shared_ptr<google::protobuf::Message> & pMsg)
{
    assert(false && "This can only be called in TCP connection!");
}
#endif
*/

}//end namespace ffnet
