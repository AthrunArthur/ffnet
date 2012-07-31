#ifndef _NETWORK_FRAMEWORK_NET_NERVURE_H_
#define _NETWORK_FRAMEWORK_NET_NERVURE_H_
#include "common.h"
#include "common/cond_pop_queue.h"
#include "network/endpoint_data.h"	
#include "middleware/bonder_splitter.h"
#include "handler/asio_conn_handler.h"
#include <boost/thread/detail/thread.hpp>
#include <handler/asio_conn_handler.h>
#include <map>


namespace ffnet
{
namespace details
{
	class TCPServer;
}
using ffnet::details::ASIOConnectionPtr_t;
using ffnet::details::TCPServer;
using boost::asio::io_service;

class NetNervure
{
public:
    NetNervure(ASIOConnHandlerPtr_t pNetHandler, BonderSplitterPtr_t pBonderSplitter);
    virtual ~NetNervure();
	
	static void				send(boost::shared_ptr<Package> pPkg, EndpointPtr_t ep);
	
    void 					run();

	void						stop();
	
    inline io_service 		&getIOService() {
        return m_oIOService;
    }
    inline ASIOConnHandlerPtr_t & getHandler() {return m_pConnHandler;}
    inline BonderSplitterPtr_t 	getBonderSplitter(){return m_pBonderSplitter;}
    
    
	
protected:
    friend class ASIOConnection;
	typedef boost::function<void () > Func_t;
	
	void						initTCPServer(uint16_t iTCPPort);
	void						initUDPServer(uint16_t iUDPPort);
	void						addTCPClient(EndpointPtr_t remoteEndPoint);
	
	void					stopInThisThread();

	
	virtual void				deseralizeAndDispatchHandler(EndPointBufferPtr_t epb);

protected:
	typedef std::list<ASIOConnectionPtr_t>	ConnContainer_t;
	
    boost::asio::io_service			m_oIOService;
    CondPopQueue<Func_t>				m_oTasks;
    boost::shared_ptr<TCPServer>		m_pTCPServer;
	ConnContainer_t					m_oConnections;
	ASIOConnHandlerPtr_t				m_pConnHandler;
	boost::thread					m_oIOThread;
	BonderSplitterPtr_t				m_pBonderSplitter;
	bool								m_bIsStopped;
};//end class NetNervure
}//end namespace ffnet
#endif

