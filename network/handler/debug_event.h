#ifndef NETWORK_HANDLER_DEBUG_EVENT_H
#define NETWORK_HANDLER_DEBUG_EVENT_H
#include "common.h"
#include <boost/asio.hpp>

namespace ffnet
{
namespace details
{
void		debug_tcp_server_start_listen(const boost::asio::ip::tcp::acceptor & acceptor);
}//end namespace details;
}//end namespace ffnet;
#endif