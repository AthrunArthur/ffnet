#ifndef FFNET_COMMON_SINGLETON_H_
#define FFNET_COMMON_SINGLETON_H_
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

//!Useage 
// class MySingleton : public ffnet::Singleton<MySingleton>
// { };
namespace ffnet{
	template<typename T>
	class Singleton: boost::noncopyable
	{
	public:
		static T& instance_ref()
		{
			boost::call_once(s_oOnce, boost::bind(Singleton<T>::init));
			return *s_pInstance;
		}
		static boost::shared_ptr<T> instance_ptr()
		{
			boost::call_once(s_oOnce, boost::bind(Singleton<T>::init));
			return s_pInstance;
		}
	protected:
		Singleton(){}
	private:
		static void init()
		{
			s_pInstance = boost::shared_ptr<T>(new T());
		}
		
	protected:
		static boost::shared_ptr<T>		s_pInstance;
		static boost::once_flag		s_oOnce;
	};
	template <typename T>
	boost::shared_ptr<T> Singleton<T>::s_pInstance;
	template <typename T>
	boost::once_flag		Singleton<T>::s_oOnce = BOOST_ONCE_INIT;
}//end namespace ffnet
#endif
