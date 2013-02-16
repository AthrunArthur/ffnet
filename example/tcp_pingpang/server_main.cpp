#include <iostream>
#include <network.h>
#include "message.h"
#include "log.h"

void onRecvPing(boost::shared_ptr<PingMsg> pPing, ffnet::EndpointPtr_t pEP)
{
	
	pPing->print();

    boost::this_thread::sleep(boost::posix_time::seconds(1));

    boost::shared_ptr<ffnet::Package> pkg(new PongMsg(1));
	ffnet::NetNervure::send(pkg, pEP);
}

void onLostTCPConnection(ffnet::TCPConnectionBase * pConn)
{
	std::cout<<"lost connection!"<<std::endl;
}

int main(int argc, char **argv) {
	
   	ffnet::Log::init(ffnet::Log::DEBUG, "svr.log"); 
	
	ffnet::NetNervureFromFile nnff("../svr_net_conf.ini");
	nnff.addNeedToRecvPkg<PingMsg>(onRecvPing);
	ffnet::event::Event<ffnet::event::tcp_lost_connection>::listen(&nnff, onLostTCPConnection);
	
	nnff.run();
	
	
    return 0;
}
