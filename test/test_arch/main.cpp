 
#define BOOST_TEST_MODULE TEST_ARCHIVE
#include <boost/test/unit_test.hpp>
#include <network.h>





class PingMsg : public ffnet::Package
{
public:
    PingMsg()
        : Package(1)
        , m_iContentLen(0)
        , m_pContent(NULL) {}

    PingMsg(const char *pContent, int iLen)
        : Package(1)
        , m_iContentLen(iLen)
        , m_pContent(pContent) {}


	virtual void			archive(ffnet::Archive &ar) 
	{
		ar.archive(m_iContentLen);
		if(ar.getArTy()== ffnet::Archive::deseralizer)
			m_pContent = (const char *)malloc(m_iContentLen);
		ar.archive(m_pContent, m_iContentLen);
	}
	void			print()
	{
		std::cout<<"ping: "<<m_iContentLen<<" content:"<<(char *)m_pContent<<std::endl;
	}
public:

    int 			m_iContentLen;
    const char	 		*m_pContent;
};

#define BUF_LEN 1024
BOOST_AUTO_TEST_SUITE(minimal_test)

BOOST_AUTO_TEST_CASE(simple_test)
{
	char buf[BUF_LEN];
	size_t len = BUF_LEN;
	
	const char * p = "hello";
	PingMsg ts(p, strlen(p) + 1);
	ffnet::Archive s(buf, len, ffnet::Archive::seralizer);
	ts.arch(s);
	
	PingMsg ds;
	ffnet::Archive d(buf, len, ffnet::Archive::deseralizer);
	ds.arch(d);
	BOOST_CHECK_EQUAL(ts.m_iContentLen, ds.m_iContentLen);
	BOOST_CHECK_EQUAL(strcmp(ts.m_pContent, ds.m_pContent), 0);
	
}
BOOST_AUTO_TEST_SUITE_END()