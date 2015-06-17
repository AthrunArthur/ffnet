#include "middleware/nake_bonder_splitter.h"

namespace ffnet
{
    
void NakeBonderSplitter::bond(NetBuffer& oSendBuffer, const PackagePtr_t& pkg)
{
    assert(0 && "really? you cannot send pkg with a NakeBonderSplitter!");
}
void NakeBonderSplitter::bond(NetBuffer& oSendBuffer, const char* pBuf, size_t len)
{
    oSendBuffer.writeBuffer(pBuf, len);
}
std::list< SharedBuffer > NakeBonderSplitter::split(NetBuffer& oRecvBuffer)
{
    std::list<SharedBuffer> res;
    size_t n = boost::asio::buffer_size(oRecvBuffer.readable());
    if (n == 0)
        return res;
    SharedBuffer sb(boost::shared_array<char>(new char[n]));
    const char * pBuf = boost::asio::buffer_cast<const char *>(oRecvBuffer.readable());
    
    std::memcpy(sb.buffer().get(), pBuf, n);
    res.push_back(sb);
    oRecvBuffer.eraseBuffer(n);
    return res;
}

}//end namespace ffnet