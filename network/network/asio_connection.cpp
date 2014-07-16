#include "network/asio_connection.h"
#include "framework/net_nervure.h"
#include "middleware/net_dispatcher.h"
#include "framework/global_connections.h"
#include "common/defines.h"
#include "middleware/net_dispatcher.h"
#ifdef PROTO_BUF_SUPPORT
#include "package/proto_buf_wrapper_pkg.h"
#endif

namespace ffnet
{
using namespace ::ffnet::event;
using namespace ::ffnet::event::more;
using ::ffnet::details::NetDispatcher;

ASIOConnection::ASIOConnection(NetNervure *pNervure)
    : m_pNervure(pNervure)
    , m_oIOService(pNervure->getIOService())
    , m_pBonderSplitter(pNervure->getBonderSplitter().get())
    , m_oRecvBuffer()
    , m_oSendBuffer()
    , m_oMutex()
    , m_bIsSending(false)
    , m_iConnectionState(s_init)
{
}

ASIOConnection::~ASIOConnection()
{
}

void ASIOConnection::handlePkgSent(const boost::system::error_code &ec, std::size_t bytes_transferred)
{
    if(!ec) {
        Event<connect_sent_stream_succ>::triger(nervure(),
                boost::bind(connect_sent_stream_succ::event, 
                this, bytes_transferred, _1));
        m_oSendBuffer.eraseBuffer(bytes_transferred);
        LOG_DEBUG(connection)<<"pkg sent "<< bytes_transferred<<" bytes, to "<<getRemoteEndpointPtr()->to_str();
        startSend();
    } else {
      m_iConnectionState.store(s_error);
        LOG_DEBUG(connection)<<"ASIOConnection::handlePkgSent(), Get error "<<ec.message();
        Event<connect_sent_stream_error>::triger(nervure(),
                boost::bind(connect_sent_stream_error::event,
                                this, ec, _1));
    }
}
void ASIOConnection::handlReceivedPkg(const boost::system::error_code &error, size_t bytes_transferred)
{
    if(!error) {
        Event<connect_recv_stream_succ>::triger(nervure(),
            boost::bind(connect_recv_stream_succ::event,
                        this, bytes_transferred, _1));
        LOG_DEBUG(connection)<<"recv pkg: "<<bytes_transferred<<" bytes, from "<< getRemoteEndpointPtr()->to_str();
        m_oRecvBuffer.filled() += bytes_transferred;
        sliceAndDispatchPkg();
        startRecv();
    } else    {
        m_iConnectionState.store(s_error);
        LOG_DEBUG(connection)<<"ASIOConnection::handlReceivedPkg(), Get error "<<error.message() <<" from "<< getRemoteEndpointPtr()->to_str();
        Event<connect_recv_stream_error>::triger(nervure(),
                        boost::bind(connect_recv_stream_error::event,
                                this, error, _1));
    }
}

void ASIOConnection::sliceAndDispatchPkg()
{
    std::list<SharedBuffer> sbs = m_pBonderSplitter->split(m_oRecvBuffer);
    for(std::list<SharedBuffer>::iterator it = sbs.begin();
            it != sbs.end(); ++it) {
        EndPointBufferPtr_t ebp(new EndPointWithData(getRemoteEndpointPtr(), *it));
        NetDispatcher::instance()->dispatch(boost::bind(&NetNervure::deseralizeAndDispatchHandler, m_pNervure, ebp));
    }
}
#ifdef PROTO_BUF_SUPPORT
void ASIOConnection::send(const boost::shared_ptr< google::protobuf::Message > & pMsg, const EndpointPtr_t & ep)
{
    boost::shared_ptr<Package> pPkg(new ::ffnet::ProtoBufWrapperPkg(pMsg));
    send(pPkg, ep);
}

#endif

}//end namespace ffnet
