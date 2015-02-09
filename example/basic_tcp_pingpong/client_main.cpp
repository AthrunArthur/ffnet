#include <iostream>
#include <network.h>
#include "message.h"

using namespace ffnet;

class RawPkgInterpter : public ffnet::RawPkgHandler
{
public:
  virtual void handle_raw_pkg(ffnet::ASIOConnection * from, const SharedBuffer & buf)
  {
    const char * pBuf = buf.buffer().get();
    uint32_t pkg_id;
    ffnet::archive::deseralize(pBuf, pkg_id);
    if(pkg_id != msg_pong)
      std::cout<<"unknown pkg id :"<<pkg_id<<std::endl;
    else
      std::cout<<"got pong pkg, start deseralize..."<<std::endl;
    
    PongMsg pm;
    ffnet::Archive ar(buf.buffer().get(), buf.length(), ffnet::Archive::deseralizer);
    pm.arch(ar);
    pm.print();
    
    boost::shared_ptr<PingMsg> pMsg(new PingMsg("Hello server!"));
    from->send(pMsg);
  }
};

int main(int argc, char **argv) {
  
  ffnet::Log::init(ffnet::Log::TRACE, "clnt.log");
  boost::asio::io_service ioservice;
  ffnet::EventHandler eh;
  ffnet::LengthBonderSplitter lbs;
  RawPkgInterpter rpi;
  //ffnet::TCPServer server(ioservice, &lbs, &eh, &rpi, "127.0.0.1", 8800);
  boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::from_string("127.0.0.1"), 8800);
  boost::shared_ptr<ffnet::TCPClient> pclient(new TCPClient(ioservice, &lbs, &eh, &rpi, ep));
  //ffnet::TCPClient client(ioservice, &lbs, &eh, &rpi, ep);
			  
  
  ioservice.run();
  
    std::cout << "Goodbye, world!" << std::endl;
    return 0;
}
