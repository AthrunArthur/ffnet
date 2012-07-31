#include "common/shared_buffer.h"
namespace ffnet
{
SharedBuffer::SharedBuffer()
: m_oBuffer()
, m_iLength(0){

}
SharedBuffer::SharedBuffer(boost::shared_array< char > buffer, size_t len)
: m_oBuffer(buffer)
, m_iLength(len)
{

}


boost::shared_array< char >& SharedBuffer::buffer()
{
	return m_oBuffer;
}

size_t &SharedBuffer::length()
{
	return m_iLength;
}

}