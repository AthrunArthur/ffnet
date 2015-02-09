#ifndef NETWORK_PACKAGE_PKG_CALLBACK_H_
#define NETWORK_PACKAGE_PKG_CALLBACK_H_
#include "common.h"
#include "package/package.h"
#include "network/asio_connection.h"
#include "network/tcp_connection_base.h"
#include "network/udp_point.h"

namespace ffnet
{
namespace details
{

template<class Ty_>
class TypedTCPRecvCallback 
{
public:
    typedef boost::function<void (boost::shared_ptr<Ty_>, TCPConnectionBase *)> PkgRecvHandler_t;
    static void            recvHandler(PackagePtr_t pPkg, TCPConnectionBase * pEP, PkgRecvHandler_t handler)
    {
        boost::shared_ptr<Ty_> pConcretPkg= boost::dynamic_pointer_cast<Ty_, Package>(pPkg);
        handler(pConcretPkg, pEP);
    }
};//end class TypedPkgRecvCallback

template<class Ty_>
class TypedUDPRecvCallback 
{
public:
    typedef boost::function<void (boost::shared_ptr<Ty_>, UDPPoint *, EndpointPtr_t)> PkgRecvHandler_t;
    static void             recvHandler(PackagePtr_t pPkg, UDPPoint * from, EndpointPtr_t pEP, PkgRecvHandler_t handler)
    {
        boost::shared_ptr<Ty_> pConcretPkg = boost::dynamic_pointer_cast<Ty_, Package>(pPkg);
        handler(pConcretPkg, from, pEP);
    }
};

template<class Ty_>
class ThreadRecvCallback
{
public:
    typedef boost::function<void (boost::shared_ptr<Ty_>, EndpointPtr_t)> PkgRecvHandler_t;
    static void     recvHandler(PackagePtr_t pPkg, EndpointPtr_t pEP, PkgRecvHandler_t handler)
    {
        boost::shared_ptr<Ty_> pConcretPkg = boost::dynamic_pointer_cast<Ty_, Package>(pPkg);
        handler(pConcretPkg, pEP);
    }
};//end class ThreadRecvCallback

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