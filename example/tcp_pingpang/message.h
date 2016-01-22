#ifndef TEST_NETWORK_COMMON_MESSAGE_H_
#define TEST_NETWORK_COMMON_MESSAGE_H_

#include "network.h"

enum MsgType {
    msg_ping = 1,
    msg_pong = 2
};


class PingMsg : public ffnet::package
{
public:
    PingMsg()
        : package(msg_ping)
        , m_iContentLen(0)
        , m_pContent(NULL) {}

    PingMsg(char *pContent, int iLen)
        : package(msg_ping)
        , m_iContentLen(iLen)
        , m_pContent(pContent) {}


    virtual void            archive(ffnet::marshaler &ar)
    {
        ar.archive(m_iContentLen);
        if(ar.get_marshaler_type()== ffnet::marshaler::deseralizer)
            m_pContent = (char *)malloc(m_iContentLen);
        ar.archive(m_pContent, m_iContentLen);
    }
    void            print()
    {
        std::cout<<"ping: "<<m_iContentLen<<" content:"<<(char *)m_pContent<<std::endl;
    }
protected:

    int             m_iContentLen;
    char             *m_pContent;
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
protected:
    int            m_iPongTimes;
};
#endif
