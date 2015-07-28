#pragma once
#include "common.h"
#include "common/shared_buffer.h"
#include "middleware/package.h"
#include <list>

namespace ffnet
{
    class net_buffer;
    class pkg_packer
    {
    public:
        virtual ~pkg_packer(){};
        virtual std::list<shared_buffer>        split(net_buffer &oRecvBuffer) = 0;
        virtual void                    pack(net_buffer & oSendBuffer, const char *pBuf, size_t len) = 0;
        virtual void                    pack(net_buffer & oSendBuffer, const package_ptr & pkg) = 0;
    };//end class PkgPacker
    typedef boost::shared_ptr<pkg_packer> pkg_packer_ptr;

    class length_packer : public : pkg_packer{
      public:
        virtual ~length_packer();
        virtual std::list<shared_buffer>        split(net_buffer &oRecvBuffer);
        virtual void                    pack(net_buffer & oSendBuffer, const char *pBuf, size_t len);
        virtual void                    pack(net_buffer & oSendBuffer, const package_ptr & pkg);
    };
}//end namespace ffnet
