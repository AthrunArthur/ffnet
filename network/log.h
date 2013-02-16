#ifndef _NETWORK_LOG_H_
#define _NETWORK_LOG_H_



//#include <utils/log.h>
#include "common/log.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>


DEF_LOG_MODULE(conf)
DEF_LOG_MODULE(tcp_server)
DEF_LOG_MODULE(udp_server)
DEF_LOG_MODULE(tcp_client)
DEF_LOG_MODULE(connection)
DEF_LOG_MODULE(frmwk)
DEF_LOG_MODULE(app)

ENABLE_LOG_MODULE(app)
ENABLE_LOG_MODULE(conf)
ENABLE_LOG_MODULE(tcp_server)
ENABLE_LOG_MODULE(udp_server)
ENABLE_LOG_MODULE(tcp_client)
ENABLE_LOG_MODULE(connection)
//ENABLE_LOG_MODULE(frmwk)


#endif