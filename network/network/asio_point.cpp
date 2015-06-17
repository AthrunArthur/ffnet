#include "network/asio_point.h"

namespace ffnet
{
    asio_point::asio_point(io_service &ioservice, pkg_packer * bs, event_handler *eh)
            : m_iPointState(state_init)
            , m_oIOService(ioservice)
            , m_pPacker(bs)
            , m_pEH(eh){ }

    asio_point::~asio_point() { }

}//end namespace ffnet
