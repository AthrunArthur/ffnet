#include "handler/conn_holder_handler.h"

namespace ffnet
{
ConnectionHolderHandler::ConnectionHolderHandler()
    : m_oConnHolder()
    , m_oMutex()
{

}
ConnectionHolderHandler::~ConnectionHolderHandler()
{

}

void ConnectionHolderHandler::onConnectionError(TCPConnectionBase *pConn, const boost::system::error_code &ec)
{

}
void ConnectionHolderHandler::onGotConnection(TCPConnectionBasePtr_t pConn)
{
    boost::unique_lock<boost::mutex> _l(m_oMutex);
    m_oConnHolder.push_back(pConn);
}
void ConnectionHolderHandler::onRecvError(ASIOConnection *pConn, const boost::system::error_code &ec)
{
    if(pConn->TCPConnectionBasePointer() == NULL)
        return ;
    boost::unique_lock<boost::mutex> _l(m_oMutex);
RECHECK:

    for(ConnHolder_t::iterator it = m_oConnHolder.begin();
            it != m_oConnHolder.end();
            ++it) {
        if(it->get() == pConn) {
            m_oConnHolder.erase(it);
			pConn->close();
            goto RECHECK;
        }
    }
}
void ConnectionHolderHandler::onSendError(ASIOConnection *pConn, const boost::system::error_code &ec)
{
    if(pConn->TCPConnectionBasePointer() == NULL)
        return ;

    boost::unique_lock<boost::mutex> _l(m_oMutex);
RECHECK:

    for(ConnHolder_t::iterator it = m_oConnHolder.begin();
            it != m_oConnHolder.end();
            ++it) {
        if(it->get() == pConn) {
            m_oConnHolder.erase(it);
			pConn->close();
            goto RECHECK;
        }
    }
}

}//end namespace ffnet