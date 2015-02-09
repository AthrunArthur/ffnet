#include "nervures/typed_net_nervure.h"
#include "middleware/length_bonder_splitter.h"
#include "common/defines.h"
#include "middleware/raw_pkg_handler.h"
#include <network/events.h>
#include <network/asio_connection.h>

namespace ffnet
{

void TypedNetNervure::handle_raw_pkg(ASIOConnection* from, const SharedBuffer& buf)
{
    const char * pBuf = buf.buffer().get();
    uint32_t iTypeID;
    ffnet::archive::deseralize(pBuf, iTypeID);
    
    if(m_oPkgCreatorContainer.find(iTypeID)  == m_oPkgCreatorContainer.end()) {
        LOG_ERROR(frmwk)<<"TypedNetNervure::handle_raw_pkg() "<< "can't find the type id:"<<iTypeID;
        assert(0 && "can't find the type id in service recv pkg set!");
        return ;
    }
    PkgCreator_t pkg_creator = m_oPkgCreatorContainer[iTypeID];
    PackagePtr_t pPkg = pkg_creator();
    Archive d(const_cast<const char *>(pBuf), buf.length(), Archive::deseralizer);
    pPkg->arch(d);
    LOG_INFO(frmwk)<<"TypedNetNervure::handle_raw_pkg(), a pkg with id:"<<pPkg->getTypeID()<<" is forward to user";
    
    
    TCPConnectionBase * pTCP = dynamic_cast<TCPConnectionBase *>(from);
    if(pTCP != NULL)
    {
        TCPHandlers_t::iterator it = m_oTCPHandlers.find(iTypeID);
        if(it == m_oTCPHandlers.end())
        {
            LOG_ERROR(frmwk)<<"TypedNetNervure::handle_raw_pkg()  cann't find the type id for tcp handler, id : "<<iTypeID;
            assert(0 && "can't find the type id for tcp handler!");
            return ;
        }
        TCPRecvHandler_t handler = it->second;
        handler(pPkg, pTCP);
    }
    else
    {
        UDPHandlers_t::iterator it = m_oUDPHandlers.find(iTypeID);
        UDPPoint * pUDP = dynamic_cast<UDPPoint *>(from);
        EndpointPtr_t pEP = pUDP->getRemoteEndpointPtr();
        if(it == m_oUDPHandlers.end())
        {
            LOG_ERROR(frmwk)<<"TypedNetNervure::handle_raw_pkg() can't find the type id for udp handler, id : "<<iTypeID;
            assert(0 && "can't find the type id for udp handler!");
            return ;
        }
        UDPRecvHandler_t handler = it->second;
        handler(pPkg, pUDP, pEP);
    }
}


TypedNetNervure::TypedNetNervure()
: NetNervure(this)
{
    m_pEH->listen<event::pkg_send_failed>(boost::bind(&TypedNetNervure::onPkgSendFailed, this, _1, _2));
}

void TypedNetNervure::onPkgSendFailed(PackagePtr_t pPkg, EndpointPtr_t pEP)
{
    //std::cout<<"pkg send failed, to remote "<<pEP->to_str();
    //we do nothing here!
}

}
