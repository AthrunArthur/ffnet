#include "framework/global_connections.h"

namespace ffnet
{
namespace details
{
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
ASIOConnection * GlobalConnections::findRemoteEndPoint(EndpointPtr_t pEndpoint)
{
	boost::unique_lock<boost::mutex> _l(m_oMutex);
    for(std::list<ASIOConnection *>::iterator it = m_oConnections.begin(); it != m_oConnections.end();
            ++it) {
        EndpointPtr_t epp = (*it)->getRemoteEndpointPtr();
        if(*(epp.get())==*(pEndpoint.get()))
	  return *it;
    }
    return NULL;
}

}//end namespace details
}//end namespace ffnet
