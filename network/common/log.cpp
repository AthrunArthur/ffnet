#include "common/log.h"
namespace ffnet
{
	 void		Log::init(LogLevel ll, const char * logfile)
		{
			s_LogLevel = ll;
			utl::Singleton<details::LogWriter>::instance_ref().run(logfile);
		}
		Log::LogLevel	Log::s_LogLevel;
}