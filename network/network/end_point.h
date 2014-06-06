#ifndef _NETWORK_NETWORK_END_POINT_H_
#define _NETWORK_NETWORK_END_POINT_H_
#include "common.h"

namespace ffnet
{
enum ProtocolType {
    tcp_v4 = 1,
    tcp_v6 = 2,
    udp_v4 = 3,
    udp_v6 = 4
};

class Endpoint
{
public:

    typedef boost::asio::detail::socket_addr_type data_type;

    Endpoint(boost::asio::ip::tcp::endpoint ep)
    : m_oImpl(ep.address(), ep.port())
    {
        if(ep.address().is_v4())
            m_iProtocol = tcp_v4;
	else
            m_iProtocol = tcp_v6;
    }
    Endpoint(boost::asio::ip::udp::endpoint ep)
    : m_oImpl(ep.address(), ep.port()){
        if(ep.address().is_v4())
            m_iProtocol = udp_v4;
        else
            m_iProtocol = udp_v6;
    }
    bool			generateTypedEndpoint(boost::asio::ip::tcp::endpoint & ep)
    {
        if(is_udp())
            return false;
	
        ep = boost::asio::ip::tcp::endpoint(address(), port());
        return true;
    }
    bool			generateTypedEndpoint(boost::asio::ip::udp::endpoint & ep)
    {
        if(is_tcp())
            return false;
        ep = boost::asio::ip::udp::endpoint(address(), port());
            return true;
    }
	

    /// Default constructor.
    Endpoint()
        : m_oImpl() {
    }

    /// Construct an endpoint using a port number, specified in the host's byte
    /// order. The IP address will be the any address (i.e. INADDR_ANY or
    /// in6addr_any). This constructor would typically be used for accepting new
    /// connections.
    /**
     * @par Examples
     * To initialise an IPv4 TCP endpoint for port 1234, use:
     * @code
     * boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), 1234);
     * @endcode
     *
     * To specify an IPv6 UDP endpoint for port 9876, use:
     * @code
     * boost::asio::ip::udp::endpoint ep(boost::asio::ip::udp::v6(), 9876);
     * @endcode
     */
    Endpoint(ProtocolType protocol, unsigned short port_num)
    : m_iProtocol(protocol) {
        switch(protocol)
        {
        case tcp_v4:
            m_oImpl = boost::asio::ip::detail::endpoint(boost::asio::ip::tcp::v4().family(), port_num);
            break;
        case tcp_v6:
            m_oImpl = boost::asio::ip::detail::endpoint(boost::asio::ip::tcp::v6().family(), port_num);
            break;
        case udp_v4:
            m_oImpl = boost::asio::ip::detail::endpoint(boost::asio::ip::udp::v4().family(), port_num);
            break;
        case udp_v6:
            m_oImpl = boost::asio::ip::detail::endpoint(boost::asio::ip::udp::v6().family(), port_num);
            break;
        }
    }

    Endpoint(ProtocolType protocol, const boost::asio::ip::address &addr, unsigned short port_num)
		: m_iProtocol(protocol)
		,  m_oImpl(addr, port_num) {
    }

    /// Copy constructor.
    Endpoint(const Endpoint &other)
        : m_iProtocol(other.m_iProtocol)
        , m_oImpl(other.m_oImpl) {
    }

    /// Assign from another endpoint.
    Endpoint &operator=(const Endpoint &other) {
        m_oImpl = other.m_oImpl;
		m_iProtocol = other.m_iProtocol;
        return *this;
    }

    bool			is_tcp() const
    {
        return m_iProtocol == tcp_v4 || m_iProtocol == tcp_v6;
    }
    bool			is_udp() const
    {
        return m_iProtocol == udp_v4 || m_iProtocol == udp_v6;
    }
    bool			is_v4() const
    {
        return m_iProtocol == tcp_v4 || m_iProtocol == udp_v4;
    }
    bool			is_v6() const
    {
        return m_iProtocol == tcp_v6 || m_iProtocol == udp_v6;
    }
	
    /// The protocol associated with the endpoint.
    ProtocolType protocol() const {
        return m_iProtocol;
    }

    /// Get the underlying endpoint in the native type.
    data_type *data() {
        return m_oImpl.data();
    }

    /// Get the underlying endpoint in the native type.
    const data_type *data() const {
        return m_oImpl.data();
    }

    /// Get the underlying size of the endpoint in the native type.
    std::size_t size() const {
        return m_oImpl.size();
    }

    /// Set the underlying size of the endpoint in the native type.
    void resize(std::size_t size) {
        m_oImpl.resize(size);
    }

    /// Get the capacity of the endpoint in the native type.
    std::size_t capacity() const {
        return m_oImpl.capacity();
    }

    /// Get the port associated with the endpoint. The port number is always in
    /// the host's byte order.
    unsigned short port() const {
        return m_oImpl.port();
    }

    /// Set the port associated with the endpoint. The port number is always in
    /// the host's byte order.
    void port(unsigned short port_num) {
        m_oImpl.port(port_num);
    }

    /// Get the IP address associated with the endpoint.
    boost::asio::ip::address address() const {
        return m_oImpl.address();
    }

    /// Set the IP address associated with the endpoint.
    void address(const boost::asio::ip::address &addr) {
        m_oImpl.address(addr);
    }

    /// Compare two endpoints for equality.
    friend bool operator==(const Endpoint& e1,
                           const Endpoint& e2) {
        return e1.m_oImpl == e2.m_oImpl && e1.m_iProtocol == e2.m_iProtocol;
    }

    /// Compare two endpoints for inequality.
    friend bool operator!=(const Endpoint& e1,
                           const Endpoint& e2) {
        return !(e1 == e2);
    }
/*
    /// Compare endpoints for ordering.
    friend bool operator<(const Endpoint& e1,
                          const Endpoint& e2) {
        return e1.m_iProtocol< e2.m_iProtocol && e1.m_oImpl < e2.m_oImpl;
    }

    /// Compare endpoints for ordering.
    friend bool operator>(const Endpoint& e1,
                          const Endpoint& e2) {
        return e1.m_iProtocol> e2.m_iProtocol && e1.m_oImpl > e2.m_oImpl;
    }

    /// Compare endpoints for ordering.
    friend bool operator<=(const Endpoint& e1,
                           const Endpoint& e2) {
        return !(e2 < e1);
    }

    /// Compare endpoints for ordering.
    friend bool operator>=(const Endpoint& e1,
                           const Endpoint& e2) {
        return !(e1 < e2);
    }
*/
private:
    ProtocolType 		m_iProtocol;
    // The underlying IP endpoint.
    boost::asio::ip::detail::endpoint m_oImpl;

};//end class Endpoint

typedef boost::shared_ptr<Endpoint> EndpointPtr_t;
}
#endif
