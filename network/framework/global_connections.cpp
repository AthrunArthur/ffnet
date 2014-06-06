#include "framework/global_connections.h"
#include "framework/event.h"
#include "common/defines.h"
namespace ffnet
{
namespace details
{
using namespace ::ffnet::event;
using namespace ::ffnet::event::more;

boost::shared_ptr<GlobalConnections> GlobalConnections::s_pInstance;
GlobalConnections::GlobalConnections()
{
	
}

boost::shared_ptr< GlobalConnections> GlobalConnections::instance()
{
    if(s_pInstance == NULL) {
        s_pInstance = boost::shared_ptr<GlobalConnections>(new GlobalConnections());
    }
    return s_pInstance;
}
void GlobalConnections::addUDPPoint(UDPPoint* pPoint)
{
    boost::unique_lock<boost::mutex> _l(m_oMutex);
    m_oUDPPoints.push_back(pPoint);
}
void GlobalConnections::delUDPPoint(UDPPoint* pPoint)
{
    boost::unique_lock<boost::mutex> _l(m_oMutex);
	
    RECHECK_DEL:
    for(std::list<UDPPoint *>::iterator it = m_oUDPPoints.begin();
        it != m_oUDPPoints.end();
        it ++)
    {
        if(pPoint == (*it))
        {
            m_oUDPPoints.erase(it);
            goto RECHECK_DEL;
        }
    }
}

ASIOConnection * GlobalConnections::findRemoteEndPoint(EndpointPtr_t pEndpoint)
{
    boost::unique_lock<boost::mutex> _l(m_oMutex);
    if(pEndpoint->is_udp())
    {
        for(std::list<UDPPoint *>::iterator it = m_oUDPPoints.begin();
                it != m_oUDPPoints.end();
                it ++)
        {
            Endpoint ep(*(*it)->getRemoteEndpointPtr());
            if(ep == *(pEndpoint.get()))
            {
                return *it;
            }
        }
    }
    else
    {
        for(ConnHolder_t::iterator it = m_oConnHolder.begin();
                it != m_oConnHolder.end();
                it ++)
        {
            Endpoint ep(*(*it)->getRemoteEndpointPtr());
            if(ep == *(pEndpoint.get()))
            {
                return it->get();
            }
        }

        for(std::list<TCPClient *>::iterator it = m_oTCPClients.begin();
                it != m_oTCPClients.end();
                it ++)
        {
            Endpoint ep(*(*it)->getRemoteEndpointPtr());
            if(ep == *(pEndpoint.get()))
            {
                return *it;
            }
        }
    }
    return NULL;
}

void GlobalConnections::onTCPConnect(TCPConnectionPtr_t pConn)
{
    m_oMutex.lock();
    m_oConnHolder.push_back(pConn);
    m_oMutex.unlock();
    LOG_TRACE(frmwk)<<"Get a TCP Connection!";
    Event<tcp_get_connection>::triger(
        boost::bind(tcp_get_connection::event,pConn.get(), _1)
    );

}
void GlobalConnections::onTCPClntConnect(TCPClient* pClnt)
{
    m_oMutex.lock();
    m_oTCPClients.push_back(pClnt);
    m_oMutex.unlock();
    Event<tcp_get_connection>::triger(
        boost::bind(tcp_get_connection::event, pClnt, _1)
    );
}
void GlobalConnections::onConnRecvOrSendError(ASIOConnection* pConn)
{
    LOG_TRACE(connection)<<"GlobalConnections::onConnRecvOrSendError() enter";
    boost::unique_lock<boost::mutex> _l(m_oMutex);
    if(pConn->UDPPointPointer() != NULL)
    {
        //TODO, emit udp error event.
        return ;
    }
    if(pConn->TCPConnectionBasePointer() == NULL)
        return ;
    TCPConnectionBase * p = dynamic_cast<TCPConnectionBase *>(pConn);

    if(dynamic_cast<TCPClient *>(p) == NULL)
    {
        //if it's tcp, then we need to check if it's in m_oConnHolder
RECHECK:
        for(ConnHolder_t::iterator it = m_oConnHolder.begin();
                it != m_oConnHolder.end();
                ++it) {
            if(it->get() == pConn) {
                pConn->close();
				LOG_TRACE(connection)<<"GlobalConnections::onConnRecvOrSendError() find an existed conn and now loose it, triger tcp_lost_connection";
                Event<tcp_lost_connection>::triger(
                    boost::bind(tcp_lost_connection::event, p, _1)
                );
                m_oConnHolder.erase(it);
                goto RECHECK;
            }
        }
    }
    else
    {
        //if it's tcp, we also need to emit lost connection error
RECHECK_CLIENT:
        for(std::list<TCPClient *>::iterator it = m_oTCPClients.begin();
                it != m_oTCPClients.end();
                it ++)
        {
            if((*it) == p)
            {
                m_oTCPClients.erase(it);
				LOG_TRACE(connection)<<"GlobalConnections::onConnRecvOrSendError(), del tcp client and triger tcp_lost_connection";
                Event<tcp_lost_connection>::triger(
                    boost::bind(tcp_lost_connection::event, p, _1)
                );
                pConn->close();
                goto RECHECK_CLIENT;
            }
        }
    }
}

}//end namespace details
}//end namespace ffnet
