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
    virtual ~TCPConnectionBase();
    
    //This may be called in another thread, and it's thread safe.
    virtual void            send(const PackagePtr_t & pkg);
    
    virtual void            send(const char * pBuf, size_t len);
    
#ifdef PROTO_BUF_SUPPORT
    virtual void            send(const boost::shared_ptr<google::protobuf::Message> & pMsg);
#endif
/*
    inline tcp::socket     &getSocket() {
        return m_oSocket;
    }
    */
    
    virtual void            close();
    virtual bool            isFree();
    
    virtual TCPConnectionBase * TCPConnectionBasePointer(){return this;}
    virtual EndpointPtr_t getRemoteEndpointPtr();
protected:
    TCPConnectionBase(io_service & ioservice, BonderSplitter *bs,
                      EventHandler * eh, RawPkgHandler *rph);
    
    virtual void         startSend();
    virtual void        startRecv();

protected:
    tcp::socket         m_oSocket;
    EndpointPtr_t       m_oRemoteEndpoint;
};//class TCPConnectionBase
typedef boost::shared_ptr<TCPConnectionBase> TCPConnectionBasePtr_t;
}//end namespace ffnet
#endif

