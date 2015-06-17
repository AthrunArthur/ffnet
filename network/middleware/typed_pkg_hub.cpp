#include "middleware/typed_pkg_hub.h"
#include "common/defines.h"
#include "middleware/pkg_handler.h"
#include "network/events.h"
#include "network/asio_point.h"

namespace ffnet {



    typed_pkg_hub::typed_tcp_pkg_handler::typed_tcp_pkg_handler(typed_pkg_hub *pHub)
            :m_pHub(pHub){}

    typed_pkg_hub::typed_tcp_pkg_handler::~typed_tcp_pkg_handler(){}

     void typed_pkg_hub::typed_tcp_pkg_handler::handle_pkg(tcp_connection_base *pFrom, const shared_buffer &buf) {
        m_pHub->handle_tcp_pkg(pFrom, buf);
    }
    bool typed_pkg_hub::typed_tcp_pkg_handler::is_pkg_to_handle(uint32_t pkg_id) {
        return m_pHub->is_tcp_pkg_to_handle(pkg_id);
    }

    typed_pkg_hub::typed_udp_pkg_handler::typed_udp_pkg_handler(typed_pkg_hub *pHub)
    : m_pHub(pHub){ }

    void typed_pkg_hub::typed_udp_pkg_handler::handle_pkg(udp_point * pPoint, const shared_buffer & buf, const udp_endpoint & ep){
        m_pHub->handle_udp_pkg(pPoint, buf, ep);
    }
    bool typed_pkg_hub::typed_udp_pkg_handler::is_pkg_to_handle(uint32_t pkg_id) {
        return m_pHub->is_udp_pkg_to_handle(pkg_id);
    }
    typed_pkg_hub::typed_udp_pkg_handler::~typed_udp_pkg_handler(){}

    typed_pkg_hub::typed_pkg_hub() {
        m_pTCPPkgHandler = new typed_tcp_pkg_handler(this);
        m_pUDPPkgHandler = new typed_udp_pkg_handler(this);
    }

    typed_pkg_hub::~typed_pkg_hub(){
        delete m_pTCPPkgHandler;
        delete m_pUDPPkgHandler;
    }

    package_ptr typed_pkg_hub::get_pkg(const shared_buffer & buf){
        const char *pBuf = buf.buffer();
        uint32_t iTypeID;
        ffnet::deseralize(pBuf, iTypeID);
        LOG_DEBUG(frmwk)<<"typed_pkt_hub::get_pkg(), buf is "<<print_buf(pBuf, buf.length());

        if (m_oPkgCreatorContainer.find(iTypeID) == m_oPkgCreatorContainer.end()) {
            LOG_ERROR(frmwk) << "TypedPkgHub::get_pkg() " << "can't find the type id:" << iTypeID;
            assert(0 && "can't find the type id in service recv pkg set!");
            return package_ptr();
        }
        pkg_creator_t pkg_creator = m_oPkgCreatorContainer[iTypeID];
        package_ptr pPkg = pkg_creator();
        marshaler d(const_cast<const char *>(pBuf), buf.length(), marshaler::deseralizer);
        pPkg->arch(d);
        LOG_INFO(frmwk) << "TypedPkgHub::get_pkg(), a pkg with id:" << pPkg->type_id() <<
                        " is forward to user";
        return pPkg;
    }
    void typed_pkg_hub::handle_tcp_pkg(tcp_connection_base * pFrom, const shared_buffer & buf){
        package_ptr pkg = get_pkg(buf);

        if(pkg){
            bool got_callback= false;
            tcp_handlers_t::iterator it1 = m_oTCPHandlers.find(pkg->type_id());
            if(it1 != m_oTCPHandlers.end()){
                got_callback = true;
                tcp_recv_handler_t handler = it1->second;
                handler(pkg, pFrom);
            }
            pkg_handlers_t::iterator it2 = m_oPkgHandlers.find(pkg->type_id());
            if(it2 != m_oPkgHandlers.end()){
                got_callback = true;
                pkg_recv_handler_t handler = it2->second;
                handler(pkg);
            }
            if(!got_callback){
                LOG_ERROR(frmwk)<<"TypedPkgHub::handle_tcp_pkg(), a pkg with id: "<<pkg->type_id()<<" has no handler!";
            }
        }
    }

    bool typed_pkg_hub::is_tcp_pkg_to_handle(uint32_t pkg_id) {
        if (m_oTCPHandlers.find(pkg_id) == m_oTCPHandlers.end() &&
                m_oPkgHandlers.find(pkg_id) == m_oPkgHandlers.end())
            return false;
        return true;
    }

    void typed_pkg_hub::handle_udp_pkg(udp_point * pPoint, const shared_buffer & buf, const udp_endpoint& from){
        package_ptr pkg = get_pkg(buf);

        if(pkg){
            bool got_callback= false;
            udp_handlers_t::iterator it1 = m_oUDPHandlers.find(pkg->type_id());
            if(it1 != m_oUDPHandlers.end()){
                got_callback = true;
                udp_recv_handler_t handler = it1->second;
                handler(pkg, pPoint, from);
            }
            pkg_handlers_t::iterator it2 = m_oPkgHandlers.find(pkg->type_id());
            if(it2 != m_oPkgHandlers.end()){
                got_callback = true;
                pkg_recv_handler_t handler = it2->second;
                handler(pkg);
            }
            if(!got_callback){
                LOG_ERROR(frmwk)<<"TypedPkgHub::handle_udp_pkg(), a pkg with id: "<<pkg->type_id()<<" has no handler!";
            }
        }
    }

    bool typed_pkg_hub::is_udp_pkg_to_handle(uint32_t pkg_id) {
        if(m_oUDPHandlers.find(pkg_id) == m_oUDPHandlers.end() &&
                m_oPkgHandlers.find(pkg_id) == m_oPkgHandlers.end())
            return false;
        return true;
    }

}
