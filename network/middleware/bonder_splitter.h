#ifndef _NETWORK_MIDDLEWARE_BONDER_SPLITTER_H_
#define _NETWORK_MIDDLEWARE_BONDER_SPLITTER_H_
#include "common.h"
#include "common/shared_buffer.h"
#include "network/net_buffer.h"
#include "package/package.h"
#include <list>
namespace ffnet
{
	using ffnet::details::NetBuffer;
	class BonderSplitter
	{
	public:
		virtual std::list<SharedBuffer>		split(NetBuffer &oRecvBuffer) = 0;
		virtual void					bond(NetBuffer & oSendBuffer, const char *pBuf, size_t len) = 0;
		virtual void					bond(NetBuffer & oSendBuffer, PackagePtr_t pkg) = 0;
	};//end class BonderSplitter
	typedef boost::shared_ptr<BonderSplitter> BonderSplitterPtr_t;
}//end namespace ffnet
#endif
