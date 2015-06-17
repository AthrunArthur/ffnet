#pragma once

#include "network.h"

enum MsgType {
    msg_ping = 1,
    msg_pong = 2
};


class PingMsg : public ffnet::package
{
public:
    PingMsg()
        : package(msg_ping) {}

    PingMsg(std::string s)
        : package(msg_ping)
	, m_strContent(s){}


    virtual void            archive(ffnet::marshaler &ar)
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

class PongMsg: public ffnet::package
{
public:
    PongMsg()
        : ffnet::package(msg_pong)
        , m_iPongTimes(0) {}

    PongMsg(int times)
        : ffnet::package(msg_pong)
        , m_iPongTimes(times) {}

    virtual void            archive(ffnet::marshaler &ar)
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
