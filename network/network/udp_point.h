#ifndef _NETWORK_NETWORK_UDP_POINT_H_
#define _NETWORK_NETWORK_UDP_POINT_H_
#include "common.h"
#include "network/asio_connection.h"
#include "package/package.h"
#include "common/sync_queue.h"


namespace ffnet
{
class NetNervure;
using boost::asio::ip::udp;
using boost::asio::ip::udp;

class UDPPoint: public ASIOConnection
{
public:
    UDPPoint(NetNervure *pNervure, const std::string & ip, uint16_t iPort);
    virtual ~UDPPoint();
    virtual void        send(const PackagePtr_t & pkg, const EndpointPtr_t & pEndpoint);
    
    virtual UDPPoint*         UDPPointPointer(){return this;}
    virtual void         close();
    virtual bool        isFree();
    virtual EndpointPtr_t     getRemoteEndpointPtr();
protected:
    virtual void         startSend();
    virtual void        startRecv();
    void             actualSendPkg(const PackagePtr_t & pkg, const EndpointPtr_t & pEndpoint);
protected:
    typedef boost::function<void () > Func_t;
    udp::socket        m_oSocket;
    udp::endpoint    m_oRemoteEndPoint;
    sync_queue<Func_t>        m_oSendTasks;
};//end class UDPPoint
typedef boost::shared_ptr<UDPPoint> UDPPointPtr_t;
}//end namespace ffnet
#endif
