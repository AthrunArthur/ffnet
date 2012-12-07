#ifndef _NETWORK_NETWORK_ASIO_CONNECTION_H_
#define _NETWORK_NETWORK_ASIO_CONNECTION_H_

#include "common.h"
#include "network/endpoint_data.h"
#include "middleware/bonder_splitter.h"
#include "package/package.h"
#include <boost/noncopyable.hpp>

namespace ffnet
{
	class ASIOConnHandler;
	typedef boost::shared_ptr<ASIOConnHandler> ASIOConnHandlerPtr_t;
class NetNervure;
	namespace details
{
using namespace boost::asio;
using namespace boost::asio::ip;


class TCPConnectionBase;
class UDPPoint;

class ASIOConnection : public boost::noncopyable
{
public:
    ASIOConnection(NetNervure *pNervure);
    virtual ~ASIOConnection();

    NetNervure 			*nervure() const {
        return m_pNervure;
    }
    ASIOConnHandlerPtr_t handler() const {
        return m_pHandler;
    }
    BonderSplitter 		*bonderSplitter() const {
        return m_pBonderSplitter;
    }
    virtual void			send(PackagePtr_t pkg, EndpointPtr_t pEndpoint) = 0;
    virtual void 		close() {};
    virtual TCPConnectionBase *TCPConnectionBasePointer() {
        return NULL;
    }
    virtual UDPPoint 	*UDPPointPointer() {
        return NULL;
    }
    
protected:
    virtual EndpointPtr_t getRemoteEndpointPtr() = 0;
    virtual 	void 		startSend() = 0;
    virtual void			startRecv() = 0;
    
    virtual void			sliceAndDispatchPkg();
	virtual void 			handlReceivedPkg(const boost::system::error_code &error, size_t bytes_transferred);
    virtual void			handlePkgSent(const boost::system::error_code & /*error*/,
                                          std::size_t /*bytes_transferred*/);
	

protected:
    NetNervure 			*m_pNervure;
    io_service 			&m_oIOService;
    ASIOConnHandlerPtr_t m_pHandler;
    BonderSplitter 		*m_pBonderSplitter;
    NetBuffer		m_oRecvBuffer;
    NetBuffer		m_oSendBuffer;
    boost::mutex		m_oMutex;
    bool				m_bIsSending;

};//end class ASIOConnection

typedef boost::shared_ptr<ASIOConnection>		ASIOConnectionPtr_t;
}//end namespace details
}//end namespace ffnet
#endif