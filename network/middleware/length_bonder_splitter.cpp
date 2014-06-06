#include "middleware/length_bonder_splitter.h"
#include "common/archive.h"
#include "package/package.h"
#include "common/defines.h"

using namespace ffnet::archive;
namespace ffnet
{
void LengthBonderSplitter::bond(NetBuffer &oSendBuffer, const char *pBuf, size_t len)
{
	uint32_t buflen = static_cast<uint32_t>(len);
	oSendBuffer.writeBuffer((char *)&buflen, sizeof(buflen));
	oSendBuffer.writeBuffer(pBuf, buflen);
}

void LengthBonderSplitter::bond(NetBuffer &oSendBuffer, PackagePtr_t pkg)
{
	Archive lr(Archive::length_retriver);
	
	pkg->arch(lr);
	uint32_t len = lr.getLength();
	
	oSendBuffer.reserveIdle(static_cast<size_t>(len + sizeof(len)));
	char *pBuf = boost::asio::buffer_cast<char *>(oSendBuffer.writeable());
	seralize(len, pBuf);
	oSendBuffer.filled() += sizeof(len);
	pBuf = boost::asio::buffer_cast<char *>(oSendBuffer.writeable());
	Archive s(pBuf, oSendBuffer.idle(), Archive::seralizer);
	pkg->arch(s);
	oSendBuffer.filled() += len;
	LOG_TRACE(connection)<<"LengthBonderSplitter::bond(), seralize pkg: ";
	     // <<printBuf(boost::asio::buffer_cast<const char *>(oSendBuffer.readable()),
//					 oSendBuffer.filled());
}

std::list<SharedBuffer> LengthBonderSplitter::split(NetBuffer &oRecvBuffer)
{
	std::list<SharedBuffer> resPkgs;
	if(oRecvBuffer.filled() <=4)
		return resPkgs;

	uint32_t len;
	const char * pBuf = boost::asio::buffer_cast<const char *>(oRecvBuffer.readable());
	size_t bi = 0;
	deseralize(pBuf + bi, len);
	
	while(oRecvBuffer.filled() - bi >= sizeof(len) &&
		oRecvBuffer.filled() -bi - sizeof(len) >=len)
	{
		LOG_DEBUG(connection) <<"LengthBonderSplitter::split() "<<"split pkg with len:"<<len;
		
		bi += sizeof(len);
		SharedBuffer sb(boost::shared_array<char>(new char[len]), len);
		std::memcpy(sb.buffer().get(), pBuf + bi, len);
		resPkgs.push_back(sb);
		bi += len;
		
		deseralize(pBuf + bi, len);
	}
	oRecvBuffer.eraseBuffer(bi);
	return resPkgs;
}

}//end namespace ffnet
