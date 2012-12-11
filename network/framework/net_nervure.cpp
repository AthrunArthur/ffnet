#include "framework/net_nervure.h"
#include "handler/event.h"
#include "network/tcp_server.h"
#include "network/tcp_client.h"
#include "network/udp_point.h"
#include "middleware/net_dispatcher.h"
#include "framework/global_connections.h"
#include "network/asio_connection.h"
#include "log.h"
#ifdef PROTO_BUF_SUPPORT
#include "package/proto_buf_wrapper_pkg.h"
#endif

namespace ffnet
{
using ffnet::details::GlobalConnections;
using ffnet::TCPClient;
using ffnet::UDPPoint;
using ffnet::UDPPointPtr_t;
using namespace ::ffnet::event;
using namespace ::ffnet::event::more;

NetNervure::NetNervure(BonderSplitterPtr_t pBonderSplitter)
    : m_oIOService()
    , m_oTasks()
    , m_pTCPServer()
    , m_oConnections()
    , m_pBonderSplitter(pBonderSplitter)
    , m_bIsStopped(false)
{
#ifdef ENABLE_LOG_CONSOLE
    log_frmwk("NetNervure", "NetNervure is initialized!");
#endif
    Event<tcp_server_accept_connection>::listen(this,
            boost::bind(&GlobalConnections::onTCPConnect,
                        GlobalConnections::instance().get(), _1)
                                               );
    Event<tcp_client_get_connection_succ>::listen(this,
            boost::bind(&GlobalConnections::onTCPClntConnect,
                        GlobalConnections::instance().get(), _1)
                                                 );
    Event<connect_recv_stream_error>::listen(this,
            boost::bind(&GlobalConnections::onConnRecvOrSendError,
                        GlobalConnections::instance().get(), _1)
                                            );
    Event<connect_sent_stream_error>::listen(this,
            boost::bind(&GlobalConnections::onConnRecvOrSendError,
                        GlobalConnections::instance().get(), _1)
                                            );
    ffnet::details::NetDispatcher::instance();
}

NetNervure::~NetNervure()
{
    stop();
    m_oIOThread.join();
}

ASIOConnection * NetNervure::send(boost::shared_ptr< Package > pPkg, EndpointPtr_t ep)
{
    ASIOConnection *tcb = GlobalConnections::instance()->findRemoteEndPoint(ep);
    if(tcb == NULL) {
		std::cout << "You have to init such a remote endpoint connected to " << ep->address() << std::endl;
        assert(0 && "You have to init such a remote endpoint!");
        return tcb;
    }
    tcb->send(pPkg, ep);
	return tcb;
}

#ifdef PROTO_BUF_SUPPORT
ASIOConnection * NetNervure::send(boost::shared_ptr< google::protobuf::Message > pMsg, EndpointPtr_t ep)
{
    boost::shared_ptr<Package> pPkg(new ::ffnet::ProtoBufWrapperPkg(pMsg));
    return send(pPkg, ep);
}

#endif

void NetNervure::initTCPServer(uint16_t iTCPPort)
{
    if(m_pTCPServer == NULL)
    {
        m_pTCPServer = boost::shared_ptr<TCPServer>(new TCPServer(this, iTCPPort));
        FFNET_DEBUG(
            log_frmwk("NetNervure", "initTCPServer(), tcp server is initialized on port:%d with Nervure:%d", iTCPPort, this);
        )
    }
    else
    {
        FFNET_DEBUG(
            log_frmwk("NetNervure", "initTCPServer(), error: tcp server has been initialized!");
        )
    }
}
void NetNervure::initUDPServer(uint16_t iUDPPort)
{
    UDPPointPtr_t upp = UDPPointPtr_t(new UDPPoint(this, iUDPPort));
    m_oConnections.push_back(upp);

    FFNET_DEBUG(
        log_frmwk("NetNervure", "initUDPServer(), udp server is initialized on port:%d", iUDPPort);
    )
}

void NetNervure::addTCPClient(EndpointPtr_t remoteEndPoint)
{
    ASIOConnectionPtr_t acp = ASIOConnectionPtr_t(new TCPClient(this, *(remoteEndPoint.get())));
    m_oConnections.push_back(acp);
    FFNET_DEBUG(
        log_frmwk("NetNervure", "addTCPClient(), tcp client has been initialized on port!");
    )
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
    if(m_pTCPServer)
    {
        m_pTCPServer->close();
    }
    //still need to check each connections is free and close it.
    m_oTasks.push_back(boost::bind(&NetNervure::stopInThisThread, this));
}

void NetNervure::stopInThisThread()
{   //Stop it until all tasks are done!.
    if(m_oTasks.empty())
        m_bIsStopped = true;
    else
    {
        for(ConnContainer_t::iterator it = m_oConnections.begin();
                it != m_oConnections.end();
                ++it)
        {
            if((*it)->isFree())
            {
                (*it)->close();
                m_oConnections.erase(it);
            }
        }
        if(!m_oConnections.empty())
        {
            boost::this_thread::sleep(boost::posix_time::microseconds(100));

        }
        m_oTasks.push_back(boost::bind(&NetNervure::stopInThisThread, this));
    }
}


}