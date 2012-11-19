#ifdef PROTO_BUF_SUPPORT
#include "nervures/proto_buf_nervure.h"
#include "handler/conn_holder_handler.h"
#include "middleware/length_bonder_splitter.h"
#include "log.h"
#include "common.h"
#include "package/proto_buf_wrapper_pkg.h"
namespace ffnet
{
	using ffnet::ConnectionHolderHandler;
	
namespace details
{
ProtoBufNervure::ProtoBufNervure()
: NetNervure(ASIOConnHandlerPtr_t(new ConnectionHolderHandler()),BonderSplitterPtr_t(new LengthBonderSplitter()) )
{

}

void ProtoBufNervure::deseralizeAndDispatchHandler(EndPointBufferPtr_t ebp)
{
    const char *pBuf = ebp->buffer().buffer().get();
	FFNET_DEBUG(
		log_frmwk("ProtoBufNervure", "deseralizeAndDispatchHandler(), buf: %s", printBuf(pBuf, ebp->buffer().length()).c_str());
	)
	String s;
	ffnet::deseralize(pBuf, s);
	ProtoBufWrapperPkg pkg(s);
	
	PkgRecvHandler_t handler = m_oPkgHandlers[s];


	//pPkg->m_oBuffer = ebp->buffer().buffer();
	Deseralizer d(const_cast<const char *>(ebp->buffer().buffer().get()), ebp->buffer().length());
	pkg.archive(d);
	
	m_oTasks.push_back(boost::bind(handler, pkg.PBMessage(), ebp->Endpoint()));
}


}//end namespace details
}//end namespace ffnet
#endif//end PROTO_BUF_SUPPORT