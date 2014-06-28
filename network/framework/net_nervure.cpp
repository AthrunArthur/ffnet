#ifndef USING_MOCK_NET
#include "framework/net_nervure.h"
#include "framework/event.h"
#include "network/tcp_server.h"
#include "network/tcp_client.h"
#include "network/udp_point.h"
#include "middleware/net_dispatcher.h"
#include "framework/global_connections.h"
#include "network/asio_connection.h"
#include "common/defines.h"
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
    LOG_TRACE(frmwk)<<"NetNervure "<<"NetNervure is initialized!";

    Event<tcp_server_accept_connection>::listen(this,
            boost::bind(&GlobalConnections::onTCPConnect,
                        GlobalConnections::instance().get(), _1)
                                               );
    LOG_TRACE(frmwk)<<"NetNervure::ctor(), Enable logging tcp_server_accept_connection: "
					<<enable_hook_event<tcp_server_accept_connection>::value;
    Event<tcp_client_get_connection_succ>::listen(this,
            boost::bind(&GlobalConnections::onTCPClntConnect,
                        GlobalConnections::instance().get(), _1)
                                                 );
    LOG_TRACE(frmwk)<<"NetNervure::ctor(), Enable logging tcp_client_get_connection_succ: "
					<<enable_hook_event<tcp_client_get_connection_succ>::value;
					
    Event<connect_recv_stream_error>::listen(this,
            boost::bind(&GlobalConnections::onConnRecvOrSendError,
                        GlobalConnections::instance().get(), _1)
                                            );
    LOG_TRACE(frmwk)<<"NetNervure::ctor(), Enable logging connect_recv_stream_error: "
					<<enable_hook_event<connect_recv_stream_error>::value;
					
    Event<connect_sent_stream_error>::listen(this,
            boost::bind(&GlobalConnections::onConnRecvOrSendError,
                        GlobalConnections::instance().get(), _1)
                                            );
    LOG_TRACE(frmwk)<<"NetNervure::ctor(), Enable logging connect_sent_stream_error: "
					<<enable_hook_event<connect_sent_stream_error>::value;
					
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
        LOG_FATAL(app)<<"NetNervure::send "<<"You have to init such a remote endpoint connected to "<<ep->address().to_string()<<":"<<ep->port();
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

void NetNervure::initTCPServer(const std::string & ip, uint16_t iTCPPort)
{
    LOG_TRACE(frmwk)<<"NetNervure::initTCPServer() "<<"initializing tcp server at port "<<iTCPPort;
    if(m_pTCPServer == NULL)
    {
        m_pTCPServer = boost::shared_ptr<TCPServer>(new TCPServer(this,ip, iTCPPort));
        LOG_TRACE(frmwk)<<"NetNervure::initTCPServer() "<< "tcp server is initialized on port:"<<iTCPPort <<" with Nervure:"<<this;
    }
    else
    {
        LOG_ERROR(frmwk)<<"NetNervure::initTCPServer() "<< "error: tcp server has been initialized!";
    }
}
void NetNervure::initUDPServer(const std::string & ip, uint16_t iUDPPort)
{
    UDPPointPtr_t upp = UDPPointPtr_t(new UDPPoint(this, ip, iUDPPort));
    m_oConnections.push_back(upp);
    LOG_TRACE(frmwk)<<"NetNervure::initUDPServer() "<<"udp server is initialized on port:"<<iUDPPort;
}

void NetNervure::addTCPClient(EndpointPtr_t remoteEndPoint)
{
    ASIOConnectionPtr_t acp = ASIOConnectionPtr_t(new TCPClient(this, *(remoteEndPoint.get())));
    m_oConnections.push_back(acp);
    LOG_DEBUG(frmwk)<<"NetNervure::addTCPClient() "<<"tcp client has been initialized on port!";
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
#endif //end USING_MOCK_NET
