#include <iostream>
#include "network.h"
#include "message.h"
#include<chrono>

class client: public ffnet::routine{
  public:
    client(): ffnet::routine("client"){}

    virtual void initialize(ffnet::net_mode nm, const std::vector<std::string> & args){
      pkghub.tcp_to_recv_pkg<PongMsg>(std::bind(&client::onRecvPong, this, std::placeholders::_1, std::placeholders::_2));

      pnn = new ffnet::net_nervure(nm);
      pnn->add_pkg_hub(pkghub);
      pnn->add_tcp_client("127.0.0.1", 6891);
      pnn->get_event_handler()->listen<ffnet::event::tcp_get_connection>(std::bind(&client::onConnSucc, this, std::placeholders::_1));
      pnn->get_event_handler()->listen<ffnet::event::tcp_lost_connection>(std::bind(&client::onLostConn, this, std::placeholders::_1, pnn));
    }

    virtual void run(){
      pnn->run();
    }

  protected:
    void    sendPingMsg(ffnet::tcp_connection_base * pConn)
    {
      char * pContent = new char[16];
      const char *str = "ping world!";
      std::memcpy(pContent, str, std::strlen(str) + 1);
      std::shared_ptr<PingMsg> pMsg(new PingMsg(pContent, std::strlen(str) + 1));

      pConn->send(pMsg);
      ffnet::mout<<"service running..."<<std::endl;
    }

    void    onRecvPong(std::shared_ptr<PongMsg>pPong, ffnet::tcp_connection_base* from)
    {
      PongMsg & msg =*pPong.get();
      ffnet::mout<<"got pong!"<<std::endl;
      sendPingMsg(from);
    }

    void    onConnSucc(ffnet::tcp_connection_base *pConn)
    {
      ffnet::mout<<"connect success"<<std::endl;
      sendPingMsg(pConn);
    }
    void    onLostConn(ffnet::tcp_connection_base * pConn, ffnet::net_nervure * pbn)
    {
      ffnet::mout<<"Server lost!"<<std::endl;
      pbn->stop();
    }

  protected:
    ffnet::typed_pkg_hub pkghub;
    ffnet::net_nervure * pnn;
};

class server: public ffnet::routine{
  public:
    server(): ffnet::routine("server"){}

    virtual void initialize(ffnet::net_mode nm, const std::vector<std::string> & args){
      pkghub.tcp_to_recv_pkg<PingMsg>(std::bind(&server::onRecvPing, this, std::placeholders::_1, std::placeholders::_2));

      pnn = new ffnet::net_nervure(nm);
      pnn->add_pkg_hub(pkghub);
      pnn->add_tcp_server("127.0.0.1", 6891);
      pnn->get_event_handler()->listen<ffnet::event::tcp_lost_connection>(std::bind(&server::onLostTCPConnection, this, std::placeholders::_1));
    }

    virtual void run(){
      std::thread monitor_thrd(std::bind(&server::press_and_stop, this));
      pnn->run();

      monitor_thrd.join();
    }
  protected:
    void onRecvPing(std::shared_ptr<PingMsg> pPing, ffnet::tcp_connection_base * from)
    {

      pPing->print();

      std::this_thread::sleep_for(std::chrono::seconds(1));

      ffnet::package_ptr pkg(new PongMsg(1));
      from->send(pkg);
    }

    void onLostTCPConnection(ffnet::tcp_connection_base * pConn)
    {
      ffnet::mout<<"lost connection!"<<std::endl;
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

