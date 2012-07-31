#include "network/asio_connection.h"
#include "framework/net_nervure.h"
#include "middleware/net_dispatcher.h"
#include "framework/global_connections.h"
#include "log.h"

namespace ffnet
{
namespace details
{
ASIOConnection::ASIOConnection(NetNervure *pNervure)
    : m_pNervure(pNervure)
    , m_oIOService(pNervure->getIOService())
    , m_pHandler(pNervure->getHandler().get())
    , m_pBonderSplitter(pNervure->getBonderSplitter().get())
    , m_oRecvBuffer()
    , m_oSendBuffer()
    , m_oMutex()
    , m_bIsSending(false)
{
	GlobalConnections::instance()->addConnection(this);
}

ASIOConnection::~ASIOConnection()
{
	GlobalConnections::instance()->delConnection(this);
}

void ASIOConnection::handlePkgSent(const boost::system::error_code &ec, std::size_t bytes_transferred)
{
    if(!ec) {
        m_oSendBuffer.eraseBuffer(bytes_transferred);
        startSend();
    } else {
        m_pHandler->onSendError(this, ec);
#ifdef ENABLE_LOG_CONSOLE
        log_connection("ASIOConnection", "handlePkgSent get error: %s", boost::system::system_error(ec).what());
#endif
    }
}
void ASIOConnection::handlReceivedPkg(const boost::system::error_code &error, size_t bytes_transferred)
{
    if(!error) {
#ifdef ENABLE_LOG_CONSOLE
        log_connection("ASIOConnection", "handleReceivedPkg() get pkg with len: %d", bytes_transferred);
#endif
        m_oRecvBuffer.filled() += bytes_transferred;
        sliceAndDispatchPkg();
        startRecv();
    } else	{
		m_pHandler->onRecvError(this, error);
        //call handler
#ifdef ENABLE_LOG_CONSOLE
        log_connection("ASIOConnection", "handleReceivedPkg() get error: %s", boost::system::system_error(error).what());
#endif
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
}//end namespace details
}//end namespace ffnet