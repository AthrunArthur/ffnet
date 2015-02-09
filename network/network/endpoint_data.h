#ifndef _NETWORK_NETWORK_ENDPOINT_DATA_H_
#define _NETWORK_NETWORK_ENDPOINT_DATA_H_
#include "common.h"
#include "common/shared_buffer.h"
#include "network/end_point.h"


namespace ffnet
{
class EndPointWithData
{
public:
    EndPointWithData(EndpointPtr_t ep, SharedBuffer &buffer);


    inline EndpointPtr_t        &Endpoint() {
        return m_pEndpoint;
    }
    inline SharedBuffer     &buffer() {
        return m_oBuffer;
    }
protected:
    EndpointPtr_t            m_pEndpoint;
    SharedBuffer             m_oBuffer;
};//end class EndPointWithData

String    toString(Endpoint  ep);

String    printBuf(const char *pBuf, size_t len);

typedef boost::shared_ptr<EndPointWithData>    EndPointBufferPtr_t;

}//end namespace ffnet
#endif
