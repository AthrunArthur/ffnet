#ifndef _NETWORK_HANDLER_CONN_HOLDER_HANDLER_H_
#define _NETWORK_HANDLER_CONN_HOLDER_HANDLER_H_
#include "handler/asio_conn_handler.h"
#include "network/tcp_connection_base.h"
#include <list>

namespace ffnet
{
	using ffnet::details::TCPConnectionBasePtr_t;
	
class ConnectionHolderHandler : public ASIOConnHandler
{
public:
	ConnectionHolderHandler();
    virtual ~ConnectionHolderHandler();

    virtual void		onRecvError(ASIOConnection *pConn, const boost::system::error_code &ec);
    virtual void		onSendError(ASIOConnection *pConn, const boost::system::error_code &ec);

    virtual void		onGotConnection(TCPConnectionBasePtr_t pConn);

    virtual void		onConnectionError(TCPConnectionBase *pConn, const boost::system::error_code &ec);
	
protected:
	typedef std::list<TCPConnectionBasePtr_t>	ConnHolder_t;
	ConnHolder_t		m_oConnHolder;
	boost::mutex		m_oMutex;
};//end class ConnectionHolderHandler

}//end namespace ffnet
#endif