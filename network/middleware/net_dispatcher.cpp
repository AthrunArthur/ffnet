
#include "middleware/net_dispatcher.h"
#include <boost/thread.hpp>
#include <cassert>
#include "log.h"

namespace ffnet
{
namespace details
{
boost::shared_ptr<NetDispatcher> NetDispatcher::s_pInstance;

NetDispatcher::NetDispatcher()
    : m_oDispatchTasks()
    , m_oDispatchThread()
    , m_bIsStopped(false)
{
}

boost::shared_ptr< NetDispatcher > NetDispatcher::instance()
{
    if(s_pInstance == NULL) {
        s_pInstance = boost::shared_ptr<NetDispatcher>(new NetDispatcher());
        s_pInstance->m_oDispatchThread = boost::thread(boost::bind(&NetDispatcher::run, s_pInstance.get()));
    }
    return s_pInstance;
}

void NetDispatcher::run()
{
    Func_t f;
    while(!m_bIsStopped) {
        m_oDispatchTasks.pop(f);
        f();
    }
}

void NetDispatcher::dispatch(Func_t f)
{
	FFNET_DEBUG(
    log_frmwk("NetDispatcher", "dispatch(), a pkg is dispatched to pNervure!");
	)
    m_oDispatchTasks.push_back(f);
}

}//end namespace details;
}//end namespace ffnet