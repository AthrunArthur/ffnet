#include <iostream>
#include <asio.hpp>
#include <network.h>



int main(int argc, char *argv[]){
  asio::io_service ioservice;
  asio::deadline_timer dt(ioservice);
  dt.expires_from_now(boost::posix_time::seconds(1));
  std::cout<<"go cross expires_from_now"<<std::endl;
  dt.async_wait([](const asio::error_code& error){
      std::cout<<"timer run"<<std::endl;
      });
  ioservice.run();
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  return 0;
}
