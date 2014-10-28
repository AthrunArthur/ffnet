#include "common/archive.h"

namespace ffnet
{
namespace archive {
size_t seralize(const String &val, char *pBuf)
{
    int32_t len = static_cast<int32_t>(val.size());
    size_t offset = seralize(len, pBuf);
    std::memcpy(pBuf + offset, val.c_str(), len);
    return static_cast<size_t>(len) + offset;
}

size_t    deseralize(const char *pBuf, String &val)
{
    int32_t len;
    size_t offset = deseralize(pBuf, len);
    val = String(len, 0);
    std::memcpy(const_cast<char *>(val.data()), pBuf + offset,len);
    return static_cast<size_t>(len) + offset;
}


size_t length(const String &val)
{
    return length(val.size()) + val.size();
}

}//end namespace archive
}//end namespace ffnet
