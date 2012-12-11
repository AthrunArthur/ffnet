#include "network/asio_connection.h"
#include "framework/net_nervure.h"
#include "middleware/net_dispatcher.h"
#include "framework/global_connections.h"
#include "handler/event.h"
#ifdef PROTO_BUF_SUPPORT
#include "package/proto_buf_wrapper_pkg.h"
#endif

namespace ffnet
{
namespace details
{
using namespace ::ffnet::event;
using namespace ::ffnet::event::more;

ASIOConnection::ASIOConnection(NetNervure *pNervure)
    : m_pNervure(pNervure)
    , m_oIOService(pNervure->getIOService())
    , m_pBonderSplitter(pNervure->getBonderSplitter().get())
    , m_oRecvBuffer()
    , m_oSendBuffer()
    , m_oMutex()
    , m_bIsSending(false)
{
	//We need this to initialize event register.
	GlobalConnections::instance();
}

ASIOConnection::~ASIOConnection()
{
}

void ASIOConnection::handlePkgSent(const boost::system::error_code &ec, std::size_t bytes_transferred)
{
    if(!ec) {
		Event<connect_sent_stream_succ>::triger(
			boost::bind(connect_sent_stream_succ::event, 
						this, bytes_transferred, _1)
		);
        m_oSendBuffer.eraseBuffer(bytes_transferred);
        startSend();
    } else {
		Event<connect_sent_stream_error>::triger(
			boost::bind(connect_sent_stream_error::event,
						this, ec, _1)
		);
    }
}
void ASIOConnection::handlReceivedPkg(const boost::system::error_code &error, size_t bytes_transferred)
{
    if(!error) {
		Event<connect_recv_stream_succ>::triger(
			boost::bind(connect_recv_stream_succ::event,
						this, bytes_transferred, _1)
		);
        m_oRecvBuffer.filled() += bytes_transferred;
        sliceAndDispatchPkg();
        startRecv();
    } else	{
        Event<connect_recv_stream_error>::triger(
			boost::bind(connect_recv_stream_error::event,
						this, error, _1)
		);
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
void ASIOConnection::send(boost::shared_ptr< google::protobuf::Message > pMsg, EndpointPtr_t ep)
{
    boost::shared_ptr<Package> pPkg(new ::ffnet::ProtoBufWrapperPkg(pMsg));
    send(pPkg, ep);
}

#endif

}//end namespace details
}//end namespace ffnet