#include "network/tcp_client.h"
#include "network/endpoint_data.h"
#include "framework/net_nervure.h"
#include "handler/event.h"

namespace ffnet
{
namespace details
{
using namespace ::ffnet::event;

TCPClient::TCPClient(NetNervure *pNervure, Endpoint &ep)
: TCPConnectionBase(pNervure)
{
    boost::system::error_code ec;
	tcp::endpoint tep;
	ep.generateTypedEndpoint(tep);
	Event<tcp_client_start_connection>::triger(
		boost::bind(tcp_client_start_connection::event, tep, _1)
	);
    m_oSocket.async_connect(tep, boost::bind(&TCPClient::handleConnected,
                                            this, boost::asio::placeholders::error()));
}

void TCPClient::handleConnected(const boost::system::error_code &ec)
{
    if(!ec) {
		Event<tcp_client_get_connection_succ>::triger(
			boost::bind(tcp_client_get_connection_succ::event,
						this, _1)
		);
        startRecv();
    } else {
		Event<tcp_client_conn_error>::triger(
			boost::bind(tcp_client_conn_error::event,
						this, ec, _1)
		);
    }
}
}//end namespace details
}//end namespace ffnet