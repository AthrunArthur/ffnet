
#include "middleware/net_dispatcher.h"
#include <boost/thread.hpp>
#include <cassert>
#include "common/defines.h"

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

NetDispatcher::~NetDispatcher()
{
    if(m_bIsStopped == false)
    {
        stop();
    }
}

boost::shared_ptr< NetDispatcher > NetDispatcher::instance()
{
    if(s_pInstance == NULL) {
        s_pInstance = boost::shared_ptr<NetDispatcher>(new NetDispatcher());
        s_pInstance->m_oDispatchThread = boost::thread(boost::bind(&NetDispatcher::run, s_pInstance.get()));
    }
    return s_pInstance;
}

static void usedToTrigerStop()
{
	//As m_oDispatchTasks.pop() will block if there is no task,
	//so we need another task to let it return and check if it's stopped.
	//So need to do nothing here, it's just a tricky.
}
void NetDispatcher::stop()
{
    m_oMutex.lock();
    m_bIsStopped = true;
    m_oMutex.unlock();
    m_oDispatchTasks.push_back(usedToTrigerStop);
    m_oDispatchThread.join();
}

void NetDispatcher::run()
{
    Func_t f;
    m_oMutex.lock();
    while(!m_bIsStopped) {
        m_oMutex.unlock();
        m_oDispatchTasks.pop(f);
        f();
        m_oMutex.lock();
    }
    m_oMutex.unlock();
}

void NetDispatcher::dispatch(Func_t f)
{
    LOG_DEBUG(frmwk)<<"NetDispatcher::dispatch() " << "a pkg is dispatched to pNervure!";
    m_oDispatchTasks.push_back(f);
}

}//end namespace details;
}//end namespace ffnet
