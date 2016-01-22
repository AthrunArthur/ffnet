#include <iostream>
#include "network.h"
#include "message.h"


ffnet::NetNervureFromFile<ffnet::ThreadNervure> nnff("../clnt_net_conf.ini");

void    sendPingMsg(ffnet::TCPConnectionBase * pConn)
{
    char * pContent = new char[16];
    const char *str = "ping world!";
    std::memcpy(pContent, str, std::strlen(str) + 1); 
    boost::shared_ptr<PingMsg> pMsg(new PingMsg(pContent, std::strlen(str) + 1));

    //ffnet::NetNervure::send(pMsg, pEP);
    pConn->send(pMsg);
    std::cout<<"service running..."<<std::endl;
}

void    onRecvPong(  boost::shared_ptr<PongMsg>pPong, ffnet::EndpointPtr_t from)
{
    PongMsg & msg =*pPong.get();
    std::cout<<"got pong!"<<std::endl;
    //sendPingMsg(from);
    char * pContent = new char[16];
    const char *str = "ping world!";
    std::memcpy(pContent, str, std::strlen(str) + 1); 
    boost::shared_ptr<PingMsg> pMsg(new PingMsg(pContent, std::strlen(str) + 1));
    nnff.send(pMsg, from);
}
void    onConnSucc(ffnet::TCPConnectionBase *pConn)
{
    std::cout<<"connect success"<<std::endl;
    sendPingMsg(pConn);
}
void    onLostConn(ffnet::EndpointPtr_t pEP, ffnet::NetNervure * pbn)
{
    std::cout<<"Server lost!"<<std::endl;
    pbn->stop();
}

int main(int argc, char **argv) {
       ffnet::Log::init(ffnet::Log::TRACE, "clnt.log"); 
    //nnff.tcpToRecvPkg<PongMsg>(onRecvPong);
       nnff.addNeedToRecvPkg<PongMsg>(onRecvPong);
    nnff.getEventHandler()->listen<ffnet::event::tcp_get_connection>(onConnSucc);
    nnff.getEventHandler()->listen<ffnet::event::tcp_lost_connection>(boost::bind(onLostConn, _1, &nnff));
    
            
    nnff.run();
    
    
    return 0;
}
