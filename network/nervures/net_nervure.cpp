#include "nervures/net_nervure.h"
#include "network/asio_connection.h"
#include "network/udp_point.h"
#include "network/end_point.h"
#include "network/tcp_client.h"
#include "network/tcp_server.h"
#include "middleware/length_bonder_splitter.h"
#include "network/events.h"


namespace ffnet
{
    using namespace event;
    using namespace event::more;
    
NetNervure::NetNervure(RawPkgHandler* rph)
: m_oIOService()
, m_pBS()
, m_pEH()
, m_pRPH(rph)
{
    m_pBS = new LengthBonderSplitter();
    m_pEH = new EventHandler();
    m_pEH->listen<tcp_server_accept_connection>(boost::bind(&NetNervure::onTCPServerAcceptConnect, this, _1));
    m_pEH->listen<tcp_client_get_connection_succ>(boost::bind(&NetNervure::onTCPClientGetConnect, this, _1));
    m_pEH->listen<connect_recv_stream_error>(boost::bind(&NetNervure::onConnRecvOrSendError, this, _1, _2));
    m_pEH->listen<connect_sent_stream_error>(boost::bind(&NetNervure::onConnRecvOrSendError, this, _1, _2));
}

NetNervure::~NetNervure()
{
}
boost::shared_ptr<TCPClient> NetNervure::addTCPClient(const std::string& ip, uint16_t iTCPPort)
{
    tcp::endpoint ep(boost::asio::ip::address_v4::from_string(ip), iTCPPort);
    return addTCPClient(ep);
}
boost::shared_ptr<TCPClient> NetNervure::addTCPClient(tcp::endpoint ep)
{
    boost::shared_ptr<ffnet::TCPClient> pclient(new TCPClient(m_oIOService, m_pBS, m_pEH, m_pRPH, ep));
    m_oClients.push_back(pclient);
    return pclient;
}

boost::shared_ptr<TCPServer> NetNervure::addTCPServer(const std::string& ip, uint16_t iTCPPort)
{
    tcp::endpoint ep(boost::asio::ip::address_v4::from_string(ip), iTCPPort);
    return addTCPServer(ep);
}
boost::shared_ptr<TCPServer> NetNervure::addTCPServer(tcp::endpoint ep)
{
    boost::shared_ptr<TCPServer> pserver(new TCPServer(m_oIOService, m_pBS, m_pEH, m_pRPH, ep));
    m_oServers.push_back(pserver);
    return pserver;
}

boost::shared_ptr<UDPPoint> NetNervure::addUDPPoint(const std::string& ip, uint16_t iUDPPort)
{
    udp::endpoint ep(boost::asio::ip::address_v4::from_string(ip), iUDPPort);
    return addUDPPoint(ep);
}

boost::shared_ptr<UDPPoint> NetNervure::addUDPPoint(udp::endpoint ep)
{
    boost::shared_ptr<UDPPoint> ppoint(new UDPPoint(m_oIOService, m_pBS, m_pEH, m_pRPH, ep));
    m_oUDPPoints.push_back(ppoint);
    return ppoint;
}

void NetNervure::onConnRecvOrSendError(ASIOConnPtr_t pConn, boost::system::error_code ec)
{
    //todo, should revise the connections.
    for(TCPClients_t::iterator it = m_oClients.begin(); it != m_oClients.end(); ++it)
    {
        TCPClient_ptr p = *it;
        if(p.get() == pConn)
        {
            pConn->close();
            m_oClients.erase(it);
            m_pEH->triger<tcp_lost_connection>(pConn->getRemoteEndpointPtr());
            break;
        }
    }
    for(TCPConnections_t::iterator it = m_oTCPConnections.begin(); it != m_oTCPConnections.end(); ++it)
    {
        TCPConnection_ptr p = *it;
        if(p.get() == pConn)
        {
            pConn->close();
            m_oTCPConnections.erase(it);
            m_pEH->triger<tcp_lost_connection>(pConn->getRemoteEndpointPtr());
        }
    }
}

void NetNervure::onTCPClientGetConnect(TCPClient * pClient)
{
    m_pEH->triger<tcp_get_connection>(pClient);
}
void NetNervure::onTCPServerAcceptConnect(TCPConnection_ptr pConn)
{
    m_oTCPConnections.push_back(pConn);
    m_pEH->triger<tcp_get_connection>(pConn.get());
}

void NetNervure::stop()
{
    for(TCPServers_t::iterator it = m_oServers.begin(); it!= m_oServers.end(); ++it)
    {
        TCPServer_ptr p = *it;
        p->close();
    }
    m_oServers.clear();;
    for(TCPConnections_t::iterator it = m_oTCPConnections.begin(); it!= m_oTCPConnections.end(); ++it)
    {
        TCPConnection_ptr p = *it;
        p->close();
    }
    m_oTCPConnections.clear();
    for(TCPClients_t::iterator it = m_oClients.begin(); it != m_oClients.end(); ++it)
    {
        TCPClient_ptr p = *it;
        p->close();
    }
    m_oClients.clear();
}

void NetNervure::run()
{
    m_oIOService.run();
}


}//end namespace ffnet