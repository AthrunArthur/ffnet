#ifndef _NETWORK_NETWORK_TCP_CLIENT_H_
#define _NETWORK_NETWORK_TCP_CLIENT_H_
#include "common.h"
#include "network/tcp_connection_base.h"
namespace ffnet
{
    
class TCPClient : public TCPConnectionBase
{
public:
    TCPClient(NetNervure *pNervure, Endpoint &ep);

    void     handleConnected(const boost::system::error_code &ec);

protected:
};//end class TCPClient
}//end namespace ffnet
#endif
