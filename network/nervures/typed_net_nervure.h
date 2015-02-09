#ifndef _NETWORK_NERVURES_TYPED_NET_NERVURE_H_
#define _NETWORK_NERVURES_TYPED_NET_NERVURE_H_

#include "common.h"
#include "nervures/net_nervure.h"
#include "package/package.h"
#include "common/archive.h"
#include "network/endpoint_data.h"
#include "network/end_point.h"
#include "package/pkg_callback.h"
#include "middleware/raw_pkg_handler.h"

#include <map>
#include <cassert>

namespace ffnet
{
typedef boost::function<PackagePtr_t ()> PkgCreator_t;

class TypedNetNervure : public NetNervure, public RawPkgHandler
{
public:
    TypedNetNervure();
        
    virtual ~TypedNetNervure() {};

    /*
    void                send(const boost::shared_ptr<Package> & pPkg, const EndpointPtr_t & ep);
#ifdef PROTO_BUF_SUPPORT
    static void             send(const boost::shared_ptr<google::protobuf::Message> & pMsg, const EndpointPtr_t & ep);
#endif
*/
    
    template<class PkgTy_>
    void                        tcpToRecvPkg(typename details::TypedTCPRecvCallback<PkgTy_>::PkgRecvHandler_t handler)
    {
        boost::shared_ptr<PkgTy_> pPkg(new PkgTy_());
        if(m_oPkgCreatorContainer.find(pPkg->getTypeID()) == m_oPkgCreatorContainer.end())
            m_oPkgCreatorContainer.insert(std::make_pair(pPkg->getTypeID(), 
                                                         boost::bind(details::PackageNewWrapper<PkgTy_>::New)));
        TCPRecvHandler_t rh(boost::bind(details::TypedTCPRecvCallback<PkgTy_>::recvHandler, _1, _2, handler));
        m_oTCPHandlers.insert(std::make_pair(pPkg->getTypeID(), rh));
    }
    
    template<class PkgTy_> 
    void                        udpToRecvPkg(typename details::TypedUDPRecvCallback<PkgTy_>::PkgRecvHandler_t handler)
    {
        boost::shared_ptr<PkgTy_> pPkg(new PkgTy_());
        if(m_oPkgCreatorContainer.find(pPkg->getTypeID()) == m_oPkgCreatorContainer.end())
            m_oPkgCreatorContainer.insert(std::make_pair(pPkg->getTypeID(), 
                                                         boost::bind(details::PackageNewWrapper<PkgTy_>::New)));
        UDPRecvHandler_t rh(boost::bind(details::TypedUDPRecvCallback<PkgTy_>::recvHandler, _1, _2, _3, handler));
        m_oUDPHandlers.insert(std::make_pair(pPkg->getTypeID(), rh));
    }
protected:
    void        onPkgSendFailed(PackagePtr_t pPkg, EndpointPtr_t pEP);
    
    virtual void  handle_raw_pkg(ffnet::ASIOConnection * from, const SharedBuffer & buf);
    
protected:
    typedef boost::function<void (PackagePtr_t, TCPConnectionBase *)> TCPRecvHandler_t;
    typedef boost::function<void (PackagePtr_t, UDPPoint *, EndpointPtr_t) > UDPRecvHandler_t;
    typedef std::map<uint32_t, PkgCreator_t> PkgCreatorContainer_t;
    typedef std::map<uint32_t, TCPRecvHandler_t> TCPHandlers_t;
    typedef std::map<uint32_t, UDPRecvHandler_t> UDPHandlers_t;

    PkgCreatorContainer_t       m_oPkgCreatorContainer;
    TCPHandlers_t       m_oTCPHandlers;
    UDPHandlers_t       m_oUDPHandlers;
};//end class TypedNetNervure;
}//end namespace ffnet
#endif
