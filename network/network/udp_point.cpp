#include "network/udp_point.h"
#include "network/events.h"
#include "common/defines.h"

namespace ffnet
{
using namespace ffnet::details;
using namespace ::ffnet::event;

UDPPoint::UDPPoint(io_service & ioservice, BonderSplitter *bs,
                   EventHandler * eh, RawPkgHandler * rph, ip::udp::endpoint ep)
: ASIOConnection(ioservice, bs, eh, rph)
, m_oSocket(ioservice, ep)
{
    m_iConnectionState.store(s_valid);
    //GlobalConnections::instance()->addUDPPoint(this);
    startRecv();
}

UDPPoint::~UDPPoint()
{
    //GlobalConnections::instance()->delUDPPoint(this);
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


void UDPPoint::send(const PackagePtr_t & pkg, const EndpointPtr_t & pEndpoint)
{
    if (m_iConnectionState.load() != s_valid)
    {
      m_pEH->triger<pkg_send_failed>(pkg, pEndpoint);
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
#ifdef PROTO_BUF_SUPPORT
void UDPPoint::send(const boost::shared_ptr< google::protobuf::Message > & pMsg, const EndpointPtr_t & ep)
{
    boost::shared_ptr<Package> pPkg(new ::ffnet::ProtoBufWrapperPkg(pMsg));
    send(pPkg, ep);
}
#endif

void UDPPoint::actualSendPkg(const PackagePtr_t & pkg, const EndpointPtr_t & pEndpoint)
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
