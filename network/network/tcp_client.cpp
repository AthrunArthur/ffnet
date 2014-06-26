#include "network/tcp_client.h"
#include "network/endpoint_data.h"
#include "framework/net_nervure.h"
#include "framework/event.h"
#include "common/defines.h"

namespace ffnet
{
using namespace ::ffnet::event;
using namespace ::ffnet::event::more;

TCPClient::TCPClient(NetNervure *pNervure, Endpoint &ep)
: TCPConnectionBase(pNervure)
{
    boost::system::error_code ec;
	tcp::endpoint tep;
	ep.generateTypedEndpoint(tep);
	Event<tcp_client_start_connection>::triger(nervure(),
		boost::bind(tcp_client_start_connection::event, tep, _1)
	);
    m_oSocket.async_connect(tep, boost::bind(&TCPClient::handleConnected,
                                            this, boost::asio::placeholders::error()));
}

void TCPClient::handleConnected(const boost::system::error_code &ec)
{
    if(!ec) {
		LOG_TRACE(tcp_client)<<"Get connection succ!";
		Event<tcp_client_get_connection_succ>::triger(nervure(),
			boost::bind(tcp_client_get_connection_succ::event,
						this, _1)
		);
        m_oRemoteEndpoint = EndpointPtr_t(new Endpoint(m_oSocket.remote_endpoint()));
        startRecv();
    } else {
		LOG_DEBUG(tcp_client) <<"Get connection error!";
		Event<tcp_client_conn_error>::triger(nervure(),
			boost::bind(tcp_client_conn_error::event,
						this, ec, _1)
		);
    }
}
}//end namespace ffnet
