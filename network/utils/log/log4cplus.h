#ifndef UTILS_LOG_LOG_4_CPLUS_H
#define UTILS_LOG_LOG_4_CPLUS_H




#ifdef LOG_4_CPLUS

#include <log4cplus/logger.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/ndc.h>
#include <log4cplus/helpers/loglog.h>
#ifdef UNICODE
#	define LOG_FATAL(logger,logEvent) LOG4CPLUS_FATAL(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(logger)), logEvent)
#	define LOG_INFO(logger,logEvent) LOG4CPLUS_INFO(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(logger)), logEvent)
#	define LOG_DEBUG(logger,logEvent) LOG4CPLUS_DEBUG(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(logger)), logEvent)
#	define LOG_WARN(logger,logEvent) LOG4CPLUS_WARN(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(logger)), logEvent)
#	define LOG_ERROR(logger,logEvent) LOG4CPLUS_ERROR(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(logger)), logEvent)
# define LOG_TRACE(logger,logEvent) LOG4CPLUS_TRACE(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(logger)), logEvent)
#else
#	define LOG_FATAL(logger,logEvent) LOG4CPLUS_FATAL(log4cplus::Logger::getInstance(logger), logEvent)
#	define LOG_INFO(logger,logEvent) LOG4CPLUS_INFO(log4cplus::Logger::getInstance(logger), logEvent)
#	define LOG_DEBUG(logger,logEvent) LOG4CPLUS_DEBUG(log4cplus::Logger::getInstance(logger), logEvent)
#	define LOG_WARN(logger,logEvent) LOG4CPLUS_WARN(log4cplus::Logger::getInstance(logger), logEvent)
#	define LOG_ERROR(logger,logEvent) LOG4CPLUS_ERROR(log4cplus::Logger::getInstance(logger), logEvent)
# define LOG_TRACE(logger,logEvent) LOG4CPLUS_TRACE(log4cplus::Logger::getInstance(logger), logEvent)
#endif//UNICODE





#else


#	define LOG_FATAL(logger,logEvent)
#	define LOG_INFO(logger,logEvent)
#	define LOG_DEBUG(logger,logEvent)
#	define LOG_WARN(logger,logEvent)
#	define LOG_ERROR(logger,logEvent)
# define LOG_TRACE(logger,logEvent)



#endif//NO_LOG_4_CPLUS


#endif
