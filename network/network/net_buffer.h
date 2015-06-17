#pragma once
#include "common.h"
#include <vector>
#include <boost/asio.hpp>

namespace ffnet
{
#define BUFFER_INC_STEP 256
class net_buffer
{
public:
    net_buffer(int iInitSize = 256);

    void                     write_buffer(const char *pBuf, size_t len) ;

    size_t                    read_buffer(char *pBuf, size_t len) ;

    void                        erase_buffer(size_t len);

    boost::asio::const_buffer        readable() const;

    boost::asio::mutable_buffer        writeable() ;

    void                        append_buffer(boost::asio::const_buffer buf) ;
    void                        reserve(size_t r) ;
    void                        reserve_idle( size_t r);
    inline const char                 *buffer() const {
        return &m_oBuffer[m_iToWriteBufIndex];
    }
    inline  size_t                    length() const {
        return m_iToWriteBufIndex-m_iToReadBufIndex;
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
    inline const size_t & read() const {
        return m_iToReadBufIndex;
    }
    inline size_t & read() {
        return m_iToReadBufIndex;
    }
    inline size_t            idle() const {
        return size() - filled();
    }
protected:
    std::vector<char>        m_oBuffer;
    size_t                    m_iToWriteBufIndex;
    size_t m_iToReadBufIndex;
};//end class NetBuffer

    std::string print_buf(const char * pBuf, size_t len);
}//end namespace ffnet
