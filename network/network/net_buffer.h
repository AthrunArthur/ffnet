#ifndef _NETWORK_NETWORK_NET_BUFFER_H_
#define _NETWORK_NETWORK_NET_BUFFER_H_
#include "common.h"
#include <vector>
#include <boost/asio.hpp>

namespace ffnet
{
namespace details
{
#define BUFFER_INC_STEP 256
class NetBuffer
{
public:
    NetBuffer(int iInitSize = 256);
        
    void                     writeBuffer(const char *pBuf, size_t len) ;

    size_t                    readBuffer(char *pBuf, size_t len) ;

    void                        eraseBuffer(size_t len);

    boost::asio::const_buffer        readable() const;
    
    boost::asio::mutable_buffer        writeable() ;
    
    void                        appendBuffer(boost::asio::const_buffer buf) ;
    void                        reserve(size_t r) ;
    void                        reserveIdle( size_t r);
    inline const char                 *buffer() const {
        return &m_oBuffer[0];
    }
    inline  size_t                    length() const {
        return m_iToWriteBufIndex;
    }
    inline size_t             size() const {
        return m_oBuffer.size();
    }
    inline void            resize(size_t s) {
        m_oBuffer.resize(s);
    }
    inline size_t            capacity() const {
        return m_oBuffer.capacity();
    }
    inline const size_t     &filled() const {
        return m_iToWriteBufIndex;
    }
    inline size_t             &filled() {
        return m_iToWriteBufIndex;
    }
    inline size_t            idle() const {
        return size() - filled();
    }
protected:
    std::vector<char>        m_oBuffer;
    size_t                    m_iToWriteBufIndex;
};//end class NetBuffer

}//end namespace details
}//end namespace ffnet
#endif
