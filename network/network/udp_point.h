#ifndef _NETWORK_NETWORK_UDP_POINT_H_
#define _NETWORK_NETWORK_UDP_POINT_H_
#include "common.h"
#include "network/asio_connection.h"
#include "package/package.h"
#include "utils.h"


namespace ffnet
{
	class NetNervure;
namespace details
{
class UDPPoint: public ffnet::details::ASIOConnection
{
public:
    UDPPoint(NetNervure *pNervure, uint16_t iPort);
	
	virtual void			send(PackagePtr_t pkg, EndpointPtr_t pEndpoint);
	
    virtual UDPPoint* 	UDPPointPointer(){return this;}
	virtual EndpointPtr_t getRemoteEndpointPtr();
protected:
	virtual 	void 		startSend();
	virtual void			startRecv();
	void 	actualSendPkg(PackagePtr_t pkg, EndpointPtr_t pEndpoint);
protected:
	typedef boost::function<void () > Func_t;
	udp::socket		m_oSocket;
	udp::endpoint	m_oRemoteEndPoint;
	utl::sync_queue<Func_t>		m_oSendTasks;
};//end class UDPPoint
typedef boost::shared_ptr<UDPPoint> UDPPointPtr_t;
}//end namespace details
}//end namespace ffnet
#endif
