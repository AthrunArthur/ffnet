#include "network/endpoint_data.h"
#include <cstdio>
namespace ffnet
{
EndPointWithData::EndPointWithData(EndpointPtr_t ep, SharedBuffer &buffer)
: m_pEndpoint(ep)
, m_oBuffer(buffer)
{

}

#ifdef ENABLE_LOG_CONSOLE
static char int2C(int t)
{
	if(t >=0 && t<= 9)
		return '0' + t;
	else if(t>=10 && t<=15)
		return 'A' + t - 10;
	return '*';
}

String toString(Endpoint tep)
{
	return tep.address().to_string();
}

String printBuf(const char *pBuf, size_t len)
{
	String res;
	for(size_t i = 0; i < len; ++i)
	{
		char t[4];
		std::sprintf(t, "%c%c ", int2C(pBuf[i]/16), int2C(pBuf[i]%16));
		String str(t);
		res += str;
	}
	return res;
}
#endif


}