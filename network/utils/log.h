/*
 * log.h
 *
 *  Created on: 2012-6-24
 *      Author: AthrunArthur
 */

#ifndef _UTILS_LOG_H_
#define _UTILS_LOG_H_

#include "utils/log/log_module.h"
#include "utils/log/logger.h"
#include <string>


#ifdef ENABLE_LOG_CONSOLE
#define DEF_LOG_MODULE(module)  \
    struct _ ## module {      \
        _ ## module() : module_name(#module){} \
        std::string  module_name; \
        static void 	enableLog()	{ utl::Logger<0>::getInstance()->logModule(#module); } \
    };   \
    typedef utl::details::LogModule< _##module >     log_ ## module; \

#else

#define DEF_LOG_MODULE(module)  \
	struct log_ ## module { \
		log_ ## module(std::string, const char * format, ...){}; \
		static void enableLog(){} \
	};

#endif

#endif /* LOG_H_ */
