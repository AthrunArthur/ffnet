#include <iostream>
#include <network.h>
#include "message.h"


void onRecvPing(boost::shared_ptr<PingMsg> pPing, ffnet::TCPConnectionBase * from)
{
    
    pPing->print();

    boost::this_thread::sleep(boost::posix_time::seconds(1));

    boost::shared_ptr<ffnet::Package> pkg(new PongMsg(1));
    from->send(pkg);
}

void onLostTCPConnection(ffnet::EndpointPtr_t pEP)
{
    std::cout<<"lost connection!"<<std::endl;
}

void  press_and_stop(ffnet::NetNervureFromFile& nnff)
{
    std::cout<<"Press any key to quit..."<<std::endl;
    std::cin.get();
    nnff.stop();
    std::cout<<"Stopping, please wait..."<<std::endl;
}

int main(int argc, char **argv) {
    
    ffnet::Log::init(ffnet::Log::TRACE, "svr.log"); 
    
    ffnet::NetNervureFromFile nnff("../svr_net_conf.ini");
    nnff.tcpToRecvPkg<PingMsg>(onRecvPing);
    nnff.getEventHandler()->listen<ffnet::event::tcp_lost_connection>(onLostTCPConnection);
    boost::thread monitor_thrd(boost::bind(press_and_stop, boost::ref(nnff)));
    nnff.run();
    
    monitor_thrd.join();
    return 0;
}
