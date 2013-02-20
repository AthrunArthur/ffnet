#ifndef _NETWORK_NETWORK_TCP_SERVER_H_
#define _NETWORK_NETWORK_TCP_SERVER_H_
#include "common.h"
#include "network/tcp_connection_base.h"
#include <boost/noncopyable.hpp>

namespace ffnet
{

class NetNervure;
class TCPServer;
using namespace boost::asio;
using namespace boost::asio::ip;
using boost::asio::ip::tcp;



//////////////////////////////////////////////////////
class TCPConnection : public TCPConnectionBase
{
public:
    TCPConnection(NetNervure *pNervure, TCPServer *pSvr);

    void 					start();
    inline TCPServer 	 	*	getTCPServer() { return m_pTCPServer;}

protected:
    friend class TCPServer;
    TCPServer 		*m_pTCPServer;
};//end class TCPConnection
typedef boost::shared_ptr<TCPConnection> TCPConnectionPtr_t;

//////////////////////////////////////////////////////
class TCPServer : public boost::noncopyable
{
public:
    TCPServer(NetNervure *pNervure, uint16_t iPort);

    inline tcp::acceptor &	getAcceptor() {
        return m_oAcceptor;
    }
    inline io_service 	&getIOService() {
        return m_oAcceptor.get_io_service();
    }
    
    void				close();
    inline NetNervure *		getNervure() const{return m_pNervure;}

protected:
    void				startAccept();

    void 			handleAccept(TCPConnectionPtr_t pNewConn, const boost::system::error_code &error);

protected:
	NetNervure *			m_pNervure;
    tcp::acceptor		m_oAcceptor;
};//end class TCPServer
typedef boost::shared_ptr<TCPServer> TCPServerPtr_t;
}//end namespace ffnet

#endif

