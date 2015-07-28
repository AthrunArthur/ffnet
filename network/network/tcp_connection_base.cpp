#include "tcp_connection_base.h"
#include "common/defines.h"
#include "network/events.h"
#include "middleware/event_handler.h"
#include "middleware/pkg_packer.h"
#include "middleware/pkg_handler.h"

namespace ffnet {
    using namespace ::ffnet::event;
    using namespace ::ffnet::event::more;


    tcp_connection_base::tcp_connection_base(io_service &ioservice, pkg_packer * bs,
                                         event_handler *eh, const std::vector<tcp_pkg_handler *> & rph)
            : asio_point(ioservice, bs, eh), m_pRPH(rph) { }



    tcp_connection_base::~tcp_connection_base() {}


    net_tcp_connection_base::net_tcp_connection_base(io_service &ioservice, pkg_packer * bs,
                                             event_handler *eh, const std::vector<tcp_pkg_handler *> & rph)
            : tcp_connection_base(ioservice, bs, eh, rph), m_oSocket(ioservice), m_bIsSending(false){}

    net_tcp_connection_base::~net_tcp_connection_base() { if (m_iPointState == state_valid) close(); }

    void net_tcp_connection_base::send(const package_ptr &pkg) {
        if (m_iPointState != state_valid) {
            m_pEH->triger<tcp_pkg_send_failed>(this, pkg);
            return;
        }
        m_oToSendPkgs.push(pkg);
        if (!m_bIsSending) {
            m_bIsSending = true;
            start_send();
        }
    }

    void net_tcp_connection_base::start_send() {
        while (!m_oToSendPkgs.empty() && m_oSendBuffer.size() < 4096) {
            package_ptr pPkg = m_oToSendPkgs.front();
            m_oToSendPkgs.pop();
            m_pPacker->pack(m_oSendBuffer, pPkg);
        }
        if (m_oSendBuffer.length() != 0) {
            m_pEH->triger<tcp_start_send_stream>(this, boost::asio::buffer_cast<const char *>(m_oSendBuffer.readable()),
                                                 boost::asio::buffer_size(m_oSendBuffer.readable()));


            m_oSocket.async_write_some(boost::asio::buffer(m_oSendBuffer.readable()),
                                       boost::bind(&net_tcp_connection_base::handle_pkg_sent, this, //shared_from_this(),
                                                   boost::asio::placeholders::error,
                                                   boost::asio::placeholders::bytes_transferred));
        } else {
            m_bIsSending = false;
        }
    }

    void net_tcp_connection_base::handle_pkg_sent(const boost::system::error_code &ec, std::size_t bytes_transferred) {
        if (!ec) {
            m_pEH->triger<tcp_send_stream_succ>(this, bytes_transferred);
            m_oSendBuffer.erase_buffer(bytes_transferred);
            LOG(INFO)<<"pkg sent "<<bytes_transferred<<" bytes, to "<<m_oRemoteEndpoint.address().to_string();
            if (m_oSendBuffer.length() != 0) {
                m_pEH->triger<tcp_start_send_stream>(this,
                                                     boost::asio::buffer_cast<const char *>(m_oSendBuffer.readable()),
                                                     boost::asio::buffer_size(m_oSendBuffer.readable()));


                m_oSocket.async_write_some(boost::asio::buffer(m_oSendBuffer.readable()),
                                           boost::bind(&net_tcp_connection_base::handle_pkg_sent, this,//shared_from_this(),
                                                       boost::asio::placeholders::error,
                                                       boost::asio::placeholders::bytes_transferred));
            } else {
                start_send();
            }
        } else {
            m_iPointState = state_error;
            LOG(WARNING)<<"handle_pkg_sent, get error "<<ec.message();
            m_pEH->triger<tcp_send_stream_error>(this, ec);
        }

    }

    void net_tcp_connection_base::start_recv() {
        try {
            m_pEH->triger<tcp_start_recv_stream>(m_oSocket.local_endpoint(),
                                                 m_oSocket.remote_endpoint());
            LOG(INFO) << "start_recv() on " << m_oRemoteEndpoint.address().to_string();
            m_oSocket.async_read_some(boost::asio::buffer(m_oRecvBuffer.writeable()),
                                  boost::bind(&net_tcp_connection_base::handle_received_pkg, this, //shared_from_this(),
                                              boost::asio::placeholders::error(),
                                              boost::asio::placeholders::bytes_transferred()));
        } catch (boost::system::system_error se) {
            LOG(WARNING) << "start_recv(), remote_endpoint is disconnected!";
        }

    }

    void net_tcp_connection_base::handle_received_pkg(const boost::system::error_code &error, size_t bytes_transferred) {
        if (!error) {
            m_pEH->triger<tcp_recv_stream_succ>(this, bytes_transferred);
            m_oRecvBuffer.filled() += bytes_transferred;
            LOG(INFO) << "recv pkg: " << bytes_transferred << " bytes, from " << m_oRemoteEndpoint.address().to_string();
            slice_and_dispatch_pkg();
            start_recv();
        } else {
            m_iPointState = state_error;
            LOG(WARNING) << "handle_received_pkg(), Get error " << error.message() << " from " <<
                                  m_oRemoteEndpoint.address().to_string();
            m_pEH->triger<tcp_recv_stream_error>(this, error);
        }
    }

    void net_tcp_connection_base::slice_and_dispatch_pkg() {
        std::list<shared_buffer> sbs = m_pPacker->split(m_oRecvBuffer);
        for (std::list<shared_buffer>::iterator it = sbs.begin();
             it != sbs.end(); ++it) {
            shared_buffer sb = *it;
            const char *pBuf = sb.buffer();
            uint32_t pkg_id;
            ffnet::deseralize(pBuf, pkg_id);
            bool got_pkg_handler = false;
            std::map<uint32_t, tcp_pkg_handler *>::iterator cit = m_oRPHCache.find(pkg_id);
            if(cit != m_oRPHCache.end())
            {
                got_pkg_handler = true;
                tcp_pkg_handler * ph = cit->second;
                ph->handle_pkg(this, sb);
            }else {
                for (size_t i = 0; i < m_pRPH.size(); ++i) {
                    if (m_pRPH[i]->is_pkg_to_handle(pkg_id)) {
                        m_oRPHCache.insert(std::make_pair(pkg_id, m_pRPH[i]));
                        m_pRPH[i]->handle_pkg(this, sb);
                        got_pkg_handler = true;
                    }
                }
            }

            if(!got_pkg_handler){
                LOG(WARNING)<<"slice_and_dispatch_pkg(), cannot find handler for pkg id: "<<pkg_id;
            }
        }
    }

    void net_tcp_connection_base::close() {
        m_oSocket.close();
        m_iPointState = state_closed;
    }

}//end namespace ffnet
