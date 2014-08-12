#include <iostream>
#include "network.h"
#include "message.h"

ffnet::NetNervureFromFile nnff("../clnt_net_conf.ini");

void    sendPingMsg(ffnet::EndpointPtr_t pEP)
{
    char * pContent = new char[16];
    const char *str = "ping world!";
    std::memcpy(pContent, str, std::strlen(str) + 1); 
    boost::shared_ptr<PingMsg> pMsg(new PingMsg(pContent, std::strlen(str) + 1));

    ffnet::NetNervure::send(pMsg, pEP);
    
    std::cout<<"service running..."<<std::endl;
}

void    onRecvPong(boost::shared_ptr<PongMsg>pPong, ffnet::EndpointPtr_t pEP)
{
    PongMsg & msg =*pPong.get();
    std::cout<<"got pong!"<<std::endl;
    sendPingMsg(pEP);
}

void    onConnSucc(ffnet::TCPConnectionBase *pConn)
{
    std::cout<<"connect success"<<std::endl;
    ffnet::EndpointPtr_t tpp = pConn->getRemoteEndpointPtr();
    sendPingMsg(tpp);
}
void    onLostConn(ffnet::EndpointPtr_t pEP, ffnet::NetNervure * pbn)
{
    std::cout<<"Server lost!"<<std::endl;
    pbn->stop();
}

int main(int argc, char **argv) {
       ffnet::Log::init(ffnet::Log::TRACE, "clnt.log"); 
    nnff.addNeedToRecvPkg<PongMsg>(onRecvPong);
    ffnet::event::Event<ffnet::event::tcp_get_connection>::listen(&nnff, onConnSucc);
    ffnet::event::Event<ffnet::event::tcp_lost_connection>::listen
            (&nnff, 
             boost::bind(onLostConn, _1, &nnff));
            
    nnff.run();
    
    
    return 0;
}
