#ifdef PROTO_BUF_SUPPORT
#ifndef FFNET_NERVURES_PROTO_BUF_NERVURE_H_
#define FFNET_NERVURES_PROTO_BUF_NERVURE_H_

#include "common.h"
#include "framework/net_nervure.h"
#include "package/package.h"
#include "archive/archive.h"
#include "network/endpoint_data.h"
#include "network/end_point.h"
#include "package/pkg_callback.h"
#include <map>
#include <cassert>
#include <google/protobuf/descriptor.h>

namespace ffnet
{
typedef boost::shared_ptr<google::protobuf::Message> MessagePtr_t;
typedef boost::function<void (MessagePtr_t, EndpointPtr_t)> PkgRecvHandler_t;
namespace details
{
class ProtoBufNervure : public NetNervure
{
public:
    ProtoBufNervure();

    virtual ~ProtoBufNervure() {};

    template<class PkgTy_>
    void						addNeedToRecvPkg(typename TypedPkgRecvCallback<PkgTy_>::PkgRecvHandler_t handler)
    {
        //boost::shared_ptr<PkgTy_> pPkg(new PkgTy_());
        String name(PkgTy_::default_instance().GetDescriptor()->full_name());
        m_oPkgHandlers.insert(std::make_pair(name, handler));
    }
protected:
    virtual void				deseralizeAndDispatchHandler(EndPointBufferPtr_t epb);

protected:
    typedef std::map<String, PkgRecvHandler_t> PkgHandlers_t;

    PkgHandlers_t				m_oPkgHandlers;
};//end class ProtoBufNervure
}//end namespace details
}//end namespace ffnet


#endif//end FFNET_NERVURES_PROTO_BUF_NERVURE_H_

#endif//end PROTO_BUG_SUPPORT