#include <iostream>
#include <network.h>
#include <middleware/length_bonder_splitter.h>
#include <time.h>
#include <cstdlib>
#include <list>

#define BOOST_TEST_MODULE pkg_test
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using namespace boost::unit_test;

const static size_t small_pkg_num = 13;
enum{
    pkg_small = 5,
    pkg_large,
};

class SmallPkg : public ffnet::Package
{
public:
    SmallPkg() : Package(pkg_small){};
    
    size_t &      num(){return m_iNum;}
    
    virtual void        archive(ffnet::Archive & ar)
    {
        ar.archive(m_iNum);
    }
protected:
    size_t      m_iNum;
};//end class SamllPkg

class LargePkg : public ffnet::Package
{
public:
    LargePkg() 
    : Package(pkg_large)
    , m_iLen(0)
    , m_pBuf(NULL){ 
    };
    
    void init(size_t n)
    {
        m_iLen = n;
        m_pBuf = new int[m_iLen];
        for(int i =0 ; i < m_iLen; ++i)
        {
            m_pBuf[i] = i;
        }
    }
    
    virtual void archive(ffnet::Archive & ar)
    {
        ar.archive(m_pBuf, m_iLen);
    }
    
    size_t len() const {return m_iLen;}
    int   data(int i )const {return m_pBuf[i];}
    
protected:
    int *   m_pBuf;
    size_t  m_iLen;
};//end class LargePkg

//void test_small()
BOOST_AUTO_TEST_CASE(small_pkg)
{
    ffnet::details::NetBuffer serbuf;
    ffnet::details::NetBuffer recvbuf;
    
    ffnet::LengthBonderSplitter lbs;
    const int total_pkg_num = 1000;
    for(int i = 0; i < total_pkg_num; ++i)
    {
        boost::shared_ptr<SmallPkg> p(new SmallPkg());
        p->num() = small_pkg_num;
        lbs.bond(serbuf, p);
    }
    
    int ser_size = serbuf.filled();
    int done_size = 0;
    
    int recv_pkg_num = 0;
    while(done_size < ser_size)
    {
        char tmp_buf[1024];
        int step_size = 0;
        if(ser_size - done_size < 256)
            step_size = ser_size - done_size;
        else{
            step_size = rand() % (256-100) + 100;
        }
        done_size += step_size;
        serbuf.readBuffer(tmp_buf, step_size);
        serbuf.eraseBuffer(step_size);
        recvbuf.writeBuffer(tmp_buf, step_size);
        std::list<ffnet::SharedBuffer> sbs = lbs.split(recvbuf);
        //std::list<ffnet::SharedBuffer> sbs = lbs.split(serbuf);
        //done_size = ser_size;
        for(std::list<ffnet::SharedBuffer>::iterator it = sbs.begin(); it != sbs.end(); ++it)
        {
            ffnet::SharedBuffer sb = *it;
            uint32_t iTypeID;
            ffnet::archive::deseralize(sb.buffer().get(), iTypeID);
            BOOST_CHECK_MESSAGE(iTypeID == pkg_small, "i_type_id:"<<iTypeID);
            ffnet::Archive d(const_cast<const char *>(sb.buffer().get()),
                             sb.length(), ffnet::Archive::deseralizer);
            SmallPkg * pPkg = new SmallPkg();
            pPkg->arch(d);
            BOOST_CHECK_MESSAGE(pPkg->num() == small_pkg_num, "num :"<<pPkg->num());
            recv_pkg_num ++;
            delete pPkg;
        }
    }
    BOOST_CHECK_MESSAGE(recv_pkg_num == total_pkg_num, "recv_pkg_num: "<<recv_pkg_num);
}

const static int large_array_size = 1000;

BOOST_AUTO_TEST_CASE(large_pkg)
{
    ffnet::details::NetBuffer serbuf;
    ffnet::details::NetBuffer recvbuf;
    
    ffnet::LengthBonderSplitter lbs;
    const int total_pkg_num = 1000;
    for(int i = 0; i < total_pkg_num; ++i)
    {
        boost::shared_ptr<LargePkg> p(new LargePkg());
        p->init(large_array_size);
        lbs.bond(serbuf, p);
    }
    
    int ser_size = serbuf.filled();
    int done_size = 0;
    
    int recv_pkg_num = 0;
    while(done_size < ser_size)
    {
        char tmp_buf[1024];
        int step_size = 0;
        if(ser_size - done_size < 256)
            step_size = ser_size - done_size;
        else{
            step_size = rand() % (256-100) + 100;
        }
        done_size += step_size;
        serbuf.readBuffer(tmp_buf, step_size);
        serbuf.eraseBuffer(step_size);
        recvbuf.writeBuffer(tmp_buf, step_size);
        std::list<ffnet::SharedBuffer> sbs = lbs.split(recvbuf);
        //std::list<ffnet::SharedBuffer> sbs = lbs.split(serbuf);
        //done_size = ser_size;
        for(std::list<ffnet::SharedBuffer>::iterator it = sbs.begin(); it != sbs.end(); ++it)
        {
            ffnet::SharedBuffer sb = *it;
            uint32_t iTypeID;
            ffnet::archive::deseralize(sb.buffer().get(), iTypeID);
            BOOST_CHECK_MESSAGE(iTypeID == pkg_large, "i_type_id:"<<iTypeID);
            ffnet::Archive d(const_cast<const char *>(sb.buffer().get()),
                             sb.length(), ffnet::Archive::deseralizer);
            LargePkg * pPkg = new LargePkg();
            pPkg->arch(d);
            for(int i = 0; i < pPkg->len(); ++i)
            {
                BOOST_CHECK_MESSAGE(pPkg->data(i) == i, "num "<<i<<" wrong!");
            }
            recv_pkg_num ++;
            delete pPkg;
        }
    }
    BOOST_CHECK_MESSAGE(recv_pkg_num == total_pkg_num, "recv_pkg_num: "<<recv_pkg_num);
}