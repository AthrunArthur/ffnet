#include "nervures/typed_net_nervure.h"
#include "handler/conn_holder_handler.h"
#include "middleware/length_bonder_splitter.h"
#include "log.h"

namespace ffnet
{
	using ffnet::ConnectionHolderHandler;
	
namespace details
{
	
TypedNetNervure::TypedNetNervure()
: NetNervure(ASIOConnHandlerPtr_t(new ConnectionHolderHandler()),BonderSplitterPtr_t(new LengthBonderSplitter()))
{
}

void TypedNetNervure::deseralizeAndDispatchHandler(EndPointBufferPtr_t ebp)
{
    const char *pBuf = ebp->buffer().buffer().get();
	FFNET_DEBUG(
	log_frmwk("TypedNetNervure", "deseralizeAndDispatchHandler(), buf: %s", printBuf(pBuf, ebp->buffer().length()).c_str());
	)
    uint32_t iTypeID;
    ffnet::deseralize(pBuf, iTypeID);

    if(m_oPkgInstanceContainer.find(iTypeID)  == m_oPkgInstanceContainer.end()) {
	FFNET_DEBUG(
	 log_frmwk("NetNervure", "deseralizeAndDispatchHandler(), can't find the type id: %d in service recv pkg set!", iTypeID);
	)
        assert(0 && "can't find the type id in service recv pkg set!");
        return ;
    }
    PackagePtr_t pPkg = m_oPkgInstanceContainer[iTypeID];
	PkgRecvHandler_t handler = m_oPkgHandlers[iTypeID];
    m_oPkgInstanceContainer[iTypeID] = pPkg->createInstance();

	pPkg->m_oBuffer = ebp->buffer().buffer();
	Deseralizer d(const_cast<const char *>(ebp->buffer().buffer().get()), ebp->buffer().length());
	pPkg->archiveDeserialize(d);
	
	m_oTasks.push_back(boost::bind(handler, pPkg, ebp->Endpoint()));
}
}
}