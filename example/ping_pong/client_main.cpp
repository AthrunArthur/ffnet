#include <iostream>
#include "network.h"
#include "message.h"
#include "log.h"

void		onRecvPong(boost::shared_ptr<PongMsg> pPong, ffnet::EndpointPtr_t  pEP)
{
	PongMsg & msg = *((PongMsg *)pPong.get());
	std::cout<<"got pong!"<<std::endl;
}

int main(int argc, char **argv) {
	
	initialize_log("clnt.log");
    
	ffnet::NetNervureFromFile nnff("/home/athrun/projects/ffnet.git/example/ping_pong/clnt_net_conf.ini");
	
	nnff.addNeedToRecvPkg<PongMsg>(onRecvPong);
	//nnff.addNeedToRecvPkg<PongMsg>(onRecvPong);
	
	
	int8_t * pContent = new int8_t[16];
	const char *str = "ping world!";
	std::memcpy(pContent, str, std::strlen(str) + 1); 
	boost::shared_ptr<PingMsg> pMsg(new PingMsg(pContent, std::strlen(str) + 1));
	ffnet::EndpointPtr_t tpp(new ffnet::Endpoint(ffnet::tcp_v4, boost::asio::ip::address_v4::from_string("127.0.0.1"), 8198));
	ffnet::NetNervure::send(pMsg, tpp);
	
	std::cout<<"service running..."<<std::endl;
	nnff.run();
	
	
    return 0;
}
