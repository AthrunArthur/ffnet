#ifndef _NETWORK_MIDDLEWARE_LENGTH_BONDER_SPLITTER_H_
#define _NETWORK_MIDDLEWARE_LENGTH_BONDER_SPLITTER_H_

#include "common.h"
#include "middleware/bonder_splitter.h"

namespace ffnet
{
class LengthBonderSplitter : public BonderSplitter
{
public:
    virtual std::list<SharedBuffer>			split(NetBuffer &oRecvBuffer) ;
    virtual void					bond(NetBuffer & oSendBuffer, const char *pBuf, size_t len) ;
    virtual void					bond(NetBuffer & oSendBuffer, const PackagePtr_t & pkg) ;
protected:
    //ffnet::Logger< true >::self printBuf(char* pBuf, uint32_t len);
};//end class LengthBonderSplitter
}//end namespace

#endif