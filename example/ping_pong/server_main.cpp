#include <iostream>
#include <network.h>
#include "message.h"
#include "log.h"

void onRecvPing(boost::shared_ptr<PingMsg> pPing, ffnet::EndpointPtr_t pEP)
{
	PingMsg & msg = *((PingMsg *)pPing.get());
	
	msg.print();

    boost::this_thread::sleep(boost::posix_time::seconds(1));

    boost::shared_ptr<ffnet::Package> pkg(new PongMsg(1));
	ffnet::NetNervure::send(pkg, pEP);
}
int main(int argc, char **argv) {
	
	initialize_log("svr.log");
    
	
	ffnet::NetNervureFromFile nnff("../svr_net_conf.ini");
	nnff.addNeedToRecvPkg<PingMsg>(onRecvPing);
	
	
	nnff.run();
	
	
    return 0;
}
