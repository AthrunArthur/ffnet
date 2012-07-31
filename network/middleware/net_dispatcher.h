#ifndef _NETWORK_MIDDLEWARE_NET_DISPATCHER_H_
#define _NETWORK_MIDDLEWARE_NET_DISPATCHER_H_
#include "common.h"
#include "common/cond_pop_queue.h"

namespace ffnet
{
namespace details
{
class NetDispatcher
{
private:
    NetDispatcher();
public:
	typedef boost::function<void ()>	Func_t;
    static boost::shared_ptr<NetDispatcher> 	instance();

    void				dispatch(Func_t  f);

protected:
    void				run();

protected:
    
    ffnet::CondPopQueue<Func_t>		m_oDispatchTasks;
    boost::thread					m_oDispatchThread;

    bool		m_bIsStopped;
    static boost::shared_ptr<NetDispatcher>	s_pInstance;
};//end class PkgDispatcher
}//end namespace details
}//end namespace ffnet
#endif
