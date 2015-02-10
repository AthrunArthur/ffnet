#ifndef _NETWORK_MIDDLE_WARE_NAKE_BONDER_SPLITTER_H_
#define _NETWORK_MIDDLE_WARE_NAKE_BONDER_SPLITTER_H_

#include "common.h"
#include "middleware/bonder_splitter.h"

namespace ffnet
{
class NakeBonderSplitter : public BonderSplitter
{
public:
    virtual std::list<SharedBuffer> split(NetBuffer & oRecvBuffer);
    virtual void bond(NetBuffer & oSendBuffer, const char *pBuf, size_t len);
    virtual void bond(NetBuffer & oSendBuffer, const PackagePtr_t & pkg);
};//end class NakeBonderSplitter
}//end namespace ffnet

#endif