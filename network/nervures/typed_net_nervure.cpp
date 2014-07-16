#include "nervures/typed_net_nervure.h"
#include "middleware/length_bonder_splitter.h"
#include "common/defines.h"

namespace ffnet
{    
namespace details
{
    
TypedNetNervure::TypedNetNervure()
: NetNervure(BonderSplitterPtr_t(new LengthBonderSplitter()))
{
}

void TypedNetNervure::deseralizeAndDispatchHandler(const EndPointBufferPtr_t & ebp)
{
    const char *pBuf = ebp->buffer().buffer().get();
    LOG_INFO(frmwk)<<"TypedNetNervure::deseralizeAndDispatchHandler()" <<"buf: "<<printBuf(pBuf, ebp->buffer().length());
    uint32_t iTypeID;
    ffnet::archive::deseralize(pBuf, iTypeID);

    if(m_oPkgCreatorContainer.find(iTypeID)  == m_oPkgCreatorContainer.end()) {
        LOG_ERROR(frmwk)<<"NetNervure::deseralizeAndDispatchHandler() "<< "can't find the type id:"<<iTypeID;
        assert(0 && "can't find the type id in service recv pkg set!");
        return ;
    }
    PkgCreator_t pkg_creator = m_oPkgCreatorContainer[iTypeID];
    PackagePtr_t pPkg = pkg_creator();
    PkgRecvHandler_t handler = m_oPkgHandlers[iTypeID];

    //pPkg->m_oBuffer = ebp->buffer().buffer();
    Archive d(const_cast<const char *>(ebp->buffer().buffer().get()), ebp->buffer().length(), Archive::deseralizer);
    pPkg->arch(d);
    
    m_oTasks.push_back(boost::bind(handler, pPkg, ebp->Endpoint()));
}
}
}
