#ifndef _NETWORK_NETWORK_TCP_CONNECTION_BASE_H_
#define _NETWORK_NETWORK_TCP_CONNECTION_BASE_H_
#include "common.h"
#include "network/net_buffer.h"
#include "package/package.h"
#include "network/asio_connection.h"

namespace ffnet
{
	using namespace boost::asio;
	using namespace boost::asio::ip;
	
class TCPConnectionBase : public ASIOConnection, public boost::enable_shared_from_this<TCPConnectionBase>
{
public:

public:
	virtual ~TCPConnectionBase();
    //This may be called in another thread, and it's thread safe.
	virtual void			send(PackagePtr_t pkg, EndpointPtr_t pEndpoint);

    inline tcp::socket 	&getSocket() {
        return m_oSocket;
    }
    
    virtual void			close();
	virtual bool			isFree();
	
	virtual TCPConnectionBase * TCPConnectionBasePointer(){return this;}
	virtual EndpointPtr_t getRemoteEndpointPtr();
protected:
    TCPConnectionBase(NetNervure * pNervure);
    
	virtual void 		startSend();
	virtual void			startRecv();

protected:
    tcp::socket 		m_oSocket;
};//class TCPConnectionBase
typedef boost::shared_ptr<TCPConnectionBase> TCPConnectionBasePtr_t;
}//end namespace ffnet
#endif

