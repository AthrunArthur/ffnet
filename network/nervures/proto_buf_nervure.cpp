#ifdef PROTO_BUF_SUPPORT
#include "nervures/proto_buf_nervure.h"
#include "middleware/length_bonder_splitter.h"
#include "common/defines.h"
#include "common.h"
#include "package/proto_buf_wrapper_pkg.h"
namespace ffnet
{	
ProtoBufNervure::ProtoBufNervure()
: NetNervure(BonderSplitterPtr_t(new LengthBonderSplitter()) )
{

}

void ProtoBufNervure::deseralizeAndDispatchHandler(EndPointBufferPtr_t ebp)
{
    const char *pBuf = ebp->buffer().buffer().get();
	LOG_DEBUG(frmwk)<<"ProtoBufNervure::deseralizeAndDispatchHandler() "<< "buf:"<< printBuf(pBuf, ebp->buffer().length());
	uint32_t id;
	String s;
	//ffnet::deseralize(pBuf, id); //We don't need to deseralize id;
	ffnet::archive::deseralize(pBuf + sizeof(id), s);
	ProtoBufWrapperPkg pkg(s);
	
	PkgRecvHandler_t handler = m_oPkgHandlers[s];

	Archive d(const_cast<const char *>(ebp->buffer().buffer().get()), ebp->buffer().length(), Archive::deseralizer);
	pkg.arch(d);
	
	m_oTasks.push_back(boost::bind(handler, pkg.PBMessage(), ebp->Endpoint()));
}


}//end namespace ffnet
#endif//end PROTO_BUF_SUPPORT