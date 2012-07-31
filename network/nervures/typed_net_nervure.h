#ifndef _NETWORK_NERVURES_TYPED_NET_NERVURE_H_
#define _NETWORK_NERVURES_TYPED_NET_NERVURE_H_

#include "common.h"
#include "framework/net_nervure.h"
#include "package/package.h"
#include "common/archive.h"
#include "network/endpoint_data.h"
#include "network/end_point.h"
#include <map>
#include <cassert>

namespace ffnet
{
typedef boost::function<void (PackagePtr_t, EndpointPtr_t)> PkgRecvHandler_t;
	
namespace details
{
class TypedNetNervure : public NetNervure
{
public:
    TypedNetNervure();
        
    virtual ~TypedNetNervure() {};

    void						addNeedToRecvPkg(PackagePtr_t pPkg, PkgRecvHandler_t handler);
protected:
	virtual void				deseralizeAndDispatchHandler(EndPointBufferPtr_t epb);

protected:
    typedef std::map<uint32_t, PackagePtr_t> PkgInstanceContainer_t;
	typedef std::map<uint32_t, PkgRecvHandler_t> PkgHandlers_t;
	
    PkgInstanceContainer_t		m_oPkgInstanceContainer;
	PkgHandlers_t				m_oPkgHandlers;
};//end class TypedNetNervure;
}//end namespace details
}//end namespace ffnet
#endif