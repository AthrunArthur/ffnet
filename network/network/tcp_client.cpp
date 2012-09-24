#include "network/tcp_client.h"
#include "log.h"
#include "network/endpoint_data.h"
#include "framework/net_nervure.h"

namespace ffnet
{
namespace details
{
TCPClient::TCPClient(NetNervure *pNervure, Endpoint &ep)
: TCPConnectionBase(pNervure)
{
    boost::system::error_code ec;
	tcp::endpoint tep;
	ep.generateTypedEndpoint(tep);
	FFNET_DEBUG(
	log_tcp_client("TCPClient", "TCPClient(), connecting to %s:%d", ffnet::toString(ep).c_str(), ep.port());
	)
    m_oSocket.async_connect(tep, boost::bind(&TCPClient::handleConnected,
                                            this, boost::asio::placeholders::error()));
}

void TCPClient::handleConnected(const boost::system::error_code &ec)
{
    if(!ec) {
	FFNET_DEBUG(log_tcp_client("TCPClient", "handleConnected(), connected!");)
        //m_pHandler->onGotConnection(this, ec);
        startRecv();
    } else {
        m_pHandler->onConnectionError(this, ec);
	FFNET_DEBUG(
        log_tcp_client("TCPClient", "handleConnected, get error:%s", boost::system::system_error(ec).what());
	)
    }
}
}//end namespace details
}//end namespace ffnet