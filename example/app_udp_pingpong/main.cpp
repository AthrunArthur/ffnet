#include <iostream>
#include "network.h"
#include "message.h"
#include<chrono>

class client: public ffnet::routine{
  public:
    client(): ffnet::routine("client"){}

    virtual void initialize(ffnet::net_mode nm, const std::vector<std::string> & args){
      pkghub.udp_to_recv_pkg<PongMsg>(std::bind(&client::onRecvPong, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

      pnn = new ffnet::net_nervure(nm);
      pnn->add_pkg_hub(pkghub);
      ffnet::udp_point * p = pnn->add_udp_point("127.0.0.1", 7891);
      ffnet::udp_endpoint ep(asio::ip::address::from_string("127.0.0.1"), 7892);
      asio::deadline_timer dt(pnn->ioservice());
      dt.expires_from_now(boost::posix_time::seconds(1));
      dt.async_wait([this, p, ep](const asio::error_code &ec){
          sendPingMsg(p, ep);
          });
    }

    virtual void run(){
      pnn->run();

    }

  protected:
    void    sendPingMsg(ffnet::udp_point * point, const ffnet::udp_endpoint & tp)
    {
      LOG(INFO)<<"to send ping message";
      ffnet::mout<<"sendPingMsg"<<std::endl;
      char * pContent = new char[16];
      const char *str = "ping world!";
      std::memcpy(pContent, str, std::strlen(str) + 1);
      std::shared_ptr<PingMsg> pMsg(new PingMsg(pContent, std::strlen(str) + 1));

      point->send(pMsg, tp);
      ffnet::mout<<"sent ping "<<std::endl;

      asio::deadline_timer dt(pnn->ioservice());
      dt.expires_from_now(boost::posix_time::seconds(1));
      dt.async_wait([this, point, tp](const asio::error_code & ec){
          sendPingMsg(point, tp);
          });

    }

    void    onRecvPong(std::shared_ptr<PongMsg>pPong, ffnet::udp_point * point, ffnet::udp_endpoint from)
    {
      PongMsg & msg =*pPong.get();
      ffnet::mout<<"got pong!"<<std::endl;
    }

  protected:
    ffnet::typed_pkg_hub pkghub;
    ffnet::net_nervure * pnn;
};

class server: public ffnet::routine{
  public:
    server(): ffnet::routine("server"){}

    virtual void initialize(ffnet::net_mode nm, const std::vector<std::string> & args){
      pkghub.udp_to_recv_pkg<PingMsg>(std::bind(&server::onRecvPing, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

      pnn = new ffnet::net_nervure(nm);
      pnn->add_pkg_hub(pkghub);
      pnn->add_udp_point("127.0.0.1", 7892);
    }

    virtual void run(){
      std::thread monitor_thrd(std::bind(&server::press_and_stop, this));
      pnn->run();

      monitor_thrd.join();
    }
  protected:
    void onRecvPing(std::shared_ptr<PingMsg> pPing, ffnet::udp_point * point, ffnet::udp_endpoint from)
    {
      pPing->print();
      LOG(INFO)<<"recv ping";
      ffnet::package_ptr pkg(new PongMsg(1));
      point->send(pkg, from);
    }

    void  press_and_stop()
    {
      ffnet::mout<<"Press any key to quit..."<<std::endl;
      std::cin.get();
      pnn->stop();
      ffnet::mout<<"Stopping, please wait..."<<std::endl;
    }
  protected:
    ffnet::typed_pkg_hub pkghub;
    ffnet::net_nervure * pnn;
};

int main(int argc, char *argv[])
{
  ffnet::application app("pingpong");
  app.initialize(argc, argv);

  client c;
  server s;
  app.register_routine(&c);
  app.register_routine(&s);
  app.run();
  return 0;
}

