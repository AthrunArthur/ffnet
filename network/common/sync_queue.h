/*
 * sync_queue.h
 *
 *  Created on: 2012-6-24
 *      Author: AthrunArthur
 */

#ifndef _LED_AD_UTILS_SYNC_QUEUE_H_
#define _LED_AD_UTILS_SYNC_QUEUE_H_
#include <list>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/noncopyable.hpp>



namespace ffnet
{
#define GUARD_LOCK boost::lock_guard <boost::mutex> _l(m_oMutex)
template <class Ty_>
class sync_queue : public boost::noncopyable
{
public:
	typedef std::list<Ty_>		Container_t;
	typedef Ty_					Elem_t;
public:
	sync_queue()
	: m_oQueue()
	, m_oMutex(){}

	std::size_t		size()
	{
		GUARD_LOCK;
		return m_oQueue.size();
	}

	void			push(const Ty_& val)
	{
		GUARD_LOCK;
		m_oQueue.push_back(val);
	}

	bool			pop(Ty_ &val)
	{
		GUARD_LOCK;
		if(m_oQueue.empty())
			return false;

		val = m_oQueue.front();
		m_oQueue.pop_front();
		return true;
	}
	boost::mutex &			mutex(){return m_oMutex;}
	std::list<Ty_> &			content(){return m_oQueue;}
	
protected:
	std::list<Ty_> 			m_oQueue;
	mutable boost::mutex		m_oMutex;
};
#undef GUARD_LOCK
}


#endif /* SYNC_QUEUE_H_ */
