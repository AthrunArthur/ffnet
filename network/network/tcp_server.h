#ifndef _NETWORK_NETWORK_TCP_SERVER_H_
#define _NETWORK_NETWORK_TCP_SERVER_H_
#include "common.h"
#include "network/tcp_connection_base.h"
#include "network/end_point.h"
#include <boost/noncopyable.hpp>

namespace ffnet
{
class TCPServer;
using namespace boost::asio;
using namespace boost::asio::ip;
using boost::asio::ip::tcp;



//////////////////////////////////////////////////////
class TCPConnection : public TCPConnectionBase
{
public:
    TCPConnection(io_service & ioservice, BonderSplitter *bs,
                  EventHandler * eh, RawPkgHandler * rph, TCPServer *pSvr);

    void                  start();
    inline TCPServer *    getTCPServer() { return m_pTCPServer;}

protected:
    friend class TCPServer;
    TCPServer         *m_pTCPServer;
};//end class TCPConnection
typedef boost::shared_ptr<TCPConnection> TCPConnectionPtr_t;

//////////////////////////////////////////////////////
class TCPServer : public boost::noncopyable
{
public:
    TCPServer(io_service & ioservice, BonderSplitter *bs,
              EventHandler * eh, RawPkgHandler * rph, ip::tcp::endpoint ep);

    inline tcp::acceptor &      getAcceptor() {
        return m_oAcceptor;
    }
    inline io_service &         getIOService() {
        return m_oAcceptor.get_io_service();
    }
    
    void                        close();

protected:
    void        startAccept();

    void        handleAccept(TCPConnectionPtr_t pNewConn, const boost::system::error_code &error);

protected:
    tcp::acceptor               m_oAcceptor;
    EndpointPtr_t               m_pAcceptEP;
    BonderSplitter *            m_pBS;
    EventHandler *              m_pEH;
    RawPkgHandler *             m_pRPH;
};//end class TCPServer
typedef boost::shared_ptr<TCPServer> TCPServerPtr_t;
}//end namespace ffnet

#endif

