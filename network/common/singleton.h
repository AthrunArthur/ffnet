#pragma once
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

//!Useage
// class Mysingleton : public ffnet::Singleton<MySingleton>
// { };
namespace ffnet{
    template<typename T>
    class singleton: boost::noncopyable
    {
    public:
        static T& instance_ref()
        {
            boost::call_once(s_oOnce, boost::bind(singleton<T>::init));
            return *s_pInstance;
        }
        static boost::shared_ptr<T> instance_ptr()
        {
            boost::call_once(s_oOnce, boost::bind(singleton<T>::init));
            return s_pInstance;
        }
    protected:
        singleton(){}
    private:
        static void init()
        {
            s_pInstance = boost::shared_ptr<T>(new T());
        }

    protected:
        static boost::shared_ptr<T>        s_pInstance;
        static boost::once_flag        s_oOnce;
    };
    template <typename T>
    boost::shared_ptr<T> singleton<T>::s_pInstance;
    template <typename T>
    boost::once_flag        singleton<T>::s_oOnce = BOOST_ONCE_INIT;
}//end namespace ffnet
