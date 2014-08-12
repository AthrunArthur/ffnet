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
public:
    typedef boost::function<void (boost::shared_ptr<Ty_>, EndpointPtr_t)> PkgRecvHandler_t;
    static void            recvHandler(PackagePtr_t pPkg, EndpointPtr_t pEP, PkgRecvHandler_t handler)
    {
        boost::shared_ptr<Ty_> pConcretPkg= boost::dynamic_pointer_cast<Ty_, Package>(pPkg);
        handler(pConcretPkg, pEP);
    }
};//end class TypedPkgRecvCallback

template<class Ty_>
class PackageNewWrapper
{
public:
    static PackagePtr_t  New()
    {
        return PackagePtr_t(new Ty_());
    }
};//end class TypedNewWrapper
}//end namespace details
}//end namespace ffnet

#endif