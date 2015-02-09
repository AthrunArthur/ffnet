#include "nervures/thread_nervure.h"
#include "network/asio_connection.h"
#include "network/tcp_client.h"
#include "network/tcp_connection_base.h"
#include "network/udp_point.h"
#include "network/tcp_server.h"

namespace ffnet{
void ThreadNervure::send(PackagePtr_t pPkg, EndpointPtr_t pEP)
    {
        m_oIOService.dispatch(boost::bind(&ThreadNervure::send_func, this, pPkg, pEP));
    }
    
    void ThreadNervure::run()
    {
        boost::thread thrd(boost::bind(&boost::asio::io_service::run, &m_oIOService));
        thrd.join();
    }
    void ThreadNervure::stop()
    {
        TypedNetNervure::stop();
    }
    
    void ThreadNervure::send_func(PackagePtr_t pPkg, EndpointPtr_t pEP)
    {
        //TODO(A.A.), we can optimize this if we need.
        if(pEP->is_tcp())
        {
            for(TCPClients_t::iterator it = m_oClients.begin(); it!=m_oClients.end(); ++it)
            {
                TCPClient_ptr pClient = *it;
                EndpointPtr_t p = pClient->getRemoteEndpointPtr();
                if(p->operator==(*pEP))
                {
                    pClient->send(pPkg);
                    return ;
                }
            }
            for(TCPConnections_t::iterator it = m_oTCPConnections.begin(); it!=m_oTCPConnections.end(); ++it)
            {
                TCPConnection_ptr pConn = *it;
                EndpointPtr_t p = pConn->getRemoteEndpointPtr();
                if(p->operator==(*pEP))
                {
                    pConn->send(pPkg);
                    return ;
                }
            }
        }
        else if(pEP->is_udp())
        {
            for(UDPPoints_t::iterator it = m_oUDPPoints.begin(); it != m_oUDPPoints.end(); ++it)
            {
                UDPPoint_ptr pUDP = *it;
                pUDP->send(pPkg, pEP);
                return ;
            }
        }
    }
}//end namespace ffnet