#ifndef _NETWORK_LOG_H_
#define _NETWORK_LOG_H_



#include <utils/log.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
DEF_LOG_MODULE(conf)
DEF_LOG_MODULE(tcp_server)
DEF_LOG_MODULE(udp_server)
DEF_LOG_MODULE(tcp_client)
DEF_LOG_MODULE(connection)
DEF_LOG_MODULE(frmwk)

void initialize_log(const char * file); 

#endif