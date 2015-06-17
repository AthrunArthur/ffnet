#include <iostream>
#include <network.h>
#include "message.h"


void onRecvPing(boost::shared_ptr<PingMsg> pPing, ffnet::tcp_connection_base * from)
{
    
    pPing->print();

    boost::this_thread::sleep(boost::posix_time::seconds(1));

    ffnet::package_ptr pkg(new PongMsg(1));
    from->send(pkg);
}

void onLostTCPConnection(ffnet::tcp_connection_base * pConn)
{
    std::cout<<"lost connection!"<<std::endl;
}

void  press_and_stop(ffnet::net_nervure & nn)
{
    std::cout<<"Press any key to quit..."<<std::endl;
    std::cin.get();
    nn.stop();
    std::cout<<"Stopping, please wait..."<<std::endl;
}

int main(int argc, char **argv) {
    
    ffnet::Log::init(ffnet::Log::TRACE, "svr.log"); 

    ffnet::typed_pkg_hub pkghub;
    pkghub.tcp_to_recv_pkg<PingMsg>(onRecvPing);

    ffnet::net_nervure nn;
    nn.add_pkg_hub(pkghub);
    nn.add_tcp_server("127.0.0.1", 6891);
    nn.get_event_handler()->listen<ffnet::event::tcp_lost_connection>(onLostTCPConnection);
    boost::thread monitor_thrd(boost::bind(press_and_stop, boost::ref(nn)));
    nn.run();
    
    monitor_thrd.join();
    return 0;
}
