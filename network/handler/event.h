#ifndef NETWORK_HANDLER_EVENT_H_
#define NETWORK_HANDLER_EVENT_H_
#include "common.h"
#include <boost/function.hpp>
#include "framework/net_nervure.h"
#include "network/tcp_connection_base.h"
#include "network/asio_connection.h"
#include "network/tcp_client.h"
#include "network/tcp_server.h"
#include "middleware/net_dispatcher.h"
#include <vector>


namespace ffnet
{
namespace event
{



template<class ETy_>
class Event {
public:
    typedef typename ETy_::event_handler event_handler;
    typedef boost::function<void (event_handler) > event_triger;
	class HandlerAndDispatcher
	{
	public:
		typedef boost::function<void () > Func_t;
		typedef ffnet::CondPopQueue<Func_t> TQ_t;
		HandlerAndDispatcher(event_handler h, TQ_t & q)
		:m_oHandler(h), m_oTaskQueue(q){}
	public:
		event_handler 		m_oHandler;
		TQ_t &	m_oTaskQueue;
		
	};//end HandlerAndDispatcher
	typedef boost::shared_ptr<HandlerAndDispatcher> HandlerAndDispatcherPtr_t;
	typedef std::vector<HandlerAndDispatcherPtr_t>	Container_t;
	
    static void listen(::ffnet::NetNervure * nn, event_handler h)
    {
		HandlerAndDispatcherPtr_t hadp(new HandlerAndDispatcher(h, nn->getTaskQueue()));
        instance()->m_oContainer.push_back(hadp);
    }
    static void triger(event_triger t)
    {
        if(s_pInstance)
        {
            for(typename Container_t::iterator it = instance()->m_oContainer.begin(); 
				it!=instance()->m_oContainer.end(); ++it)
            {
                //t(*it);
                (*it)->m_oTaskQueue.push_back(boost::bind(t, (*it)->m_oHandler));
            }
        }
    }
protected:
    Event() : m_oContainer() {}
    static Event<ETy_> * 		instance() {
        if(s_pInstance == NULL)
        {
            s_pInstance = new Event<ETy_>();
        }
        return s_pInstance;
    }
    static Event<ETy_> *		internalUseInstance()
    {
        return s_pInstance;
    }
protected:
    Container_t					m_oContainer;
    static Event<ETy_> *		s_pInstance;
};//end class Event
template <class ETy_>
Event<ETy_> *		Event<ETy_>::s_pInstance = NULL;



struct tcp_get_connection
{
	typedef ffnet::TCPConnectionBase TCPConnectionBase;
    typedef boost::function<void (TCPConnectionBase *)> event_handler;
    static void 	event(TCPConnectionBase * pConn, event_handler h)
    {
        h(pConn);
    }
};//end tcp_get_connection
struct tcp_lost_connection
{
	typedef ffnet::TCPConnectionBase TCPConnectionBase;
    typedef boost::function<void (TCPConnectionBase *)> event_handler;
    static void event(TCPConnectionBase * pConn, event_handler h)
    {
        h(pConn);
    }
};//end tcp_get_connection
struct udp_send_recv_exception
{
    typedef ffnet::Endpoint endpoint;
    //TODO, what's the event_handler we need?
};//end udp_send_recv_exception

namespace more
{
struct tcp_server_start_listen
{
    typedef boost::asio::ip::tcp::endpoint endpoint;
    typedef boost::function<void (endpoint) > 	event_handler;
    static void 	event(endpoint tcp, event_handler h)
    {
        h(tcp);
    }
};//end struct tcp_server_start_listen

struct tcp_server_accept_connection
{
    typedef boost::shared_ptr<ffnet::TCPConnection> TCPConnectionPtr_t;
    typedef boost::function<void (TCPConnectionPtr_t) > event_handler;
    static void 	event(TCPConnectionPtr_t conn, event_handler h)
    {
        h(conn);
    }
};//end struct tcp_server_accept_connection
struct tcp_server_accept_error
{
    typedef boost::asio::ip::tcp::endpoint endpoint;
    typedef boost::system::error_code error_code;
    typedef boost::function<void (endpoint, error_code)> event_handler;
    static void		event(endpoint svr, error_code error, event_handler h)
    {
        h(svr, error);
    }
};//end tcp_server_accept_error
struct tcp_start_recv_stream {
    typedef boost::asio::ip::tcp::endpoint endpoint;
    typedef boost::function<void (endpoint, endpoint)> event_handler;
    static void event(endpoint local, endpoint remote, event_handler h)
    {
        h(local, remote);
    }
};//end tcp_start_recv_stream
struct tcp_start_send_stream {
    typedef ffnet::TCPConnectionBase * TCPConnectionBasePtr;
    typedef boost::function<void (TCPConnectionBasePtr,  const char *, size_t)> event_handler;
    static void event(TCPConnectionBasePtr p, const char * buf, size_t buf_len, event_handler h)
    {
        h(p, buf, buf_len);
    }
};//end tcp_start_send_stream
struct tcp_client_start_connection {
    typedef boost::asio::ip::tcp::endpoint endpoint;
    typedef boost::function<void (endpoint) > event_handler;
    static void event(endpoint remote, event_handler h)
    {
        h(remote);
    }
};//end tcp_client_start_connection
struct tcp_client_get_connection_succ {
    typedef ::ffnet::TCPClient *TCPClientPtr_t;
    typedef boost::function<void (TCPClientPtr_t)> event_handler;
    static void event(TCPClientPtr_t client_ptr, event_handler h)
    {
        h(client_ptr);
    }
};//end tcp_client_get_connection
struct tcp_client_conn_error {
    typedef ::ffnet::TCPClient *TCPClientPtr_t;
    typedef boost::system::error_code error_code;
    typedef boost::function<void (TCPClientPtr_t, error_code)> event_handler;
    static void event(TCPClientPtr_t client_ptr, error_code error, event_handler h)
    {
        h(client_ptr, error);
    }
};//end tcp_client_conn_error
struct connect_sent_stream_succ {
    typedef ::ffnet::ASIOConnection * ASIOConnPtr_t;
    typedef boost::function<void (ASIOConnPtr_t, size_t)> event_handler;
    static void event(ASIOConnPtr_t pConn, size_t len, event_handler h)
    {
        h(pConn, len);
    }
};//end connect_sent_stream
struct connect_sent_stream_error {
    typedef ::ffnet::ASIOConnection *ASIOConnPtr_t;
    typedef boost::system::error_code	error_code;
    typedef boost::function<void (ASIOConnPtr_t, error_code)> event_handler;
    static void event(ASIOConnPtr_t pConn, error_code error, event_handler h)
    {
        h(pConn, error);
    }
};//end connect_sent_stream_error
struct connect_recv_stream_succ {
    typedef ::ffnet::ASIOConnection * ASIOConnPtr_t;
    typedef boost::function<void (ASIOConnPtr_t, size_t)> event_handler;
    static void event(ASIOConnPtr_t pConn, size_t len, event_handler h)
    {
        h(pConn, len);
    }
};//end connect_recv_stream_succ
struct connect_recv_stream_error {
    typedef ::ffnet::ASIOConnection *ASIOConnPtr_t;
    typedef boost::system::error_code	error_code;
    typedef boost::function<void (ASIOConnPtr_t, error_code)> event_handler;
    static void event(ASIOConnPtr_t pConn, error_code error, event_handler h)
    {
        h(pConn, error);
    }
};//end connect_recv_stream_error
}//end namespace more
}//end namespace event
}//end namespace ffnet
#endif