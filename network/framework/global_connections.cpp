#include "framework/global_connections.h"
#include "handler/event.h"

namespace ffnet
{
namespace details
{
	using namespace ::ffnet::event;
	
boost::shared_ptr<GlobalConnections> GlobalConnections::s_pInstance;
GlobalConnections::GlobalConnections()
{
	Event<tcp_server_accept_connection>::listen(
		boost::bind(GlobalConnections::onTCPConnect, this, _1)
	);
	Event<tcp_client_get_connection_succ>::listen(
		boost::bind(GlobalConnections::onTCPClntConnect, this, _1)
	);
}

boost::shared_ptr< GlobalConnections> GlobalConnections::instance()
{
    if(s_pInstance == NULL) {
        s_pInstance = boost::shared_ptr<GlobalConnections>(new GlobalConnections());
    }
    return s_pInstance;
}
void GlobalConnections::addConnection(ASIOConnection *pConn)
{
	boost::unique_lock<boost::mutex> _l(m_oMutex);
	m_oConnections.push_back(pConn);
}

void GlobalConnections::delConnection(ASIOConnection *pConn)
{
   boost::unique_lock<boost::mutex> _l(m_oMutex);
    for(std::list<ASIOConnection *>::iterator it = m_oConnections.begin(); it != m_oConnections.end();
            ++it) {
        if(*it == pConn) {
            m_oConnections.erase(it);
            break;
        }
    }
}
//TODO, we need to compare the endpoint
ASIOConnection * GlobalConnections::findRemoteEndPoint(EndpointPtr_t pEndpoint)
{
	boost::unique_lock<boost::mutex> _l(m_oMutex);
    for(std::list<ASIOConnection *>::iterator it = m_oConnections.begin(); it != m_oConnections.end();
            ++it) {
        return *it;
    }
    return NULL;
}

void GlobalConnections::onTCPConnect(TCPConnectionPtr_t pConn)
{
	boost::unique_lock<boost::mutex> _l(m_oMutex);
	m_oConnHolder.push_back(pConn);
	m_oConnections.push_back(pConn.get());
}
void GlobalConnections::onTCPClntConnect(TCPClient* pClnt)
{
	boost::unique_lock<boost::mutex> _l(m_oMutex);
	m_oConnections.push_back(pClnt);
}
void GlobalConnections::onConnRecvOrSendError(ASIOConnection* pConn)
{
	boost::unique_lock<boost::mutex> _l(m_oMutex);
	//TODO, need to check it's tcp or udp
	//if it's tcp, then we need to check if it's in m_oConnHolder
	//if it's tcp, we also need to emit lost connection error
}

}//end namespace details
}//end namespace ffnet