#include <iostream>
#include "network.h"
#include "message.h"


void    sendPingMsg(ffnet::tcp_connection_base * pConn)
{
    char * pContent = new char[16];
    const char *str = "ping world!";
    std::memcpy(pContent, str, std::strlen(str) + 1);
    std::shared_ptr<PingMsg> pMsg(new PingMsg(pContent, std::strlen(str) + 1));

    //ffnet::NetNervure::send(pMsg, pEP);
    pConn->send(pMsg);
    std::cout<<"service running..."<<std::endl;
}

void    onRecvPong(std::shared_ptr<PongMsg>pPong, ffnet::tcp_connection_base* from)
{
    PongMsg & msg =*pPong.get();
    std::cout<<"got pong!"<<std::endl;
    sendPingMsg(from);
}

void    onConnSucc(ffnet::tcp_connection_base *pConn)
{
    std::cout<<"connect success"<<std::endl;
    sendPingMsg(pConn);
}
void    onLostConn(ffnet::tcp_connection_base * pConn, ffnet::net_nervure * pbn)
{
    std::cout<<"Server lost!"<<std::endl;
    pbn->stop();
}

int main(int argc, char **argv) {
    //ffnet::Log::init(ffnet::Log::TRACE, "clnt.log");

    ffnet::typed_pkg_hub pkghub;
    pkghub.tcp_to_recv_pkg<PongMsg>(onRecvPong);

    ffnet::net_nervure nn;
    nn.add_pkg_hub(pkghub);
    nn.add_tcp_client("127.0.0.1", 6891);
    nn.get_event_handler()->listen<ffnet::event::tcp_get_connection>(onConnSucc);
    nn.get_event_handler()->listen<ffnet::event::tcp_lost_connection>(std::bind(onLostConn, std::placeholders::_1, &nn));


    nn.run();


    return 0;
}
