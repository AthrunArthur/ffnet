#ifdef PROTO_BUF_SUPPORT
#ifndef FFNET_NERVURES_PROTO_BUF_NERVURE_H_
#define FFNET_NERVURES_PROTO_BUF_NERVURE_H_

#include "common.h"
#include "framework/net_nervure.h"
#include "package/package.h"
#include "common/archive.h"
#include "network/endpoint_data.h"
#include "network/end_point.h"
#include "package/pkg_callback.h"
#include <map>
#include <cassert>
#include <google/protobuf/descriptor.h>

namespace ffnet
{
typedef boost::shared_ptr< ::google::protobuf::Message> MessagePtr_t;
typedef boost::function<void (MessagePtr_t, EndpointPtr_t)> PkgRecvHandler_t;

class ProtoBufNervure : public NetNervure
{
public:
    ProtoBufNervure();

    virtual ~ProtoBufNervure() {};

	//! Not thread-safe
    template<class PkgTy_>
    void						addNeedToRecvPkg(typename details::TypedPkgRecvCallback<PkgTy_>::PkgRecvHandler_t handler)
    {
        String name(PkgTy_::default_instance().GetDescriptor()->full_name());
		PkgRecvHandler_t h = boost::bind(ProtoBufNervure::handlerToAvoidTypeErrorFromCompiler<PkgTy_>,  _1, _2, handler);
		m_oPkgHandlers.insert(std::make_pair(name, h));
    }
protected:
    virtual void				deseralizeAndDispatchHandler(const EndPointBufferPtr_t & epb);
	
	template <class PkgTy_>
	static void handlerToAvoidTypeErrorFromCompiler
		( MessagePtr_t pMsg, EndpointPtr_t ep,  typename details::TypedPkgRecvCallback<PkgTy_>::PkgRecvHandler_t handler)
	{
		boost::shared_ptr<PkgTy_> pConcretPkg= boost::dynamic_pointer_cast<PkgTy_, ::google::protobuf::Message>(pMsg);
		handler(pConcretPkg, ep);
	}
protected:
    typedef std::map<String, PkgRecvHandler_t> PkgHandlers_t;

    PkgHandlers_t				m_oPkgHandlers;
};//end class ProtoBufNervure
}//end namespace ffnet


#endif//end FFNET_NERVURES_PROTO_BUF_NERVURE_H_

#endif//end PROTO_BUG_SUPPORT