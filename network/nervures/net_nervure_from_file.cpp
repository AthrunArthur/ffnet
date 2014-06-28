#include "nervures/net_nervure_from_file.h"

namespace ffnet
{
NetNervureFromFile::NetNervureFromFile(String conf)
    : TypedNetNervure()
	, m_pConfigure(new NervureConfigure(conf))
{
	
    NervureConfigure & nc = *(m_pConfigure.get());

    int8_t tcp_server_flag = nc.get<int8_t>("conf.open-tcp-server");
    int8_t udp_server_flag = nc.get<int8_t>("conf.open-udp-server");
    int8_t tcp_client_flag = nc.get<int8_t>("conf.open-tcp-client");

    if(tcp_server_flag) {
        String ip = nc.get<String>("tcp-server.ip");
        uint16_t port = nc.get<uint16_t>("tcp-server.port");
        initTCPServer(ip, port);
    }
    if(udp_server_flag) {
        String ip = nc.get<String>("udp-server.ip");
        uint16_t port = nc.get<uint16_t>("udp-server.port");
        initUDPServer(ip, port);
    }
    if(tcp_client_flag) {
        String ip = nc.get<String>("tcp-client.target-svr-ip-addr");
        uint16_t port = nc.get<uint16_t>("tcp-client.target-svr-port");

        EndpointPtr_t ep(new Endpoint(ffnet::tcp_v4, boost::asio::ip::address_v4::from_string(ip), port));
        
        addTCPClient(ep);
    }
}
}//end namespace ffnet
