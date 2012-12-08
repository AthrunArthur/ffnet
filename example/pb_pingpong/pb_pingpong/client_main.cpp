#include <iostream>
#include "network.h"
#include "message.pb.h"
#include "../../../network/handler/event.h"
#include <log.h>

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
	PingPong::Pong & msg = *(pPong.get());
	std::cout<<"got pong! "<<msg.msg()<<std::endl;
	sendPingMsg(pEP);
}

void	onConnSucc(ffnet::TCPConnectionBase *pConn)
{
	std::cout<<"connect success"<<std::endl;
	ffnet::EndpointPtr_t tpp(new ffnet::Endpoint(pConn->getSocket().remote_endpoint()));
	sendPingMsg(tpp);
}
void	onLostConn(ffnet::TCPConnectionBase *pConn, ffnet::ProtoBufNervure * pbn)
{
	std::cout<<"Server lost!"<<std::endl;
	pbn->stop();
}
int main(int argc, char **argv) {
	
    initialize_log("clnt.log");
	ffnet::ProtoBufNervure pbn;
	
	String ip = nc.get<String>("tcp-client.target-svr-ip-addr");
    uint16_t port = nc.get<uint16_t>("tcp-client.target-svr-port");

	ffnet::EndpointPtr_t ep(new ffnet::Endpoint(ffnet::tcp_v4, boost::asio::ip::address_v4::from_string(ip), port));
	pbn.addTCPClient(ep);
	
	pbn.addNeedToRecvPkg<PingPong::Pong>(onRecvPong);
	
	ffnet::event::Event<ffnet::event::tcp_get_connection>::listen(&pbn, onConnSucc);
	ffnet::event::Event<ffnet::event::tcp_lost_connection>::listen
			(&pbn, 
			 boost::bind(onLostConn, _1, &pbn));
			 
	pbn.run();
	
	std::cout<<"Quiting main..."<<std::endl;
    return 0;
}
