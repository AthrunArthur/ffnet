#ifndef _NETWORK_FRAME_WORK_NET_NERVURE_FROM_FILE_H_
#define _NETWORK_FRAME_WORK_NET_NERVURE_FROM_FILE_H_
#include "common.h"
#include "nervures/typed_net_nervure.h"
#include "framework/nervure_config.h"

namespace ffnet
{
template<class NTy>
class NetNervureFromFile : public NTy
{
public:
    NetNervureFromFile(String conf)
        : NTy()
        , m_pConfigure(new NervureConfigure(conf))
    {
        NervureConfigure & nc = *(m_pConfigure.get());

        int8_t tcp_server_flag = nc.get<int8_t>("conf.open-tcp-server");
        int8_t udp_server_flag = nc.get<int8_t>("conf.open-udp-server");
        int8_t tcp_client_flag = nc.get<int8_t>("conf.open-tcp-client");

        if(tcp_server_flag) {
            String ip = nc.get<String>("tcp-server.ip");
            uint16_t port = nc.get<uint16_t>("tcp-server.port");
            NTy::addTCPServer(ip, port);
        }
        if(udp_server_flag) {
            String ip = nc.get<String>("udp-server.ip");
            uint16_t port = nc.get<uint16_t>("udp-server.port");
            NTy::addUDPPoint(ip, port);
        }
        if(tcp_client_flag) {
            String ip = nc.get<String>("tcp-client.target-svr-ip-addr");
            uint16_t port = nc.get<uint16_t>("tcp-client.target-svr-port");

            NTy::addTCPClient(ip, port);
        }
    }

    inline NervureConfigurePtr_t &        NervureConf() {
        return m_pConfigure;
    }
protected:
    NervureConfigurePtr_t        m_pConfigure;
};//end class NetNervureFromFile
}//end namespace ffnet
#endif
