#include <iostream>
#include "network.h"
#include "message.pb.h"

ffnet::NervureConfigure nc("../clnt_net_conf.ini");

void	sendPingMsg(ffnet::EndpointPtr_t tp)
{
    boost::shared_ptr<PingPong::Ping> pMsg(new PingPong::Ping());
    pMsg->set_msg("ping from client");
    pMsg->set_id(1);
	
    ffnet::NetNervure::send(pMsg, tp);

    std::cout<<"service running..."<<std::endl;
}

void	onRecvPong(boost::shared_ptr<PingPong::Pong>pPong, ffnet::EndpointPtr_t pEP)
{
    std::cout<<"got pong! "<<pPong->msg()<<std::endl;
    sendPingMsg(pEP);
}

int main(int argc, char **argv) {
	
    ffnet::Log::init(ffnet::Log::TRACE, "clnt.log");
    ffnet::ProtoBufNervure pbn;
	
    uint16_t port = nc.get<uint16_t>("udp-server.port");

    pbn.initUDPServer("127.0.0.1", port);
	
    pbn.addNeedToRecvPkg<PingPong::Pong>(onRecvPong);
	
    uint16_t clnt_port = nc.get<uint16_t>("udp-client.port");
    ffnet::EndpointPtr_t tpp(
        new ffnet::Endpoint(ffnet::udp_v4, 
                boost::asio::ip::address::from_string(nc.get<std::string>("udp-client.ip-addr")),
		nc.get<uint16_t>("udp-client.port")));
    sendPingMsg(tpp);
    pbn.run();
	
    std::cout<<"Quiting main..."<<std::endl;
    return 0;
}
