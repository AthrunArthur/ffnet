#include "network/net_buffer.h"

namespace ffnet
{
namespace details
{
NetBuffer::NetBuffer(int iInitSize)
    : m_oBuffer(iInitSize)
    , m_iToWriteBufIndex(0)
{
}

void NetBuffer::writeBuffer(const char *pBuf, size_t len)
{
    if(len >= idle())
        m_oBuffer.resize(m_iToWriteBufIndex + len + 64);
    memcpy(&m_oBuffer[m_iToWriteBufIndex], pBuf, len);
    m_iToWriteBufIndex += len;
}

size_t NetBuffer::readBuffer(char *pBuf, size_t len)
{
    if(filled() < len)
        len = filled();
    if(len == 0)
        return 0;
    memcpy(pBuf, &m_oBuffer[0], len);
    return len;
}

void NetBuffer::eraseBuffer(size_t len)
{
    if(len == 0)
        return;
    if(len >= m_iToWriteBufIndex)
        len = m_iToWriteBufIndex;
    m_oBuffer.erase(m_oBuffer.begin(), m_oBuffer.begin() + len);
    m_iToWriteBufIndex -= len;
}

boost::asio::const_buffer NetBuffer::readable() const
{
    return boost::asio::const_buffer(m_oBuffer.data(), m_iToWriteBufIndex);
}
boost::asio::mutable_buffer NetBuffer::writeable()
{
    if(idle() < BUFFER_INC_STEP)
        m_oBuffer.resize(size() + BUFFER_INC_STEP);
    return boost::asio::mutable_buffer(m_oBuffer.data() + m_iToWriteBufIndex, idle());
}

void NetBuffer::appendBuffer(boost::asio::const_buffer buf)
{
    writeBuffer(boost::asio::buffer_cast<const char *>(buf), boost::asio::buffer_size(buf));
}

void NetBuffer::reserve(size_t r)
{
    m_oBuffer.reserve(r);
}

void NetBuffer::reserveIdle(size_t r)
{
	if(idle() < r)
	{
		m_oBuffer.resize(filled() + r);
	}
}

}//end namespace details
}//end namespace ffnet