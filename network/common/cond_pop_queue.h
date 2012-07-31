#ifndef _NETWORK_COMMON_COND_POP_QUEUE_H_
#define _NETWORK_COMMON_COND_POP_QUEUE_H_
#include "common.h"
#include <boost/thread/condition_variable.hpp>
#include <queue>
namespace ffnet
{
	template<class Ty>
	class CondPopQueue
	{
	public:
		CondPopQueue(){}
		
		void							push_back(const Ty & val)
		{
			m_oMutex.lock();
			bool ef = m_oContainer.empty();
			m_oContainer.push(val);
			m_oMutex.unlock();
			
			if(ef)
			{
				m_oCond.notify_all();
			}
		}
		
		void							pop(Ty & val)
		{
			boost::unique_lock<boost::mutex> ul(m_oMutex);

			if(m_oContainer.empty())
			{
				m_oCond.wait(ul);
			}
			
			val = m_oContainer.front();
			m_oContainer.pop();
		}
		
		size_t						size() const
		{
			boost::unique_lock<boost::mutex> ul(m_oMutex);
			return m_oContainer.size();
		}
		bool							empty() const
		{
			boost::unique_lock<boost::mutex> ul(m_oMutex);
			return m_oContainer.empty();
		}
	protected:
		mutable boost::mutex					m_oMutex;
		mutable boost::condition_variable	m_oCond;
		std::queue<Ty>				m_oContainer;
	};//end class CondPopQueue
}

#endif