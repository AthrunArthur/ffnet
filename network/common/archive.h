#ifndef _NETWORK_COMMON_ARCHIVE_H_
#define _NETWORK_COMMON_ARCHIVE_H_

#include "common/serializer.h"
#include "common/deseralizer.h"
#include "common/length_retriver.h"

namespace ffnet
{
		
template<class Ty_>
size_t	seralize(const Ty_ & val, char *pBuf)
{
    std::memcpy(pBuf, (const char *)&val, sizeof(Ty_));
    return sizeof(Ty_);
}

template<class Ty_, size_t N>
size_t	seralize(const Ty_ (& val)[N], char *pBuf)
{
	std::memcpy(pBuf, (const char *)val, sizeof(Ty_)*N);
	return sizeof(Ty_)*N;
}

template<class Ty_>
size_t seralize(const Ty_ * val, size_t cout, char *pBuf)
{
	std::memcpy(pBuf, (const char *)val, sizeof(Ty_) * cout);
	return sizeof(Ty_) * cout;
}

template <class Ty_>
size_t	deseralize(const char *pBuf, Ty_ & val)
{
    std::memcpy((char *)&val, pBuf, sizeof(Ty_));
    return sizeof(Ty_);
}

template <class Ty_, size_t N>
size_t deseralize(const char *pBuf, Ty_ (& val)[N])
{
	std::memcpy((char *)val, pBuf, sizeof(Ty_) *N);
	return sizeof(Ty_) * N;
}

template<class Ty_>
size_t deseralize(const char *pBuf, Ty_ *val, size_t count)
{
	std::memcpy((char *)val, pBuf, sizeof(Ty_) * count);
	return count * sizeof(Ty_);
}

}
#endif

