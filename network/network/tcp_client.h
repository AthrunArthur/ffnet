#pragma once
#include "common.h"
#include "network/tcp_connection_base.h"
namespace ffnet
{

class net_tcp_client : public net_tcp_connection_base
{
public:
    net_tcp_client(io_service & ioservice, pkg_packer * bs,
              event_handler * eh, const std::vector<tcp_pkg_handler *> & rph, const tcp_endpoint & ep);
protected:
    void handle_connected(const boost::system::error_code &ec);
};//end class tcp_client
}//end namespace ffnet
