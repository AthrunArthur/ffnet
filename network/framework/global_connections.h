#ifndef _NETWORK_FRAMEWORK_GLOBAL_CONNECTIONS_H_
#define _NETWORK_FRAMEWORK_GLOBAL_CONNECTIONS_H_
#include "common.h"
#include "network/asio_connection.h"
#include "network/end_point.h"
#include "network/tcp_client.h"
#include "network/tcp_server.h"
#include "network/tcp_connection_base.h"

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
	//event
	void				onTCPConnect(TCPConnectionPtr_t pConn);
	void				onTCPClntConnect(TCPClient * pClnt);
	void				onConnRecvOrSendError(ASIOConnection *pConn);
protected:
    GlobalConnections();
	
protected:
	typedef std::list<TCPConnectionBasePtr_t>	ConnHolder_t;
	ConnHolder_t		m_oConnHolder;
	
	std::list<ASIOConnection *>			m_oConnections;
	boost::mutex						m_oMutex;
	static boost::shared_ptr< GlobalConnections> 	s_pInstance;
};//end class GlobalConnections
}//end namespace details
}//end namespace ffnet
#endif

