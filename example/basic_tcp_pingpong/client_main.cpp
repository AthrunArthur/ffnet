#include <iostream>
#include <network.h>
#include "message.h"

using namespace ffnet;

class RawPkgInterpter : public ffnet::tcp_pkg_handler
{
public:
  virtual void handle_pkg(tcp_connection_base *from, const shared_buffer &buf)
  {
    const char * pBuf = buf.buffer();
    uint32_t pkg_id;
    ffnet::deseralize(pBuf, pkg_id);
    if(pkg_id != msg_pong)
      std::cout<<"unknown pkg id :"<<pkg_id<<std::endl;
    else
      std::cout<<"got pong pkg, start deseralize..."<<std::endl;

    PongMsg pm;
    ffnet::marshaler ar(buf.buffer(), buf.length(), ffnet::marshaler::deseralizer);
    pm.arch(ar);
    pm.print();

    boost::shared_ptr<PingMsg> pMsg(new PingMsg("Hello server!"));
    from->send(pMsg);
  }
  virtual bool is_pkg_to_handle(uint32_t pkg_id){
    return true;
  }
};

void    onLostConn(ffnet::tcp_connection_base * pConn, ffnet::net_nervure * pbn)
{
    std::cout<<"Server lost!"<<std::endl;
    pbn->stop();
}

int main(int argc, char **argv) {

  ffnet::Log::init(ffnet::Log::TRACE, "clnt.log");
  RawPkgInterpter rpi;
  ffnet::net_nervure nn;
  nn.add_pkg_handler(&rpi, NULL);
  nn.add_tcp_client("127.0.0.1", 8800);
  nn.get_event_handler()->listen<ffnet::event::tcp_lost_connection>(boost::bind(onLostConn, _1, &nn));
  nn.run();

  std::cout << "Goodbye, world!" << std::endl;
  return 0;
}
