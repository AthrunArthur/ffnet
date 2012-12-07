#include <iostream>
#include <network.h>
#include "message.pb.h"
#include <log.h>
void onRecvPing(boost::shared_ptr<PingPong::Ping> pPing, ffnet::EndpointPtr_t pEP)
{
	PingPong::Ping & msg = *(pPing.get());
	
	//msg.print();
	std::cout<<msg.msg()<<std::endl;

    boost::this_thread::sleep(boost::posix_time::seconds(1));

    boost::shared_ptr<PingPong::Pong> pkg(new PingPong::Pong());
	pkg->set_msg("Pong from server!");
	pkg->set_id(0);
	ffnet::NetNervure::send(pkg, pEP);
}
void onLostTCPConnection(ffnet::Endpoint remote, ffnet::Endpoint local)
{
	std::cout<<"lost connection!"<<std::endl;
}
int main(int argc, char **argv) {
	initialize_log("svr.log");
	
	ffnet::ProtoBufNervure pbn;
	ffnet::event::Event<ffnet::event::tcp_lost_connection>::listen(onLostTCPConnection);
	
	ffnet::NervureConfigure nc("/home/athrun/projects/ffnet.git/example/pb_pingpong/pb_pingpong/svr_net_conf.ini");
    uint16_t port = nc.get<uint16_t>("tcp-server.port");
	pbn.initTCPServer(port);
	
	
	pbn.addNeedToRecvPkg<PingPong::Ping>(onRecvPing);
	pbn.run();
	
	
    return 0;
}
