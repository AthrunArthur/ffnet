#ifndef _NETWORK_FRAMEWORK_GLOBAL_CONNECTIONS_H_
#define _NETWORK_FRAMEWORK_GLOBAL_CONNECTIONS_H_
#include "common.h"
#include "network/asio_connection.h"
#include "network/end_point.h"
#include "network/tcp_client.h"
#include "network/tcp_server.h"
#include "network/tcp_connection_base.h"
#include "network/udp_point.h"
#include "package/package.h"
#include <boost/noncopyable.hpp>
#include <list>
#include <exception>


namespace ffnet
{
  using ffnet::PackagePtr_t;
  class FindConnectionException : public std::exception
  {
   public:
     FindConnectionException(EndpointPtr_t point)
       : m_pPoint(point){
      std::stringstream ss;
      ss<<"cannot find endpoint "<< m_pPoint->address().to_string() <<" in connected endpoints!"<<std::endl;
      m_strMsg = ss.str();
    }
     virtual ~FindConnectionException() throw(){}

    virtual const char* what() const throw()
    {
        return m_strMsg.c_str(); 
    }
   protected:
    EndpointPtr_t       m_pPoint;
    std::string         m_strMsg;
  };
namespace details
{
using ffnet::ASIOConnection;
using ffnet::UDPPoint;
	
class GlobalConnections : public boost::noncopyable
{
public:
    static boost::shared_ptr< GlobalConnections> 		instance();
    typedef boost::function<void (ASIOConnection * p)> FuncOnConn_t;

	void            addUDPPoint(UDPPoint * pPoint);
	void            delUDPPoint(UDPPoint *pPoint);
	
	void            findConnectionAndDo( EndpointPtr_t pEndpoint, FuncOnConn_t func);
	
	//event
	void				onTCPConnect(TCPConnectionPtr_t pConn);
	void				onTCPClntConnect(TCPClient * pClnt);
	void				onConnRecvOrSendError(ASIOConnection *pConn);
        

        //A helper function!
        static void  send(ASIOConnection * pConn, PackagePtr_t pkg, EndpointPtr_t ep);
protected:
    GlobalConnections();
	
protected:
    typedef std::list<TCPConnectionBasePtr_t>	ConnHolder_t;
    ConnHolder_t		m_oConnHolder;

    std::list<TCPClient *>			m_oTCPClients;
    std::list<UDPPoint *>			m_oUDPPoints;
    boost::mutex						m_oMutex;
    static boost::shared_ptr< GlobalConnections> 	s_pInstance;
};//end class GlobalConnections
}//end namespace details
}//end namespace ffnet
#endif

