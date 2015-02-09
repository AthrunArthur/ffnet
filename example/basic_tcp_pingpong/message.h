#ifndef TEST_NETWORK_COMMON_MESSAGE_H_
#define TEST_NETWORK_COMMON_MESSAGE_H_

#include "network.h"

enum MsgType {
    msg_ping = 1,
    msg_pong = 2
};


class PingMsg : public ffnet::Package
{
public:
    PingMsg()
        : Package(msg_ping) {}

    PingMsg(std::string s)
        : Package(msg_ping)
	, m_strContent(s){}


    virtual void            archive(ffnet::Archive &ar) 
    {
        ar.archive(m_strContent);
    }
    void            print()
    {
        std::cout<<"ping: "<<m_strContent<<std::endl;
    }
protected:

    std::string m_strContent;
};

class PongMsg: public ffnet::Package
{
public:
    PongMsg()
        : ffnet::Package(msg_pong)
        , m_iPongTimes(0) {}

    PongMsg(int times)
        : ffnet::Package(msg_pong)
        , m_iPongTimes(times) {}

    virtual void            archive(ffnet::Archive &ar) 
    {
        ar.archive(m_iPongTimes);
    }
    void print()
    {
      std::cout<<"pong: "<<m_iPongTimes<<std::endl;
    }
protected:
    int            m_iPongTimes;
};
#endif
