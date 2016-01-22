#include <iostream>
#include <network.h>

using namespace ffnet;

class RawPkgInterpter : public RawPkgHandler
{
public:
    virtual void handle_raw_pkg(ASIOConnection * from, const SharedBuffer & buf)
    {
        const char * pBuf = buf.buffer().get();
        size_t len = buf.length();
        from->send(pBuf, len);
    }
};//end class RawPkgHandler
int main(int argc, char *argv[])
{
    Log::init(Log::ERROR, "tcp_svr.log");
    boost::asio::io_service ioservice;
    
    ffnet::EventHandler eh;
    ffnet::NakeBonderSplitter nbs;
    
    
    return 0;
}