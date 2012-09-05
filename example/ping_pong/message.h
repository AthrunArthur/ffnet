#ifndef TEST_NETWORK_COMMON_MESSAGE_H_
#define TEST_NETWORK_COMMON_MESSAGE_H_

#include "network.h"
#include "log.h"

enum MsgType {
    msg_ping = 1,
    msg_pong = 2
};


class PingMsg : public ffnet::Package
{
protected:
	ARCHIVE_PACKAGE(PingMsg);
public:
    PingMsg()
        : Package(msg_ping)
        , m_iContentLen(0)
        , m_pContent(NULL) {}

    PingMsg(char *pContent, int iLen)
        : Package(msg_ping)
        , m_iContentLen(iLen)
        , m_pContent(pContent) {}


	
	template <class Ar_t>
	void			archive(Ar_t &ar) 
	{
		ar.archive(m_pContent, m_iContentLen);
	}
	void			print()
	{
		std::cout<<"ping: "<<m_iContentLen<<" content:"<<m_pContent<<std::endl;
	}
protected:

    int 			m_iContentLen;
    char 		*m_pContent;
};

class PongMsg: public ffnet::Package
{
protected:
	ARCHIVE_PACKAGE(PongMsg);
public:
    PongMsg()
        : ffnet::Package(msg_pong)
        , m_iPongTimes(0) {}

    PongMsg(int times)
        : ffnet::Package(msg_pong)
        , m_iPongTimes(times) {}


	template <class Ar_t>
	void			archive(Ar_t &ar) 
	{
		ar.archive(m_iPongTimes);
	}
protected:
    int			m_iPongTimes;
};
#endif
