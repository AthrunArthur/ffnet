#include <iostream>
#include <network.h>
#include "message.pb.h"
void onRecvPing(boost::shared_ptr<PingPong::Ping> pPing, ffnet::udp_point * point, ffnet::udp_endpoint ep)
{
    std::cout<<pPing->msg()<<std::endl;

    boost::this_thread::sleep(boost::posix_time::seconds(1));

    boost::shared_ptr<PingPong::Pong> pkg(new PingPong::Pong());
    pkg->set_msg("Pong from server!");
    pkg->set_id(0);
    ffnet::send_message(point, ep, pkg);
}

int main(int argc, char **argv) {
    ffnet::Log::init(ffnet::Log::TRACE, "svr.log");
    
    ffnet::protobuf_pkg_hub pkghub;
    pkghub.udp_to_recv_pkg<PingPong::Ping>(onRecvPing);
    
    ffnet::net_nervure nn;
    nn.add_pkg_hub(pkghub);
    nn.add_udp_point("127.0.0.1", 7892);
    nn.run();
    
    return 0;
}
