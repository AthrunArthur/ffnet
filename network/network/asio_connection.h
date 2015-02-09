#ifndef _NETWORK_NETWORK_ASIO_CONNECTION_H_
#define _NETWORK_NETWORK_ASIO_CONNECTION_H_

#include "common.h"
#include "network/endpoint_data.h"
#include "middleware/bonder_splitter.h"
#include "middleware/event_handler.h"
#include "package/package.h"
#include <boost/noncopyable.hpp>
#include <boost/atomic.hpp> 
#ifdef PROTO_BUF_SUPPORT
#include <google/protobuf/message.h>
#endif

namespace ffnet
{
using namespace boost::asio;
using namespace boost::asio::ip;

class RawPkgHandler;

class ASIOConnection : public boost::noncopyable
{
public:
    enum ConnState{
      s_init,
      s_valid,
      s_closed,
      s_error,
    };

    /*
    virtual void        send(const PackagePtr_t & pkg, const EndpointPtr_t & pEndpoint);
    virtual void        send(const PackagePtr_t & pkg);
#ifdef PROTO_BUF_SUPPORT
    virtual void send(const boost::shared_ptr< google::protobuf::Message > & pMsg, const EndpointPtr_t & ep);
    virtual void send(const boost::shared_ptr<google::protobuf::Message> & pMsg);
#endif
    */
    
    ASIOConnection(io_service & ioservice, BonderSplitter * bs, EventHandler * eh, RawPkgHandler * rph);
    virtual ~ASIOConnection();

    inline BonderSplitter         *bonderSplitter() const {return m_pBonderSplitter;}
    inline io_service &            ioservice() {return m_oIOService;}
    inline const io_service &      ioservice() const {return m_oIOService;}
    inline EventHandler *          eventHandler() const{return m_pEH;}
    inline RawPkgHandler *         rawPkgHandler() const{return m_pRPH;}

    virtual void         close() {m_iConnectionState.store(s_closed);}

    
    virtual bool        isFree() = 0;
    virtual EndpointPtr_t getRemoteEndpointPtr() = 0;
    
protected:
    virtual void        startSend() = 0;
    virtual void        startRecv() = 0;
    
    virtual void        sliceAndDispatchPkg();
    virtual void         handlReceivedPkg(const boost::system::error_code &error, size_t bytes_transferred);
    virtual void        handlePkgSent(const boost::system::error_code & /*error*/,
                                          std::size_t /*bytes_transferred*/);
    

protected:
    io_service &            m_oIOService;
    BonderSplitter *        m_pBonderSplitter;
    EventHandler *          m_pEH;
    RawPkgHandler *         m_pRPH;
    NetBuffer               m_oRecvBuffer;
    NetBuffer               m_oSendBuffer;
    boost::mutex        m_oMutex;
    bool            m_bIsSending;
    boost::atomic<ConnState>    m_iConnectionState;

};//end class ASIOConnection

typedef boost::shared_ptr<ASIOConnection>        ASIOConnectionPtr_t;
}//end namespace ffnet
#endif
