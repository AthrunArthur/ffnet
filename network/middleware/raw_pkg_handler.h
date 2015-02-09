#ifndef _NETWORK_MIDDLEWARE_RAW_PKG_HANDLER_H
#define _NETWORK_MIDDLEWARE_RAW_PKG_HANDLER_H

#include "network/end_point.h"
#include "common/shared_buffer.h"
#include "network/asio_connection.h"

namespace ffnet
{

class RawPkgHandler
{
public:
    virtual void handle_raw_pkg(ASIOConnection * from, const SharedBuffer & buf) = 0;
};

}
#endif // _NETWORK_MIDDLEWARE_RAW_PKG_HANDLER_H

