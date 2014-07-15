#ifdef USING_MOCK_NET
#include "mock/mock_network.h"
#else
#ifndef _NETWORK_FRAMEWORK_NET_NERVURE_H_
#define _NETWORK_FRAMEWORK_NET_NERVURE_H_
#include "common.h"
#include "common/blocking_queue.h"
#include "network/endpoint_data.h"
#include "middleware/bonder_splitter.h"
#include <boost/thread/detail/thread.hpp>
#include <map>
#ifdef PROTO_BUF_SUPPORT
#include <google/protobuf/message.h>
#endif

namespace ffnet
{
class ASIOConnection;
typedef boost::shared_ptr<ASIOConnection> ASIOConnectionPtr_t;
class TCPServer;
class TCPConnectionBase;

using boost::asio::io_service;

class NetNervure
{
public:
    typedef boost::function<void () > Func_t;
public:
    NetNervure( BonderSplitterPtr_t pBonderSplitter);
    virtual ~NetNervure();

    static void				send(const boost::shared_ptr<Package> & pPkg, const EndpointPtr_t & ep);
#ifdef PROTO_BUF_SUPPORT
    static void 			send(const boost::shared_ptr<google::protobuf::Message> & pMsg, const EndpointPtr_t & ep);
#endif
    void 					run();

    void					stop();


    inline io_service 		&getIOService() {
        return m_oIOService;
    }
    inline BonderSplitterPtr_t 	getBonderSplitter() {
        return m_pBonderSplitter;
    }

    inline BlockingQueue<Func_t>&	getTaskQueue() {
        return m_oTasks;
    }

    void						initTCPServer(const std::string & ip, uint16_t iTCPPort);
    void						initUDPServer(const std::string & ip, uint16_t iUDPPort);
    void						addTCPClient(const EndpointPtr_t & remoteEndPoint);

protected:
    friend class ASIOConnection;


    void						stopInThisThread();


    virtual void				deseralizeAndDispatchHandler(const EndPointBufferPtr_t & epb);

protected:
    typedef std::list<ASIOConnectionPtr_t>	ConnContainer_t;

    boost::asio::io_service			m_oIOService;
    BlockingQueue<Func_t>				m_oTasks;
    TCPServer*                                  m_pTCPServer;
    ConnContainer_t					m_oConnections;
    boost::thread					m_oIOThread;
    BonderSplitterPtr_t				m_pBonderSplitter;
    bool					m_bIsStopped;
};//end class NetNervure
}//end namespace ffnet
#endif
#endif //end USING_MOCK_NET
