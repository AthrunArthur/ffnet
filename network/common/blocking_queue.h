#pragma once
#include "common.h"
#include <boost/thread/condition_variable.hpp>
#include <boost/thread.hpp>
#include <queue>
namespace ffnet
{
    template<class Ty>
    class BlockingQueue : boost::noncopyable
    {
    public:
        BlockingQueue(){
        }

        void                            push_back(const Ty & val)
        {
            m_oMutex.lock();
            m_oContainer.push(val);
            size_t s = m_oContainer.size();
            m_oMutex.unlock();

            if(s == 1)
            {
                m_oCond.notify_all();
            }
        }

        void                            pop(Ty & val)
        {
            boost::unique_lock<boost::mutex> ul(m_oMutex);

            while(m_oContainer.empty())
            {
                m_oCond.wait(ul);
            }

            val = m_oContainer.front();
            m_oContainer.pop();
        }

        size_t                        size() const
        {
            boost::unique_lock<boost::mutex> ul(m_oMutex);
            return m_oContainer.size();
        }
        bool                            empty() const
        {
            boost::unique_lock<boost::mutex> ul(m_oMutex);
            return m_oContainer.empty();
        }
    protected:
        mutable boost::mutex                    m_oMutex;
        mutable boost::condition_variable    m_oCond;
        std::queue<Ty>                m_oContainer;
    };//end class CondPopQueue
}

