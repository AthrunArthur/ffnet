#include "handler/debug_event.h"
#include "log.h"
#include "utils.h"
#include "network/end_point.h"
#include "network/endpoint_data.h"
namespace ffnet
{
namespace details
{
void debug_tcp_server_start_listen(const boost::asio::ip::tcp::acceptor& acceptor)
{
	//log_tcp_server("TCPServer", "startAccept(), listening on %s:%d", 
	//			   ffnet::toString(acceptor.local_endpoint()).c_str(), acceptor.local_endpoint().port());
}

}//end namespace details
}//end namespace ffnet