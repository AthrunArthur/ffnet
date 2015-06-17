#pragma once
#include "common.h"
#include "common/shared_buffer.h"
#include "network/end_point.h"
#include "middleware/package.h"


namespace ffnet {

    class udp_point;
    class tcp_connection_base;

    class udp_pkg_handler {
    public:
        virtual ~udp_pkg_handler(){}
        virtual void handle_pkg(udp_point *pPoint, const shared_buffer &buf, const udp_endpoint &from) = 0;
        virtual bool is_pkg_to_handle(uint32_t pkg_id) = 0;
    };

    class tcp_pkg_handler {
    public:
        virtual ~tcp_pkg_handler(){}
        virtual void handle_pkg(tcp_connection_base *pFrom, const shared_buffer &buf) = 0;
        virtual bool is_pkg_to_handle(uint32_t pkg_id) = 0;
    };

    template<typename Base_, typename Ty_>
    class pkg_recv_callback{
    public:
        typedef boost::function<void(boost::shared_ptr<Ty_>)> pkg_recv_handler_t;

        static void recv_handler(boost::shared_ptr<Base_> pPkg, const pkg_recv_handler_t & handler) {
            boost::shared_ptr<Ty_> pConcretPkg = boost::dynamic_pointer_cast<Ty_, Base_>(pPkg);
            handler(pConcretPkg);
        }
    };

    template<class Base_, class Ty_>
    class tcp_recv_callback{
    public:
        typedef boost::function<void(boost::shared_ptr<Ty_>, tcp_connection_base *)> pkg_recv_handler_t;

        static void recv_handler(boost::shared_ptr<Base_> pPkg, tcp_connection_base *pConn, const pkg_recv_handler_t & handler) {
            boost::shared_ptr<Ty_> pConcretPkg = boost::dynamic_pointer_cast<Ty_, Base_>(pPkg);
            handler(pConcretPkg, pConn);
        }
    };

    template<class Base_, class Ty_>
    class udp_recv_callback{
    public:
        typedef boost::function<void(boost::shared_ptr<Ty_>, udp_point *, udp_endpoint)> pkg_recv_handler_t;

        static void recv_handler(boost::shared_ptr<Base_> pPkg, udp_point *from, udp_endpoint ep, const pkg_recv_handler_t & handler) {
            boost::shared_ptr<Ty_> pConcretPkg = boost::dynamic_pointer_cast<Ty_, Base_>(pPkg);
            handler(pConcretPkg, from, ep);
        }
    };


    template<class Ty_>
    class package_new_wrapper {
    public:
        static package_ptr New() {
            return package_ptr(new Ty_());
        }
    };


}
