#ifndef _NETWORK_COMMON_SHARED_BUFFER_H_
#define _NETWORK_COMMON_SHARED_BUFFER_H_
#include "common.h"
namespace ffnet
{
class SharedBuffer {

public:
    SharedBuffer();
    SharedBuffer(boost::shared_array<char> buffer, size_t len);

    boost::shared_array<char> & buffer() ;
    size_t &    length();

protected:
    boost::shared_array<char>	m_oBuffer;
    size_t      m_iLength;

};//end class SharedBuffer
}//end namespace ffnet
#endif
