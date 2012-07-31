#ifndef UTILS_LOG_LOGGER_H
#define UTILS_LOG_LOGGER_H

#include <string>
#include <set>
#include "utils/log/log4cplus.h"


namespace utl {

///! For initialize the log system.
/// Usage: LogInitializer<log_level>("file_path");
template<int log_level = 0>
class Logger
{
protected:
	Logger(){};
	Logger(const char *path)
	{
#ifdef LOG_4_CPLUS
		/* step 1: Instantiate an appender object */
		//log4cplus::SharedAppenderPtr _append(new log4cplus::RollingFileAppender( logfilename.c_str(), 20*1024, 1000));
		log4cplus::SharedAppenderPtr _append(new log4cplus::FileAppender( path));//, 20*1024, 1000));
		//SharedAppenderPtr _append(new DailyRollingFileAppender(logfilename.c_str(), HOURLY, true, 1000));
		_append->setName("append for test");

		/* step 2: Instantiate a layout object */
		std::string pattern = "%-6p  %-15c %D %m %n ";

		std::auto_ptr<log4cplus::Layout> _layout(new log4cplus::PatternLayout(pattern));

		/* step 3: Attach the layout object to the appender */
		_append->setLayout( _layout );

		/* step 4: Instantiate a logger object */
		log4cplus::Logger _logger = log4cplus::Logger::getRoot();

		_logger.setLogLevel( log_level);

		/* step 5: Attach the appender object to the logger  */
		_logger.addAppender(_append);
#endif
	}; //end the constructor
public:
	static Logger<log_level> *			initial(const char *path)
	{
		if(!s_pInstance)
			s_pInstance = new Logger<log_level>(path);
		return s_pInstance;
	}

	static Logger<log_level> *			getInstance(){return s_pInstance;};

	void								logModule(const std::string &	strModuleName)
	{
		m_oLoggedModules.insert(strModuleName);
	}

	bool								isModuleLogged(const std::string & strModuleName)
	{
		if(m_oLoggedModules.find(strModuleName) == m_oLoggedModules.end())
			return false;
		return true;
	}

protected:
	typedef std::set<std::string >		ModuleNames;
	ModuleNames							m_oLoggedModules;
	static Logger<log_level> *			s_pInstance;
};//end class Logger

template <int log_level>
Logger<log_level> * Logger<log_level>::s_pInstance = NULL;

};//end namespace utl
#endif

