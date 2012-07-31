#ifndef UTILS_LOG_LOG_MODULE_H
#define UTILS_LOG_LOG_MODULE_H

#include "utils/log/logger.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

namespace utl{
namespace details{


template <typename MTy_>
class LogModule
{
private:
    LogModule(){};
public:
	static void 	enableLog()	{ 
		MTy_::enableLog();		
	} 
	
	LogModule(std::string strLogger, const char * format, ...)
	: m_strModuleName()
	, m_strLogger(strLogger)
	, m_strLogEvent()
	{
		va_list vs;
		char buf[1024];
		char tstr[1024];
		va_start(vs,format);
		vsprintf(buf,format,vs);
		//sprintf(tstr, buf);
		va_end(vs); 
		m_strLogEvent = std::string(buf);
		MTy_ t;
		m_strModuleName = t.module_name;
	}
	

    virtual ~LogModule()
    {
        utl::Logger<0> *p = utl::Logger<0>::getInstance();
        if(p && p->isModuleLogged(m_strModuleName))
        {
#ifdef LOG_4_CPLUS
        	LOG_INFO(m_strModuleName+"     "+m_strLogger, m_strLogEvent);
#elif ENABLE_LOG_CONSOLE
            std::cout<<m_strLogger<<"  "<<m_strLogEvent<<std::endl;
#endif
        }
    };

private:
    std::string         m_strModuleName;
    std::string         m_strLogger;
    std::string         m_strLogEvent;
};

}//end namespace details
}//end namespace utl

#endif

