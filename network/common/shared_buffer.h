#pragma once
#include "common.h"
namespace ffnet
{
class shared_buffer {

public:
    shared_buffer();
    shared_buffer(const char * pBuf, size_t len);

    inline const char * buffer() const {return m_pBuffer;}
    inline size_t &    length(){return m_iLength;};
    inline const size_t & length() const{return m_iLength;}

protected:
    const char *    m_pBuffer;
    size_t      m_iLength;

};
}//end namespace ffnet
