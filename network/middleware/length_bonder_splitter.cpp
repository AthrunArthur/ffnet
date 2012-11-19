#include "middleware/length_bonder_splitter.h"
#include "archive/archive.h"
#include "archive/deseralizer.h"
#include "archive/length_retriver.h"
#include "archive/serializer.h"
#include "package/package.h"
#include "log.h"

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
	LengthRetriver lr;
	
	pkg->arch(lr);
	uint32_t len = lr.getLength();
	
	oSendBuffer.reserveIdle(static_cast<size_t>(len + sizeof(len)));
	char *pBuf = boost::asio::buffer_cast<char *>(oSendBuffer.writeable());
	ffnet::seralize(len, pBuf);
	oSendBuffer.filled() += sizeof(len);
	
	pBuf = boost::asio::buffer_cast<char *>(oSendBuffer.writeable());
	Seralizer s(pBuf, oSendBuffer.idle());
	pkg->arch(s);
	oSendBuffer.filled() += len;
}

std::list<SharedBuffer> LengthBonderSplitter::split(NetBuffer &oRecvBuffer)
{
	std::list<SharedBuffer> resPkgs;
	if(oRecvBuffer.filled() <=4)
		return resPkgs;

	uint32_t len;
	const char * pBuf = boost::asio::buffer_cast<const char *>(oRecvBuffer.readable());
	size_t bi = 0;
	ffnet::deseralize(pBuf + bi, len);
	
	while(oRecvBuffer.filled() - bi >= sizeof(len) &&
		oRecvBuffer.filled() -bi - sizeof(len) >=len)
	{
		FFNET_DEBUG(
			log_connection("LengthBonderSplitter", "split(), split pkg with len: %d", len);
		)
		bi += sizeof(len);
		SharedBuffer sb(boost::shared_array<char>(new char[len]), len);
		std::memcpy(sb.buffer().get(), pBuf + bi, len);
		resPkgs.push_back(sb);
		bi += len;
		
		ffnet::deseralize(pBuf + bi, len);
	}
	oRecvBuffer.eraseBuffer(bi);
	return resPkgs;
}

}//end namespace ffnet