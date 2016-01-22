#include "common/archive.h"

namespace ffnet
{
    marshaler::marshaler(const char *buf,size_t len, marshaler_type at)
: m_pReadBuf(NULL)
, m_iBufLen(len)
, m_iAT(at)
, m_iBase(0)
, m_pWriteBuf(NULL)
{
    if(m_iAT == deseralizer)
    {
        m_pReadBuf = buf;
    }
    else if(m_iAT == seralizer)
    {
        assert(0);
    }
}

    marshaler::marshaler(char *buf, size_t len, marshaler_type at)
: m_pReadBuf(NULL)
, m_iBufLen(len)
, m_iAT(at)
, m_iBase(0)
, m_pWriteBuf(buf)
{
    if(m_iAT == deseralizer)
    {
        m_pReadBuf = buf;
    }
    else if(m_iAT == seralizer)
    {
        m_pWriteBuf = buf;
    }
}

    marshaler::marshaler(marshaler_type at)
: m_pReadBuf(NULL)
, m_iBufLen(0)
, m_iAT(at)
, m_iBase(0)
, m_pWriteBuf(NULL)
{
    assert(m_iAT == length_retriver);
}

void marshaler::archive(String & s)
{
    size_t len = s.size();
    switch(get_marshaler_type())
    {
    case seralizer:
        len = s.size();
        std::memcpy(m_pWriteBuf + m_iBase, (const char *) & len, sizeof(size_t));
        m_iBase += sizeof(size_t);
        std::memcpy(m_pWriteBuf + m_iBase, s.c_str(), len);
        m_iBase += len;
        break;
    case deseralizer:
        std::memcpy((char *)&len, m_pReadBuf + m_iBase, sizeof(size_t));
        m_iBase += sizeof(size_t);
        s = String(len, 0);
        std::memcpy(const_cast<char *>(s.c_str()), m_pReadBuf + m_iBase, len);
        m_iBase += len;
        break;
    case length_retriver:
        m_iBase += (sizeof(size_t) + s.size());
        break;
    }
}


}//end namespace ffnet
