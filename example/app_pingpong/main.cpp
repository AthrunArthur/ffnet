#include <iostream>
#include "network.h"
#include "message.h"

class client: public ffnet::routine{
  public:
    client(): ffnet::routine("client"){}

    virtual void initialize(ffnet::net_mode nm, const std::vector<std::string> & args){
      pkghub.tcp_to_recv_pkg<PongMsg>(boost::bind(&client::onRecvPong, this, _1, _2));

      pnn = new ffnet::net_nervure(nm);
      pnn->add_pkg_hub(pkghub);
      pnn->add_tcp_client("127.0.0.1", 6891);
      pnn->get_event_handler()->listen<ffnet::event::tcp_get_connection>(boost::bind(&client::onConnSucc, this, _1));
      pnn->get_event_handler()->listen<ffnet::event::tcp_lost_connection>(boost::bind(&client::onLostConn, this, _1, pnn));
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
      boost::shared_ptr<PingMsg> pMsg(new PingMsg(pContent, std::strlen(str) + 1));

      pConn->send(pMsg);
      std::cout<<"service running..."<<std::endl;
    }

    void    onRecvPong(boost::shared_ptr<PongMsg>pPong, ffnet::tcp_connection_base* from)
    {
      PongMsg & msg =*pPong.get();
      std::cout<<"got pong!"<<std::endl;
      sendPingMsg(from);
    }

    void    onConnSucc(ffnet::tcp_connection_base *pConn)
    {
      std::cout<<"connect success"<<std::endl;
      sendPingMsg(pConn);
    }
    void    onLostConn(ffnet::tcp_connection_base * pConn, ffnet::net_nervure * pbn)
    {
      std::cout<<"Server lost!"<<std::endl;
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
      pkghub.tcp_to_recv_pkg<PingMsg>(boost::bind(&server::onRecvPing, this, _1, _2));

      pnn = new ffnet::net_nervure(nm);
      pnn->add_pkg_hub(pkghub);
      pnn->add_tcp_server("127.0.0.1", 6891);
      pnn->get_event_handler()->listen<ffnet::event::tcp_lost_connection>(boost::bind(&server::onLostTCPConnection, this, _1));
    }

    virtual void run(){
      boost::thread monitor_thrd(boost::bind(&server::press_and_stop, this));
      pnn->run();

      monitor_thrd.join();
    }
  protected:
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

    void  press_and_stop()
    {
      std::cout<<"Press any key to quit..."<<std::endl;
      std::cin.get();
      pnn->stop();
      std::cout<<"Stopping, please wait..."<<std::endl;
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

