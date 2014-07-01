#include "network/udp_point.h"
#include "framework/net_nervure.h"
#include "framework/global_connections.h"
#include "framework/event.h"

namespace ffnet
{
using namespace ffnet::details;
using namespace ::ffnet::event;

UDPPoint::UDPPoint(ffnet::NetNervure *pNervure, const std::string & ip, uint16_t iPort)
: ffnet::details::ASIOConnection(pNervure)
, m_oSocket(pNervure->getIOService(), udp::endpoint(ip::address::from_string(ip.c_str()), iPort))
{
    m_iConnectionState.store(s_valid);
    GlobalConnections::instance()->addUDPPoint(this);
    startRecv();
}

UDPPoint::~UDPPoint()
{
    GlobalConnections::instance()->delUDPPoint(this);
}

EndpointPtr_t UDPPoint::getRemoteEndpointPtr()
{
    EndpointPtr_t pEP (new Endpoint(m_oRemoteEndPoint));
    return pEP;
}

void UDPPoint::startRecv()
{
    //m_pHandler->onUDPStartReceive(this);

    m_oSocket.async_receive_from(
        boost::asio::buffer(m_oRecvBuffer.writeable()), m_oRemoteEndPoint,
        boost::bind(&UDPPoint::handlReceivedPkg, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
}
void UDPPoint::startSend()
{
    Func_t f;
    if(m_oSendTasks.pop(f))
    {
        f();
    }
    else
    {
        m_oMutex.lock();
        m_bIsSending = false;
        m_oMutex.unlock();
    }
}
void UDPPoint::send(PackagePtr_t pkg, EndpointPtr_t pEndpoint)
{
    if (m_iConnectionState.load() != s_valid)
    {
      Event<pkg_send_failed>::triger(nervure(),
          boost::bind(pkg_send_failed::event, pkg, pEndpoint, _1));
      return ;
    }
    m_oSendTasks.push(boost::bind(&UDPPoint::actualSendPkg, this, pkg, pEndpoint));
    m_oMutex.lock();
    if(!m_bIsSending)
    {
        m_bIsSending = true;
        m_oMutex.unlock();
        startSend();
    }
    else
        m_oMutex.unlock();
}

void UDPPoint::actualSendPkg(PackagePtr_t pkg, EndpointPtr_t pEndpoint)
{
    //m_pHandler->onUDPStartSend(this);
    m_pBonderSplitter->bond(m_oSendBuffer, pkg);
    UDPEndPoint uep;
    pEndpoint->generateTypedEndpoint(uep);
    m_oSocket.async_send_to(boost::asio::buffer(m_oSendBuffer.readable()), uep,
                                   boost::bind(&UDPPoint::handlePkgSent, this,
                                               boost::asio::placeholders::error,
                                               boost::asio::placeholders::bytes_transferred));
}
void UDPPoint::close()
{
    m_oSocket.close();
    m_iConnectionState.store(s_closed);
}

bool UDPPoint::isFree()
{
    if(m_oSendTasks.size() == 0)
        return true;
    return false;
}

}//end namespace ffnet
