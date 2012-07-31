#ifndef _NETWORK_FRAMEWORK_GLOBAL_CONNECTIONS_H_
#define _NETWORK_FRAMEWORK_GLOBAL_CONNECTIONS_H_
#include "common.h"
#include "network/asio_connection.h"
#include "network/end_point.h"
#include <boost/noncopyable.hpp>
#include <list>


namespace ffnet
{
namespace details
{
class GlobalConnections : public boost::noncopyable
{
public:
    static boost::shared_ptr< GlobalConnections> 		instance();

	void				addConnection(ASIOConnection *pConn);
	
	void				delConnection(ASIOConnection *pConn);
	
	ASIOConnection *		findRemoteEndPoint( EndpointPtr_t pEndpoint);
protected:
    GlobalConnections();
	
protected:
	std::list<ASIOConnection *>			m_oConnections;
	boost::mutex						m_oMutex;
	static boost::shared_ptr< GlobalConnections> 	s_pInstance;
};//end class GlobalConnections
}//end namespace details
}//end namespace ffnet
#endif

