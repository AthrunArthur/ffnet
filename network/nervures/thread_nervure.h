#ifndef _NETWORK_NERVURES_THREAD_NERVURE_H_
#define _NETWORK_NERVURES_THREAD_NERVURE_H_

#include "common.h"
#include "nervures/typed_net_nervure.h"
#include "nervures/net_nervure_from_file.h"
#include "common/blocking_queue.h"
#include "boost/thread.hpp"


namespace ffnet{
    
class ThreadNervure : public TypedNetNervure{
public:
    template <class Ty_>
    void    addNeedToRecvPkg(typename details::ThreadRecvCallback<Ty_>::PkgRecvHandler_t handler)
    {
        Ty_ t;
        tcpToRecvPkg<Ty_>(boost::bind(&ThreadNervure::tcpPkgHandler<Ty_>, this, _1, _2));
        udpToRecvPkg<Ty_>(boost::bind(&ThreadNervure::udpPkgHandler<Ty_>, this, _1, _2, _3));
        m_oHandlers[t.getTypeID()] = boost::bind(&details::ThreadRecvCallback<Ty_>::recvHandler, _1, _2, handler);
    }
    
    void        send(PackagePtr_t pPkg, EndpointPtr_t pEP);
    
    void    run();
    void    stop();
    
protected:
    using TypedNetNervure::tcpToRecvPkg;
    using TypedNetNervure::udpToRecvPkg;
    void        send_func(PackagePtr_t pPkg, EndpointPtr_t pEP);
    template<class Ty_>
    void    tcpPkgHandler(boost::shared_ptr<Ty_> pPkg, TCPConnectionBase * from)
    {
        m_oRecvedPkgs.push_back(std::make_pair(pPkg, from->getRemoteEndpointPtr()));
    }
    template<class Ty_>
    void    udpPkgHandler(boost::shared_ptr<Ty_>pPkg, UDPPoint * pConn, EndpointPtr_t from)
    {
        m_oRecvedPkgs.push_back(std::make_pair(pPkg, from));
    }
    
    
protected:
    typedef boost::function<void (PackagePtr_t, EndpointPtr_t) > Handler_t;
    typedef std::map<uint32_t, Handler_t> PkgHandlers_t;
    typedef std::pair<PackagePtr_t, EndpointPtr_t> PkgAndFrom_t;
    PkgHandlers_t       m_oHandlers;
    
    BlockingQueue<PkgAndFrom_t>  m_oRecvedPkgs;
    
};//end class ThreadNervure
        
}//end namespace ff
#endif
