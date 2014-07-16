#ifndef _NETWORK_NETWORK_ASIO_CONNECTION_H_
#define _NETWORK_NETWORK_ASIO_CONNECTION_H_

#include "common.h"
#include "network/endpoint_data.h"
#include "middleware/bonder_splitter.h"
#include "package/package.h"
#include <boost/noncopyable.hpp>
#include <boost/atomic.hpp> 
#ifdef PROTO_BUF_SUPPORT
#include <google/protobuf/message.h>
#endif

namespace ffnet
{
class NetNervure;
class TCPConnectionBase;
class UDPPoint;
using namespace boost::asio;
using namespace boost::asio::ip;


class ASIOConnection : public boost::noncopyable
{
public:
    enum ConnState{
      s_init,
      s_valid,
      s_closed,
      s_error,
    };

    ASIOConnection(NetNervure *pNervure);
    virtual ~ASIOConnection();

    NetNervure             *nervure() const {
        return m_pNervure;
    }
    BonderSplitter         *bonderSplitter() const {
        return m_pBonderSplitter;
    }
    virtual void        send(const PackagePtr_t & pkg, const EndpointPtr_t & pEndpoint) = 0;
#ifdef PROTO_BUF_SUPPORT
    virtual void        send(const boost::shared_ptr<google::protobuf::Message> & pMsg, const EndpointPtr_t & ep);
#endif
    virtual void         close() {m_iConnectionState.store(s_closed);};
    virtual TCPConnectionBase *TCPConnectionBasePointer() {
        return NULL;
    }
    virtual UDPPoint *          UDPPointPointer() {
        return NULL;
    }
    
    virtual bool        isFree() = 0;
    virtual EndpointPtr_t getRemoteEndpointPtr() = 0;
    
protected:
    virtual     void                startSend() = 0;
    virtual void        startRecv() = 0;
    
    virtual void        sliceAndDispatchPkg();
    virtual void         handlReceivedPkg(const boost::system::error_code &error, size_t bytes_transferred);
    virtual void        handlePkgSent(const boost::system::error_code & /*error*/,
                                          std::size_t /*bytes_transferred*/);
    

protected:
    NetNervure             *m_pNervure;
    io_service             &m_oIOService;
    BonderSplitter         *m_pBonderSplitter;
    NetBuffer                m_oRecvBuffer;
    NetBuffer                m_oSendBuffer;
    boost::mutex        m_oMutex;
    bool            m_bIsSending;
    boost::atomic<ConnState>    m_iConnectionState;

};//end class ASIOConnection

typedef boost::shared_ptr<ASIOConnection>        ASIOConnectionPtr_t;
}//end namespace ffnet
#endif
