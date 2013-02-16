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

template<class MTy_>
class Logger : public ffnet::details::LoggerDoNothing{};
	
#define DEF_LOG_MODULE( module) struct log_ ## module{};

#define ENABLE_LOG_MODULE(module) \
	template<> \
	class Logger<log_ ## module> : public ::ffnet::details::LoggerDoLog{ \
	public: \
		virtual const char *		 getModuleName(){return #module;} \
	};
	

#define LOG_TRACE(module)  if(::ffnet::Log::s_LogLevel <= ::ffnet::Log::TRACE) \
	::Logger<log_ ## module>()<<"\tTRACE"<<"\t"<<#module<<"\t"
	
#define LOG_DEBUG(module)  if(::ffnet::Log::s_LogLevel <= ::ffnet::Log::DEBUG) \
	::Logger<log_ ## module>()<<"\tDEBUG"<<"\t"<<#module<<"\t"
	
#define LOG_INFO(module)  if(::ffnet::Log::s_LogLevel <= ::ffnet::Log::INFO) \
	::Logger<log_ ## module>()<<"\tINFO"<<"\t"<<#module<<"\t"

#define LOG_WARN(module)  if(::ffnet::Log::s_LogLevel <= ::ffnet::Log::WARN) \
	::Logger<log_ ## module>()<<"\tWARN"<<"\t"<<#module<<"\t"

#define LOG_ERROR(module)  if(::ffnet::Log::s_LogLevel <= ::ffnet::Log::ERROR) \
	::Logger<log_ ## module>()<<"\tERROR"<<"\t"<<#module<<"\t"

#define LOG_FATAL(module)  if(::ffnet::Log::s_LogLevel <= ::ffnet::Log::FATAL) \
	::Logger<log_ ## module>()<<"\tFATAL"<<"\t"<<#module<<"\t"

#endif