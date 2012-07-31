#ifndef _NETWORK_HANDLER_ASIO_CONN_HANDLER_H_
#define _NETWORK_HANDLER_ASIO_CONN_HANDLER_H_
#include "common.h"
#include "network/asio_connection.h"
#include "network/tcp_connection_base.h"

namespace ffnet
{
	using ffnet::details::ASIOConnection;
	using ffnet::details::TCPConnectionBase;
	using ffnet::details::TCPConnectionBasePtr_t;
	class ASIOConnHandler
	{
	public:
		virtual ~ASIOConnHandler(){}
		
		virtual void		onRecvError(ASIOConnection * pConn, const boost::system::error_code &ec) = 0;
		virtual void		onSendError(ASIOConnection * pConn, const boost::system::error_code &ec) = 0;
		
		virtual void		onGotConnection(TCPConnectionBasePtr_t pConn) = 0;
		
		virtual void		onConnectionError(TCPConnectionBase *pConn, const boost::system::error_code &ec) = 0;
	};//end class ASIOConnHandler
	typedef boost::shared_ptr<ASIOConnHandler> ASIOConnHandlerPtr_t;
}//end namespace ffnet

#endif