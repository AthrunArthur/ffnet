#ifndef _NETWORK_NERVURES_TYPED_NET_NERVURE_H_
#define _NETWORK_NERVURES_TYPED_NET_NERVURE_H_

#include "common.h"
#include "framework/net_nervure.h"
#include "package/package.h"
#include "common/archive.h"
#include "network/endpoint_data.h"
#include "network/end_point.h"
#include "package/pkg_callback.h"
#include <map>
#include <cassert>

namespace ffnet
{
typedef boost::function<PackagePtr_t ()> PkgCreator_t;
namespace details
{
class TypedNetNervure : public NetNervure
{
public:
    TypedNetNervure();
        
    virtual ~TypedNetNervure() {};

    template<class PkgTy_>
    void						addNeedToRecvPkg(typename TypedPkgRecvCallback<PkgTy_>::PkgRecvHandler_t handler)
    {
        boost::shared_ptr<PkgTy_> pPkg(new PkgTy_());
        m_oPkgCreatorContainer.insert(std::make_pair(pPkg->getTypeID(), boost::bind(PackageNewWrapper<PkgTy_>::New)));
        PkgRecvHandler_t rh(boost::bind(TypedPkgRecvCallback<PkgTy_>::recvHandler, _1, _2, handler));
        m_oPkgHandlers.insert(std::make_pair(pPkg->getTypeID(), rh));
    }
protected:
    virtual void				deseralizeAndDispatchHandler(const EndPointBufferPtr_t & epb);

protected:
    typedef boost::function<void (PackagePtr_t, EndpointPtr_t)> PkgRecvHandler_t;
    typedef std::map<uint32_t, PkgCreator_t> PkgCreatorContainer_t;
    typedef std::map<uint32_t, PkgRecvHandler_t> PkgHandlers_t;

    PkgCreatorContainer_t       m_oPkgCreatorContainer;
    PkgHandlers_t       m_oPkgHandlers;
};//end class TypedNetNervure;
}//end namespace details
}//end namespace ffnet
#endif
