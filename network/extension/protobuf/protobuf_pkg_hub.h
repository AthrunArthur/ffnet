#pragma once
#include "common.h"
#include "common/archive.h"
#include "middleware/net_nervure.h"
#include "middleware/package.h"
#include "middleware/pkg_handler.h"
#include "network/end_point.h"
#include "extension/package_types.h"
#include "extension/protobuf/proto_buf_wrapper_pkg.h"

#include <map>
#include <cassert>

namespace  ffnet{
    class protobuf_pkg_hub{
    public:
        protobuf_pkg_hub();
        virtual ~protobuf_pkg_hub();

        tcp_pkg_handler * get_tcp_pkg_handler() {return m_pTCPPkgHandler;}
        udp_pkg_handler * get_udp_pkg_handler() {return m_pUDPPkgHandler;}

        template <class PkgTy_>
        void to_recv_pkg(const typename pkg_recv_callback<Message, PkgTy_>::pkg_recv_handler_t & handler)
        {
            std::string name(PkgTy_::default_instance().GetDescriptor()->full_name());
            pkg_recv_handler_t h = std::bind(pkg_recv_callback<Message, PkgTy_>::recv_handler, _1, handler);
            m_oPkgHandlers.insert(std::make_pair(name, h));
        }

        template <class PkgTy_>
        void tcp_to_recv_pkg(const typename tcp_recv_callback<Message, PkgTy_>::pkg_recv_handler_t & handler){
            std::string name(PkgTy_::default_instance().GetDescriptor()->full_name());
            tcp_recv_handler_t h = std::bind(tcp_recv_callback<Message, PkgTy_>::recv_handler, _1, _2, handler);
            m_oTCPHandlers.insert(std::make_pair(name, h));
        }

        template <class PkgTy_>
        void udp_to_recv_pkg(const typename udp_recv_callback<Message, PkgTy_>::pkg_recv_handler_t & handler){
            std::string name(PkgTy_::default_instance().GetDescriptor()->full_name());
            udp_recv_handler_t h = std::bind(udp_recv_callback<Message, PkgTy_>::recv_handler, _1, _2, _3, handler);
            m_oUDPHandlers.insert(std::make_pair(name, h));
        }

    protected:
        typedef std::shared_ptr<Message> message_ptr;
        class pb_tcp_pkg_handler : public tcp_pkg_handler {
        public:
            pb_tcp_pkg_handler(protobuf_pkg_hub *pHub);

            virtual ~pb_tcp_pkg_handler();

            virtual void handle_pkg(tcp_connection_base *pFrom, const shared_buffer &buf);

            virtual bool is_pkg_to_handle(uint32_t pkg_id);

        protected:
            protobuf_pkg_hub *m_pHub;
        };

        class pb_udp_pkg_handler : public udp_pkg_handler {
        public:
            pb_udp_pkg_handler(protobuf_pkg_hub *pHub);

            virtual ~pb_udp_pkg_handler();

            virtual void handle_pkg(udp_point *pPoint, const shared_buffer &buf, const udp_endpoint &from);

            virtual bool is_pkg_to_handle(uint32_t pkg_id);

        protected:
            protobuf_pkg_hub *m_pHub;
        };

        void handle_tcp_pkg(tcp_connection_base *pFrom, const shared_buffer &buf);

        void handle_udp_pkg(udp_point *pPoint, const shared_buffer &buf, const udp_endpoint &from);

        message_ptr get_pkg(const shared_buffer &buf);

    protected:

        typedef std::function<void (message_ptr, tcp_connection_base *)> tcp_recv_handler_t;
        typedef std::function<void (message_ptr, udp_point *, udp_endpoint) > udp_recv_handler_t;
        typedef std::function<void (message_ptr) > pkg_recv_handler_t;
        typedef std::map<std::string, tcp_recv_handler_t> tcp_handlers_t;
        typedef std::map<std::string, udp_recv_handler_t> udp_handlers_t;
        typedef std::map<std::string, pkg_recv_handler_t> pkg_handlers_t;
        tcp_pkg_handler * m_pTCPPkgHandler;
        udp_pkg_handler * m_pUDPPkgHandler;
        tcp_handlers_t m_oTCPHandlers;
        udp_handlers_t m_oUDPHandlers;
        pkg_handlers_t m_oPkgHandlers;
    };

    void send_message(tcp_connection_base * p_from, const std::shared_ptr<Message>& p_msg);
    void send_message(udp_point * p_from, const udp_endpoint & to, const std::shared_ptr<Message> & p_msg);

}
