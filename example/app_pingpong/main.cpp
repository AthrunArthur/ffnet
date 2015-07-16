#include <iostream>
#include "network.h"
#include "message.h"

class client: public ffnet::routine{
  public:
    client(): ffnet::routine("client"){}

    virtual void run(ffnet::net_mode nm){
    }
};

class server: public ffnet::routine{
  public:
    server(): ffnet::routine("server"){}
    virtual void run(ffnet::net_mode nm){
    }
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

