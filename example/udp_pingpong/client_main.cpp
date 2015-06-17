#include <iostream>
#include "network.h"
#include "message.pb.h"


void    sendPingMsg(ffnet::udp_point * point, const ffnet::udp_endpoint & tp)
{
    boost::shared_ptr<PingPong::Ping> pMsg(new PingPong::Ping());
    pMsg->set_msg("ping from client");
    pMsg->set_id(1);
    ffnet::send_message(point, tp, pMsg);

    std::cout<<"send a message, service running..."<<std::endl;
}

void    onRecvPong(boost::shared_ptr<PingPong::Pong>  pPong, ffnet::udp_point * point, ffnet::udp_endpoint ep)
{
    std::cout<<"got pong: "<<pPong->msg()<<std::endl;
    sendPingMsg(point, ep);
}

int main(int argc, char **argv) {
    
    ffnet::Log::init(ffnet::Log::TRACE, "clnt.log");
    
    ffnet::protobuf_pkg_hub pkghub;
    pkghub.udp_to_recv_pkg<PingPong::Pong>(onRecvPong);
    
    ffnet::net_nervure nn;
    nn.add_pkg_hub(pkghub);
    ffnet::udp_point * point = nn.add_udp_point("127.0.0.1", 7891);
    ffnet::udp_endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 7892);
    
    sendPingMsg(point, ep);
    
    nn.run();
    
    std::cout<<"Quiting main..."<<std::endl;
    return 0;
}
