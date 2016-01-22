#include "common/shared_buffer.h"
namespace ffnet
{
shared_buffer::shared_buffer()
: m_pBuffer()
, m_iLength(0){}

shared_buffer::shared_buffer(const char * buffer, size_t len)
: m_pBuffer(buffer)
, m_iLength(len){}
}
