#include "framework/net_nervure.h"
#include "network/tcp_server.h"
#include "network/tcp_client.h"
#include "network/udp_point.h"
#include "middleware/net_dispatcher.h"
#include "framework/global_connections.h"
#include "network/asio_connection.h"
#include "log.h"

namespace ffnet
{
using ffnet::details::GlobalConnections;
using ffnet::details::TCPClient;
using ffnet::details::UDPPoint;
using ffnet::details::UDPPointPtr_t;

NetNervure::NetNervure(ASIOConnHandlerPtr_t pNetHandler, BonderSplitterPtr_t pBonderSplitter)
: m_oIOService()
, m_oTasks()
, m_pTCPServer()
, m_oConnections()
, m_pConnHandler(pNetHandler)
, m_pBonderSplitter(pBonderSplitter)
, m_bIsStopped(false)
{
#ifdef ENABLE_LOG_CONSOLE
	log_frmwk("NetNervure", "NetNervure is initialized!");
#endif
}

NetNervure::~NetNervure()
{
	stop();
	m_oIOThread.join();
}

void NetNervure::send(boost::shared_ptr< Package > pPkg, EndpointPtr_t ep)
{
	ASIOConnection *tcb = GlobalConnections::instance()->findRemoteEndPoint(ep);
    if(tcb == NULL) {
        assert(0 && "You have to init such a remote endpoint!");
        std::cout << "You have to init such a remote endpoint connected to " << ep->address() << std::endl;
        return;
    }
    tcb->send(pPkg, ep);
}


void NetNervure::initTCPServer(uint16_t iTCPPort)
{
	if(m_pTCPServer == NULL)
	{
		m_pTCPServer = boost::shared_ptr<TCPServer>(new TCPServer(this, iTCPPort));
#ifdef ENABLE_LOG_CONSOLE
		log_frmwk("NetNervure", "initTCPServer(), tcp server is initialized on port:%d with Nervure:%d", iTCPPort, this);
#endif
	}
	else
	{
#ifdef ENABLE_LOG_CONSOLE
		log_frmwk("NetNervure", "initTCPServer(), error: tcp server has been initialized!");
#endif
	}
}
void NetNervure::initUDPServer(uint16_t iUDPPort)
{
	UDPPointPtr_t upp = UDPPointPtr_t(new UDPPoint(this, iUDPPort));
	m_oConnections.push_back(upp);
	
#ifdef ENABLE_LOG_CONSOLE
	log_frmwk("NetNervure", "initUDPServer(), udp server is initialized on port:%d", iUDPPort);
#endif
}

void NetNervure::addTCPClient(EndpointPtr_t remoteEndPoint)
{
	ASIOConnectionPtr_t acp = ASIOConnectionPtr_t(new TCPClient(this, *(remoteEndPoint.get())));
	m_oConnections.push_back(acp);
#ifdef ENABLE_LOG_CONSOLE
	log_frmwk("NetNervure", "addTCPClient(), tcp client has been initialized!");
#endif
}

void NetNervure::run()
{
	m_oIOThread = boost::thread(boost::bind(&io_service::run, &m_oIOService));
	
	Func_t f;
    while(!m_bIsStopped) {
        m_oTasks.pop(f);
        f();
    }
}

void NetNervure::deseralizeAndDispatchHandler(EndPointBufferPtr_t epb)
{

}

void NetNervure::stop()
{
	m_oTasks.push_back(boost::bind(&NetNervure::stopInThisThread, this));
}

void NetNervure::stopInThisThread()
{    //TODO(A.A.) we should stop it until all pkg are sent!.
    m_bIsStopped = true;
}


}