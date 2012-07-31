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
		virtual void					bond(NetBuffer & oSendBuffer, PackagePtr_t pkg) ;
	};//end class LengthBonderSplitter
}//end namespace

#endif