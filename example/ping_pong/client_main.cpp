#include <iostream>
#include <network.h>
#include "message.h"
#include "log.h"
#include <boost/serialization/array.hpp>
#include <boost/archive/binary_iarchive.hpp>

void	sendPingMsg()
{
	char * pContent = new char[16];
	const char *str = "ping world!";
	std::memcpy(pContent, str, std::strlen(str) + 1); 
	boost::shared_ptr<PingMsg> pMsg(new PingMsg(pContent, std::strlen(str) + 1));
	ffnet::EndpointPtr_t tpp(new ffnet::Endpoint(ffnet::tcp_v4, boost::asio::ip::address_v4::from_string("127.0.0.1"), 39));
	ffnet::NetNervure::send(pMsg, tpp);
	
	std::cout<<"service running..."<<std::endl;
}

void	onRecvPong(ffnet::PackagePtr_t pPong, ffnet::EndpointPtr_t pEP)
{
	PongMsg & msg = *((PongMsg *)pPong.get());
	std::cout<<"got pong!"<<std::endl;
	sendPingMsg();
}

int main(int argc, char **argv) {
	
	initialize_log("clnt.log");
    
	ffnet::NetNervureFromFile nnff("clnt_net_conf.ini");
	
	nnff.addNeedToRecvPkg<PongMsg>(onRecvPong);
	sendPingMsg();
	
	
	nnff.run();
	
	
    return 0;
}
