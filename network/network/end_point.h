#ifndef _NETWORK_NETWORK_END_POINT_H_
#define _NETWORK_NETWORK_END_POINT_H_
#include "common.h"
#include <boost/concept_check.hpp>

namespace ffnet
{
enum ProtocolType {
    proto_invalid = 0,
    proto_tcp = 1,
    proto_udp = 3,
};

//! This is a key class to represent an endpoint in ffnet.
//! It contains 3 things, protocol (tcp or udp), ip address, and port.
class Endpoint
{
public:
    Endpoint(const boost::asio::ip::address & addr, uint16_t port, ProtocolType proto)
    : m_iProtocol(proto)
    , m_oIPAddr(addr)
    , m_iPort(port){}
    
    Endpoint(const boost::asio::ip::tcp::endpoint & ep)
    : m_iProtocol(proto_tcp)
    , m_oIPAddr(ep.address())
    , m_iPort(ep.port())
    {}
    
    Endpoint(const boost::asio::ip::udp::endpoint & ep)
    : m_iProtocol(proto_udp)
    , m_oIPAddr(ep.address())
    , m_iPort(ep.port()){}
    
    bool            generateTypedEndpoint(boost::asio::ip::tcp::endpoint & ep)
    {
        if(is_udp())
            return false;
    
        ep = boost::asio::ip::tcp::endpoint(address(), port());
        return true;
    }
    bool            generateTypedEndpoint(boost::asio::ip::udp::endpoint & ep)
    {
        if(is_tcp())
            return false;
        ep = boost::asio::ip::udp::endpoint(address(), port());
            return true;
    }
    

    /// Default constructor.
    Endpoint()
        : m_iProtocol(proto_invalid)
        , m_oIPAddr()
        , m_iPort(0){
    }

    

    /// Copy constructor.
    Endpoint(const Endpoint &other)
        : m_iProtocol(other.m_iProtocol)
        , m_oIPAddr(other.m_oIPAddr)
        , m_iPort(other.m_iPort){
    }

    /// Assign from another endpoint.
    Endpoint &operator=(const Endpoint &other) {
        if (&other == this)
            return *this;
        m_iProtocol = other.m_iProtocol;
        m_oIPAddr = other.m_oIPAddr;
        m_iPort = other.m_iPort;
        return *this;
    }

    bool            is_tcp() const
    {
        return m_iProtocol == proto_tcp;
    }
    bool            is_udp() const
    {
        return m_iProtocol == proto_udp;
    }
    bool            is_v4() const
    {
        return m_oIPAddr.is_v4();
    }
    bool            is_v6() const
    {
        return m_oIPAddr.is_v6();
    }
    
    /// The protocol associated with the endpoint.
    ProtocolType protocol() const {
        return m_iProtocol;
    }
    void        protocol(ProtocolType pt){
        m_iProtocol = pt;
    }

    /// Get the port associated with the endpoint. The port number is always in
    /// the host's byte order.
    unsigned short port() const {
        return m_iPort;
    }

    /// Set the port associated with the endpoint. The port number is always in
    /// the host's byte order.
    void port(unsigned short port_num) {
        m_iPort = port_num;
    }

    /// Get the IP address associated with the endpoint.
    boost::asio::ip::address address() const {
        return m_oIPAddr;
    }

    /// Set the IP address associated with the endpoint.
    void address(const boost::asio::ip::address &addr) {
        m_oIPAddr = addr;
    }

    /// Compare two endpoints for equality.
    bool operator==(const Endpoint& e) {
        return e.m_iPort == m_iPort && e.m_oIPAddr == m_oIPAddr;
    }

    /// Compare two endpoints for inequality.
    bool operator!=(const Endpoint& e) {
        return !(operator==(e));
    }

    /// Compare endpoints for ordering.
    friend bool operator<(const Endpoint& e1,
                          const Endpoint& e2) {
        return e1.m_iProtocol< e2.m_iProtocol && e1.m_oIPAddr < e2.m_oIPAddr;
    }

    /// Compare endpoints for ordering.
    friend bool operator>(const Endpoint& e1,
                          const Endpoint& e2) {
        return e1.m_iProtocol> e2.m_iProtocol && e1.m_oIPAddr > e2.m_oIPAddr;
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
    
    std::string to_str() const{
        std::stringstream ss;
        ss<<m_oIPAddr<<":"<<m_iPort;
        return ss.str();
    }

private:
    ProtocolType         m_iProtocol;
    // The underlying IP endpoint.
    boost::asio::ip::address m_oIPAddr;
    uint16_t    m_iPort;
};//end class Endpoint

typedef boost::shared_ptr<Endpoint> EndpointPtr_t;
}
#endif
