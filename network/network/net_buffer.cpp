#include "network/net_buffer.h"
#include <sstream>
namespace ffnet
{

net_buffer::net_buffer(int iInitSize)
    : m_oBuffer(iInitSize)
    , m_iToWriteBufIndex(0)
, m_iToReadBufIndex(0){}

void net_buffer::write_buffer(const char *pBuf, size_t len)
{
    if(len >= idle())
        m_oBuffer.resize(m_iToWriteBufIndex + len + 64);
    memcpy(&m_oBuffer[m_iToWriteBufIndex], pBuf, len);
    m_iToWriteBufIndex += len;
}

size_t net_buffer::read_buffer(char *pBuf, size_t len)
{
    if(filled() < len)
        len = filled();
    if(len == 0)
        return 0;
    memcpy(pBuf, &m_oBuffer[0], len);
    return len;
}

void net_buffer::erase_buffer(size_t len)
{
    if(len == 0)
        return;
    if(len >= m_iToWriteBufIndex)
        len = m_iToWriteBufIndex;
    m_oBuffer.erase(m_oBuffer.begin(), m_oBuffer.begin() + len);
    m_iToReadBufIndex += len;
    if(m_iToReadBufIndex == m_iToWriteBufIndex)
    {
        m_iToReadBufIndex = 0;
        m_iToWriteBufIndex = 0;
    }
}

asio::const_buffer net_buffer::readable() const
{
    return asio::const_buffer(m_oBuffer.data() + m_iToReadBufIndex, m_iToWriteBufIndex);
}
asio::mutable_buffer net_buffer::writeable()
{
    if(idle() < BUFFER_INC_STEP)
        m_oBuffer.resize(size() + BUFFER_INC_STEP);
    return asio::mutable_buffer(m_oBuffer.data() + m_iToWriteBufIndex, idle());
}

void net_buffer::append_buffer(asio::const_buffer buf)
{
    write_buffer(asio::buffer_cast<const char *>(buf), asio::buffer_size(buf));
}

void net_buffer::reserve(size_t r)
{
    m_oBuffer.reserve(r);
}

void net_buffer::reserve_idle(size_t r)
{
    if(idle() < r)
    {
        m_oBuffer.resize(filled() + r);
    }
}
    std::string print_buf(const char * pBuf, size_t len)
    {
        std::stringstream ss;
        for(size_t i = 0; i < len; ++i){
            uint8_t v = (uint8_t) pBuf[i];
            ss<<std::hex<<v/16<<v%16<<" ";
        }
        return ss.str();
    }

}//end namespace ffnet
