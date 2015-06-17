#include <iostream>
#include <network.h>
#include "message.h"
using namespace ffnet;
int g_iValue = 0;

class RawPkgInterpter : public ffnet::tcp_pkg_handler
{
public:
  virtual void handle_pkg(ffnet::tcp_connection_base *from, const shared_buffer &buf)
  {
      LOG_DEBUG(app)<<"enter handle_raw_pkg";
    const char * pBuf = buf.buffer();
    uint32_t pkg_id;
    ffnet::deseralize(pBuf, pkg_id);
    LOG_DEBUG(app)<<"handle_raw_pkg pkg id is "<<pkg_id;
    if(pkg_id != msg_ping)
      std::cout<<"unknown pkg id :"<<pkg_id<<std::endl;
    else
      std::cout<<"got ping pkg, start deseralize..."<<std::endl;

    PingMsg pm;
    ffnet::marshaler ar(buf.buffer(), buf.length(), ffnet::marshaler::deseralizer);
    pm.arch(ar);
    pm.print();
    boost::shared_ptr<PongMsg> pMsg(new PongMsg(g_iValue));
    from->send(pMsg);
  }

  virtual bool is_pkg_to_handle(uint32_t pkg_id){
    return true;
  }
};


void get_connection(ffnet::tcp_connection_base * pConn)
{
  std::cout<<"****get connection from "<<pConn->remote_endpoint().address().to_string()<<std::endl;
  boost::shared_ptr<PongMsg> pMsg(new PongMsg(g_iValue));
  pConn->send(pMsg);
  std::cout<<"****pong msg send with value :"<<g_iValue<<std::endl;
  g_iValue ++;

}
int main(int argc, char **argv) {
  ffnet::Log::init(ffnet::Log::TRACE, "svr.log");

  RawPkgInterpter rpi;

  ffnet::net_nervure nn;
  nn.add_pkg_handler(&rpi, NULL);
  nn.add_tcp_server("127.0.0.1", 8800);
  nn.get_event_handler()->listen<ffnet::event::tcp_get_connection>(get_connection);
  nn.run();


    std::cout << "Goodbye, world!" << std::endl;
    return 0;
}
