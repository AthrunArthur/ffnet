#pragma once
#include "common.h"
#include <condition_variable>
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
            std::unique_lock<std::mutex> ul(m_oMutex);

            while(m_oContainer.empty())
            {
                m_oCond.wait(ul);
            }

            val = m_oContainer.front();
            m_oContainer.pop();
        }

        size_t                        size() const
        {
            std::unique_lock<std::mutex> ul(m_oMutex);
            return m_oContainer.size();
        }
        bool                            empty() const
        {
            std::unique_lock<std::mutex> ul(m_oMutex);
            return m_oContainer.empty();
        }
    protected:
        mutable std::mutex                    m_oMutex;
        mutable std::condition_variable    m_oCond;
        std::queue<Ty>                m_oContainer;
    };//end class CondPopQueue
}

