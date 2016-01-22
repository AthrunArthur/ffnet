#ifndef _FFNET_EXTENSION_UDP_RETRANS_UDP_RETRANS_H_
#define _FFNET_EXTENSION_UDP_RETRANS_UDP_RETRANS_H_

#include "common.h"
#include "package/package.h"
#include "extension/package_types.h"
#include "nervures/typed_net_nervure.h"


namespace ffnet{

  class UDPRetransmitter;

  class RetransPkg: Package{
    public:
      RetransPkg(PackagePtr_t pPkg)
        : Package(retrans_pkg_type){}

      virtual void archive(Archive & ar)
      {
        m_pPkg->arch(ar);
      }
    protected:
      PackagePtr_t m_pPkg;
  };

  class UDPRetransmitter{
    public:
      UDPRetransmitter(TypedNetNervure * pNN){}


      void udpToRecvPkg(typename UDPRetransRecvCallback<PkgTy_>::PkgRecvHandler_t handler)
      {
          pNN->udpToRecvPkg<RetransPkg>(std::bind(&UDPRetransmitter::onRecvPkg, this));
      }

      void        send(const PackagePtr_t & pkg, const EndpointPtr_t & pEndpoint)
      {
      }

    protected:
      void onRecvPkg(std::shared_ptr<RetransPkg>, UDPPoint *, EndpointPtr_t)
      {
      }


  };//end class UDPRetransmitter
}//end namespace ffnet

#endif
