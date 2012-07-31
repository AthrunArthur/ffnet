#include "log.h"

#ifdef ENABLE_LOG_CONSOLE
void initialize_log(const char * file)
{
	utl::Logger<0>::initial(file);
	utl::Logger<0> * pInstance = utl::Logger<0>::getInstance();
	log_connection::enableLog();
	log_frmwk::enableLog();
	log_tcp_client::enableLog();
	log_udp_server::enableLog();
	log_tcp_server::enableLog();
	log_conf::enableLog();
}

#else
void initialize_log(const char * file)
{
}
#endif