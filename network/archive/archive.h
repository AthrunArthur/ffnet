#ifndef _NETWORK_ARCHIVE_ARCHIVE_H_
#define _NETWORK_ARCHIVE_ARCHIVE_H_

#include "common.h"


namespace ffnet
{
	class Archive
	{
	public:
		virtual void			archive(int8_t & val) = 0;
		virtual void			archive(int16_t & val) = 0;
		virtual void			archive(int32_t & val) = 0;
		virtual void			archive(int64_t & val) = 0;
		virtual void			archive(uint8_t & val) = 0;
		virtual void			archive(uint16_t & val) = 0;
		virtual void			archive(uint32_t & val) = 0;
		virtual void			archive(uint64_t & val) = 0;
		virtual void			archive(String & val) = 0;
		virtual void			archive(int8_t * val, int& len) = 0;
		virtual void			archive(uint8_t * val, int& len) = 0;
	};//end  class Archive
	
	
template<class Ty_>
size_t	seralize(const Ty_ & val, char *pBuf)
{
    std::memcpy(pBuf, (const char *)&val, sizeof(Ty_));
    return sizeof(Ty_);
}

size_t seralize(const String &val, char *pBuf);

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
size_t	deseralize(const char *pBuf, String &val);

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

