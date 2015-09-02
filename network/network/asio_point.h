#pragma once
#include "common.h"
#include "network/net_buffer.h"

namespace ffnet
{

    class pkg_packer;
    class event_handler;
    using asio::io_service;

    class asio_point : public boost::noncopyable
    {
    public:
        enum point_state{
            state_init,
            state_valid,
            state_closed,
            state_error,
        };

        asio_point(io_service & ioservice, pkg_packer *  bs, event_handler* eh);
        virtual ~asio_point();

        inline pkg_packer *        get_pkg_packer() const {return m_pPacker;}
        inline io_service &            ioservice() {return m_oIOService;}
        inline const io_service &      ioservice() const {return m_oIOService;}
        inline event_handler*          get_event_handler() const{return m_pEH;}

    protected:
        io_service &       m_oIOService;
        pkg_packer *        m_pPacker;
        event_handler*     m_pEH;
        point_state m_iPointState;

    };
    typedef std::shared_ptr<asio_point>        asio_point_ptr;
}//end namespace ffnet
