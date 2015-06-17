#pragma once
#include "common.h"
#include <boost/any.hpp>

namespace ffnet
{
class event_handler
{
public:
    template<class ETy_>
    void listen(const typename ETy_::Handler_t & h)
    {
        size_t v = ETy_::identifier;
        m_oHandlers[v] = boost::any(h);
    }

#define RESTORE_HANDLER  size_t v = ETy_::identifier;  \
    if(m_oHandlers.find(v) == m_oHandlers.end())   \
        return ;  \
    typename ETy_::Handler_t h = boost::any_cast<typename ETy_::Handler_t>(m_oHandlers[v]);

    template<class ETy_, class T1>
    void triger(const T1 & t1)
    {
        RESTORE_HANDLER
        h(t1);
    }

    template<class ETy_, class T1, class T2>
    void triger(const T1 & t1, const T2 & t2)
    {
        RESTORE_HANDLER
        h(t1, t2);
    }

    template<class ETy_, class T1, class T2, class T3>
    void triger(const T1 & t1, const T2 & t2, const T3 & t3)
    {
        RESTORE_HANDLER
        h(t1, t2, t3);
    }
#undef RESTORE_HANDLER

protected:
    typedef std::map<size_t, boost::any> ETHandlers_t;
    ETHandlers_t m_oHandlers;
};
}//end ffnet

