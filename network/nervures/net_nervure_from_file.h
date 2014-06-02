#ifndef _NETWORK_FRAME_WORK_NET_NERVURE_FROM_FILE_H_
#define _NETWORK_FRAME_WORK_NET_NERVURE_FROM_FILE_H_
#include "common.h"
#include "nervures/typed_net_nervure.h"
#include "framework/nervure_config.h"

namespace ffnet
{
    class NetNervureFromFile : public ffnet::details::TypedNetNervure
    {
    public:
        NetNervureFromFile(String conf);
		
        inline NervureConfigurePtr_t &		NervureConf(){return m_pConfigure;}
    protected:
        NervureConfigurePtr_t		m_pConfigure; 
    };//end class NetNervureFromFile
}//end namespace ffnet
#endif
