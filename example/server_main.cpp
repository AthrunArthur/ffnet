#include <iostream>
#include <network.h>
#include "common/app_global_meta.h"
#include "common/message.h"
#include "log.h"

void onRecvPing(ffnet::PackagePtr_t pPing, ffnet::EndpointPtr_t pEP)
{
	PingMsg & msg = *((PingMsg *)pPing.get());
	
	msg.print();

    boost::this_thread::sleep(boost::posix_time::seconds(1));

    boost::shared_ptr<ffnet::Package> pkg(new PongMsg(1));
	ffnet::NetNervure::send(pkg, pEP);
}
int main(int argc, char **argv) {
	
	initialize_log("svr.log");
    
	
	ffnet::NetNervureFromFile nnff("svr_net_conf.ini");
	nnff.addNeedToRecvPkg(ffnet::PackagePtr_t(new PingMsg()), boost::bind(onRecvPing, _1, _2));
	
	
	nnff.run();
	
	
    return 0;
}
