#include <iostream>
#include <network.h>
#include "common/app_global_meta.h"
#include "common/message.h"
#include "log.h"
#include <boost/serialization/array.hpp>
#include <boost/archive/binary_iarchive.hpp>

void		onRecvPong(ffnet::PackagePtr_t pPong, ffnet::EndpointPtr_t pEP)
{
	PongMsg & msg = *((PongMsg *)pPong.get());
	std::cout<<"got pong!"<<std::endl;
}

int main(int argc, char **argv) {
	
	initialize_log("clnt.log");
    
	ffnet::NetNervureFromFile nnff("clnt_net_conf.ini");
	
	nnff.addNeedToRecvPkg(ffnet::PackagePtr_t(new PongMsg()), boost::bind(&onRecvPong, _1, _2));
	
	
	char * pContent = new char[16];
	const char *str = "ping world!";
	std::memcpy(pContent, str, std::strlen(str) + 1); 
	boost::shared_ptr<PingMsg> pMsg(new PingMsg(pContent, std::strlen(str) + 1));
	ffnet::EndpointPtr_t tpp(new ffnet::Endpoint(ffnet::tcp_v4, boost::asio::ip::address_v4::from_string("127.0.0.1"), 8198));
	ffnet::NetNervure::send(pMsg, tpp);
	
	std::cout<<"service running..."<<std::endl;
	nnff.run();
	
	
    return 0;
}
