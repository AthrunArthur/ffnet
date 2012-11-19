#include "archive/archive.h"

namespace ffnet
{
size_t seralize(const String &val, char *pBuf)
{
	int32_t len = static_cast<int32_t>(val.size());
	std::memcpy(pBuf, (const char *)&len, sizeof(len));
	std::memcpy(pBuf + sizeof(len), val.c_str(), len);
	return static_cast<size_t>(len) + sizeof(len);
}


size_t	deseralize(const char *pBuf, String &val)
{
	int32_t len;
	std::memcpy((char *)&len, pBuf, sizeof(int32_t));
	val = String(len, 0);
	std::memcpy(const_cast<char *>(val.data()), pBuf + sizeof(len),len);
	return static_cast<size_t>(len) + sizeof(len);
}

}
