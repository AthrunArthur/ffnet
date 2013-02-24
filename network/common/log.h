#ifndef FFNET_COMMON_LOG_H_
#define FFNET_COMMON_LOG_H_
#include "common/log/logger.h"
#include "common/log/logwriter.h"
namespace ffnet
{
	class Log : boost::noncopyable
	{
	public:
	enum LogLevel
	{
		TRACE,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL,
		NUM_LOG_LEVELS,
	};
	public:
		static void		init(LogLevel ll, const char * logfile);
		static LogLevel	s_LogLevel;
	};//end class Log
}//end namespace ffnet

//template<class MTy_>
//class Logger : public ffnet::details::LoggerDoNothing{};
	

#define DEF_LOG_MODULE(module) struct log_ ##module{}; 

#define ENABLE_LOG_MODULE(module) \
	namespace ffnet{namespace details{ \
  template<> struct enable_traits<log_ ##module> { \
	static const bool value = true; };  \
	}}
	

#define LOG_TRACE(module)  if(::ffnet::Log::s_LogLevel <= ::ffnet::Log::TRACE) \
	ffnet::Logger<ffnet::details::enable_traits<log_ ## module>::value >()\
	<<"\tTRACE"<<"\t"<<#module<<"\t"
	
#define LOG_DEBUG(module)  if(::ffnet::Log::s_LogLevel <= ::ffnet::Log::DEBUG) \
	ffnet::Logger<ffnet::details::enable_traits<log_ ## module>::value >()\
	<<"\tDEBUG"<<"\t"<<#module<<"\t"
	
#define LOG_INFO(module)  if(::ffnet::Log::s_LogLevel <= ::ffnet::Log::INFO) \
	ffnet::Logger<ffnet::details::enable_traits<log_ ## module>::value >()\
	<<"\tINFO"<<"\t"<<#module<<"\t"

#define LOG_WARN(module)  if(::ffnet::Log::s_LogLevel <= ::ffnet::Log::WARN) \
	ffnet::Logger<ffnet::details::enable_traits<log_ ## module>::value >()\
	<<"\tWARN"<<"\t"<<#module<<"\t"

#define LOG_ERROR(module)  if(::ffnet::Log::s_LogLevel <= ::ffnet::Log::ERROR) \
	ffnet::Logger<ffnet::details::enable_traits<log_ ## module>::value >()\
	<<"\tERROR"<<"\t"<<#module<<"\t"

#define LOG_FATAL(module)  if(::ffnet::Log::s_LogLevel <= ::ffnet::Log::FATAL) \
	ffnet::Logger<ffnet::details::enable_traits<log_ ## module>::value >()\
	<<"\tFATAL"<<"\t"<<#module<<"\t"

#endif