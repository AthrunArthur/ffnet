#include "network/tcp_client.h"
#include "network/endpoint_data.h"
#include "network/events.h"
#include "common/defines.h"

namespace ffnet
{
using namespace ::ffnet::event;
using namespace ::ffnet::event::more;

TCPClient::TCPClient(io_service & ioservice, BonderSplitter * bs,
                     EventHandler *eh, RawPkgHandler * rph, boost::asio::ip::tcp::endpoint &ep)
: TCPConnectionBase(ioservice, bs, eh, rph)
{
    boost::system::error_code ec;
    tcp::endpoint tep = ep;
    m_pEH->triger<tcp_client_start_connection>(tep);
    m_oSocket.async_connect(tep, boost::bind(&TCPClient::handleConnected,
                                            this, boost::asio::placeholders::error()));
}

void TCPClient::handleConnected(const boost::system::error_code &ec)
{
    if(!ec) {
        LOG_TRACE(tcp_client)<<"Get connection succ!";
        m_iConnectionState.store(s_valid);
        m_oRemoteEndpoint = EndpointPtr_t(new Endpoint(m_oSocket.remote_endpoint()));
        m_pEH->triger<tcp_client_get_connection_succ>(this);
        startRecv();
    } else {
      m_iConnectionState.store(s_error);
      LOG_DEBUG(tcp_client) <<"Get connection error!";
      m_pEH->triger<tcp_client_conn_error>(this, ec);
    }
}
}//end namespace ffnet
