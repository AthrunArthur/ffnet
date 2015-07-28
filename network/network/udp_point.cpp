#include "network/udp_point.h"
#include "network/events.h"
#include "common/defines.h"
#include "middleware/event_handler.h"
#include "middleware/pkg_packer.h"

namespace ffnet {
    using namespace event;
    using namespace event::more;

    udp_point::udp_point(io_service &ioservice, pkg_packer *bs,
                         event_handler *eh, const std::vector<udp_pkg_handler *> & rph, const udp_endpoint &ep)
            : asio_point(ioservice, bs, eh), m_oSocket(ioservice, ep), m_pRPH(rph){}

    udp_point::~udp_point(){ }
    net_udp_point::net_udp_point(io_service &ioservice, pkg_packer *bs,
                       event_handler *eh, const std::vector<udp_pkg_handler *> & rph, const udp_endpoint &ep)
            : udp_point(ioservice, bs, eh, rph, ep), m_bIsSending(false) {
        m_iPointState = state_valid;
        start_recv();
    }

    net_udp_point::~net_udp_point() {
        if (m_iPointState == state_valid)
            close();
        for (recv_buffer_t::iterator it = m_oRecvBuffer.begin(); it != m_oRecvBuffer.end(); it++) {
            delete it->second;
        }
    }

    void net_udp_point::send(const package_ptr &pkg, const udp_endpoint &pEndpoint) {
        if (m_iPointState != state_valid) {
            //m_pEH->triger<udp_send_recv_exception>(pkg, pEndpoint);
            return;
        }
        m_oSendTasks.push(boost::bind(&net_udp_point::actual_send_pkg, this, pkg, pEndpoint));
        if (!m_bIsSending) {
            m_bIsSending = true;
            start_send();
        }
    }

    void net_udp_point::start_send() {
        func_t f;
        if (!m_oSendTasks.empty()) {
            f = m_oSendTasks.front();
            m_oSendTasks.pop();
            f();
        }
        else {
            m_bIsSending = false;
        }
    }

    void net_udp_point::actual_send_pkg(const package_ptr &pkg, const udp_endpoint &ep) {
        //m_pHandler->onUDPStartSend(this);
        m_pPacker->pack(m_oSendBuffer, pkg);
        m_oSendEndpoint = ep;
        m_oSocket.async_send_to(boost::asio::buffer(m_oSendBuffer.readable()), m_oSendEndpoint,
                                boost::bind(&net_udp_point::handle_pkg_sent, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }

    void net_udp_point::handle_pkg_sent(const boost::system::error_code &ec, std::size_t bytes_transferred) {
        if (!ec) {
            m_pEH->triger<udp_send_data_succ>(this, bytes_transferred);
            m_oSendBuffer.erase_buffer(bytes_transferred);
            LOG(INFO) << "pkg sent " << bytes_transferred <<
                                  " bytes, to ";//<<getRemoteEndpointPtr()->to_str();
            if (m_oSendBuffer.length() != 0) {
                m_oSocket.async_send_to(boost::asio::buffer(m_oSendBuffer.readable()), m_oSendEndpoint,
                                        boost::bind(&net_udp_point::handle_pkg_sent, this,
                                                    boost::asio::placeholders::error,
                                                    boost::asio::placeholders::bytes_transferred));
            } else {
                start_send();
            }
        } else {
            m_iPointState = state_error;
            LOG(WARNING) << "handle_pkg_sent(), Get error " << ec.message();
            m_pEH->triger<udp_send_data_error>(this, ec);
        }
    }

    void net_udp_point::start_recv() {
        m_oSocket.async_receive_from(
                boost::asio::buffer(m_oTempBuffer.writeable()), m_oRecvEndPoint,
                boost::bind(&net_udp_point::handle_received_pkg, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
    }

    void net_udp_point::handle_received_pkg(const boost::system::error_code &error, size_t bytes_transferred) {
        if (!error) {
            m_pEH->triger<udp_recv_data_succ>(this, bytes_transferred);
            m_oTempBuffer.filled() += bytes_transferred;
            LOG(INFO) << "recv pkg: " << bytes_transferred << " bytes, from " << m_oRecvEndPoint.address().to_string();
            if (m_oRecvBuffer.find(m_oRecvEndPoint) == m_oRecvBuffer.end()) {
                net_buffer *pBuffer = new net_buffer();
                if (pBuffer == NULL) {
                    assert(0 && "Allocation failed!");
                    exit(-1);
                }
                m_oRecvBuffer.insert(std::make_pair(m_oRecvEndPoint, pBuffer));
            }
            net_buffer *pBuffer = m_oRecvBuffer[m_oRecvEndPoint];
            pBuffer->write_buffer(m_oTempBuffer.buffer(), bytes_transferred);
            m_oTempBuffer.erase_buffer(bytes_transferred);
            slice_and_dispatch_pkg(pBuffer, m_oRecvEndPoint);
            start_recv();
        } else {
            m_iPointState = state_error;
            LOG(WARNING) << "handle_received_pkg(), Get error " << error.message() << " from " <<
                                  m_oRecvEndPoint.address().to_string();
            m_pEH->triger<udp_recv_data_error>(this, error);
        }
    }

    void net_udp_point::slice_and_dispatch_pkg(net_buffer *pBuf, const udp_endpoint &ep) {
        std::list<shared_buffer> sbs = m_pPacker->split(*pBuf);
        for (std::list<shared_buffer>::iterator it = sbs.begin();
             it != sbs.end(); ++it) {
            shared_buffer sb = *it;
            const char *pBuf = sb.buffer();
            uint32_t pkg_id;
            ffnet::deseralize(pBuf, pkg_id);
            bool got_pkg_handler = false;
            std::map<uint32_t, udp_pkg_handler *>::iterator cit = m_oRPHCache.find(pkg_id);
            if(cit != m_oRPHCache.end())
            {
                got_pkg_handler = true;
                udp_pkg_handler * ph = cit->second;
                ph->handle_pkg(this, sb, ep);
            }else {
                for (size_t i = 0; i < m_pRPH.size(); ++i) {
                    if (m_pRPH[i]->is_pkg_to_handle(pkg_id)) {
                        m_oRPHCache.insert(std::make_pair(pkg_id, m_pRPH[i]));
                        m_pRPH[i]->handle_pkg(this, sb, ep);
                        got_pkg_handler = true;
                    }
                }
            }

            if(!got_pkg_handler){
                LOG(WARNING)<<"slice_and_dispatch_pkg(), cannot find handler for pkg id: "<<pkg_id;
            }
        }
    }

    void net_udp_point::close() {
        m_oSocket.close();
        m_iPointState = state_closed;
    }

    void net_udp_point::ticktack() {
        //!TODO: we should clean the m_oRecvBuffers after sometime!.
    }
}//end namespace ffnet
