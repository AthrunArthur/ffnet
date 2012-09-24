#ifndef NETWORK_PACKAGE_PKG_CALLBACK_H_
#define NETWORK_PACKAGE_PKG_CALLBACK_H_

#include "package/package.h"
#include "network/end_point.h"

namespace ffnet
{
namespace details
{
		class PkgRecvCallback
		{
		public:
			typedef boost::function<void (PackagePtr_t, EndpointPtr_t)> PkgRecvHandler_t;
		};//end class PkgRecvCallback
		
		template<class Ty_>
		class TypedPkgRecvCallback : public PkgRecvCallback
		{
		};//end class TypedPkgRecvCallback
}//end namespace details
}//end namespace ffnet

#endif