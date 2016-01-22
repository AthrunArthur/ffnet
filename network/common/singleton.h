#pragma once
#include "common.h"

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
            std::call_once(s_oOnce, std::bind(singleton<T>::init));
            return *s_pInstance;
        }
        static std::shared_ptr<T> instance_ptr()
        {
            std::call_once(s_oOnce, std::bind(singleton<T>::init));
            return s_pInstance;
        }
    protected:
        singleton(){}
    private:
        static void init()
        {
            s_pInstance = std::shared_ptr<T>(new T());
        }

    protected:
        static std::shared_ptr<T>        s_pInstance;
        static std::once_flag        s_oOnce;
    };
    template <typename T>
    std::shared_ptr<T> singleton<T>::s_pInstance;
    template <typename T>
    std::once_flag        singleton<T>::s_oOnce;
}//end namespace ffnet
