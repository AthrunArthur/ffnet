#ifndef _NETWORK_FRAMEWORK_NET_NERVURE_H_
#define _NETWORK_FRAMEWORK_NET_NERVURE_H_
#include "common.h"
#include "common/blocking_queue.h"
#include "network/endpoint_data.h"
#include "network/end_point.h"
#include "middleware/bonder_splitter.h"
#include "middleware/event_handler.h"
#include "middleware/raw_pkg_handler.h"
#include <map>
#ifdef PROTO_BUF_SUPPORT
#include <google/protobuf/message.h>
#endif

namespace ffnet
{
class TCPServer;
class TCPClient;
class UDPPoint;
class TCPConnection;
class ASIOConnection;
typedef boost::shared_ptr<TCPServer> TCPServer_ptr;
typedef boost::shared_ptr<TCPClient> TCPClient_ptr;
typedef boost::shared_ptr<UDPPoint> UDPPoint_ptr;
typedef boost::shared_ptr<TCPConnection> TCPConnection_ptr;

typedef ASIOConnection *ASIOConnPtr_t;

typedef boost::function<PackagePtr_t ()> PkgCreator_t;

class NetNervure
{
public:
    
    NetNervure(RawPkgHandler *rph);
    virtual ~NetNervure();
    
    void                    run();

    void                    stop();

    boost::shared_ptr<TCPServer>       addTCPServer(const std::string & ip, uint16_t iTCPPort);
    boost::shared_ptr<TCPServer>       addTCPServer(boost::asio::ip::tcp::endpoint ep);
    
    boost::shared_ptr<UDPPoint>        addUDPPoint(const std::string & ip, uint16_t iUDPPort);
    boost::shared_ptr<UDPPoint>        addUDPPoint(boost::asio::ip::udp::endpoint ep);
    
    boost::shared_ptr<TCPClient>       addTCPClient(const std::string & ip, uint16_t iTCPPort);
    boost::shared_ptr<TCPClient>       addTCPClient(boost::asio::ip::tcp::endpoint ep);
    
    
    inline io_service &    getIOService() {
        return m_oIOService;
    }
    inline BonderSplitter *     getBonderSplitter() {
        return m_pBS;
    }
    inline EventHandler *   getEventHandler() {
        return m_pEH;
    }
    inline RawPkgHandler *  getRawPkgHandler() {
        return m_pRPH;
    }
    
protected:
    void        onTCPServerAcceptConnect(TCPConnection_ptr pConn);
    void        onTCPClientGetConnect(TCPClient * pClient);
    void        onConnRecvOrSendError(ASIOConnPtr_t pConn, boost::system::error_code ec);
    
protected:
    BonderSplitter *          m_pBS;
    EventHandler *            m_pEH;
    RawPkgHandler *           m_pRPH;
    io_service                m_oIOService;
    typedef std::vector<TCPServer_ptr>  TCPServers_t;
    typedef std::vector<TCPClient_ptr>  TCPClients_t;
    typedef std::vector<UDPPoint_ptr>  UDPPoints_t;
    typedef std::vector<TCPConnection_ptr> TCPConnections_t;
    
    TCPServers_t        m_oServers;
    TCPClients_t        m_oClients;
    UDPPoints_t         m_oUDPPoints;
    TCPConnections_t    m_oTCPConnections;
    

};//end class NetNervure
}

#endif