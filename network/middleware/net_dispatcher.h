#ifndef _NETWORK_MIDDLEWARE_NET_DISPATCHER_H_
#define _NETWORK_MIDDLEWARE_NET_DISPATCHER_H_
#include "common.h"
#include "common/blocking_queue.h"

namespace ffnet
{
namespace details
{
class NetDispatcher
{
private:
    NetDispatcher();
public:
    typedef boost::function<void ()>    Func_t;
    static boost::shared_ptr<NetDispatcher>     instance();

    ~NetDispatcher();
    void                dispatch(Func_t  f);

    void                stop();
protected:
    void                run();

protected:
    
    ffnet::BlockingQueue<Func_t>        m_oDispatchTasks;
    boost::thread                    m_oDispatchThread;

    volatile bool        m_bIsStopped;
    boost::mutex        m_oMutex;
    static boost::shared_ptr<NetDispatcher>    s_pInstance;
};//end class PkgDispatcher
}//end namespace details
}//end namespace ffnet
#endif
