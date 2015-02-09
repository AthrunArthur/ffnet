#include <iostream>
#include <network.h>
#include "message.h"
using namespace ffnet;
int g_iValue = 0;

class RawPkgInterpter : public ffnet::RawPkgHandler
{
public:
  virtual void handle_raw_pkg(ASIOConnection * from, const SharedBuffer & buf)
  {
      LOG_DEBUG(app)<<"enter handle_raw_pkg";
    const char * pBuf = buf.buffer().get();
    uint32_t pkg_id;
    ffnet::archive::deseralize(pBuf, pkg_id);
    LOG_DEBUG(app)<<"handle_raw_pkg pkg id is "<<pkg_id;
    if(pkg_id != msg_ping)
      std::cout<<"unknown pkg id :"<<pkg_id<<std::endl;
    else
      std::cout<<"got ping pkg, start deseralize..."<<std::endl;
    
    PingMsg pm;
    ffnet::Archive ar(buf.buffer().get(), buf.length(), ffnet::Archive::deseralizer);
    pm.arch(ar);
    pm.print();
    boost::shared_ptr<PongMsg> pMsg(new PongMsg(g_iValue));
    from->send(pMsg);
  }
  
};


void get_connection(boost::shared_ptr<TCPConnection> pConn)
{
  std::cout<<"****get connection from "<<pConn->getRemoteEndpointPtr()->to_str()<<std::endl;
  boost::shared_ptr<PongMsg> pMsg(new PongMsg(g_iValue));
  pConn->send(pMsg);
  std::cout<<"****pong msg send with value :"<<g_iValue<<std::endl;
  g_iValue ++;
  
}
int main(int argc, char **argv) {
  ffnet::Log::init(ffnet::Log::TRACE, "svr.log");
  boost::asio::io_service ioservice;
  ffnet::EventHandler eh;
  eh.listen<ffnet::event::more::tcp_server_accept_connection>(get_connection);
  ffnet::LengthBonderSplitter lbs;
  RawPkgInterpter rpi;
  boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 8800);
  boost::shared_ptr<ffnet::TCPServer> pserver(new ffnet::TCPServer(ioservice, &lbs, &eh, &rpi, ep));
  //ffnet::TCPServer server(ioservice, &lbs, &eh, &rpi, "127.0.0.1", 8800);
  
  ioservice.run();
  
    std::cout << "Goodbye, world!" << std::endl;
    return 0;
}
