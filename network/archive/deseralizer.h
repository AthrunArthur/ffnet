#ifndef _NETWORK_COMMON_DESERALIZER_H_
#define _NETWORK_COMMON_DESERALIZER_H_
#include "common/type_traits.h"
#include "archive/archive.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <string>
namespace ffnet
{
class Deseralizer : public Archive
	{
	public:
		Deseralizer(const char * buf, size_t len)
		: m_iBase(0)
		, m_pBuf(buf)
		, m_iBufLen(len){};
		
		virtual void			archive(int8_t & val){archive_impl(val);}
		virtual void			archive(int16_t & val){archive_impl(val);}
		virtual void			archive(int32_t & val){archive_impl(val);}
		virtual void			archive(int64_t & val){archive_impl(val);}
		virtual void			archive(uint8_t & val) {archive_impl(val);}
		virtual void			archive(uint16_t & val){archive_impl(val);}
		virtual void			archive(uint32_t & val){archive_impl(val);}
		virtual void			archive(uint64_t & val){archive_impl(val);}
		virtual void			archive(String & val) {archive_impl(val);}
		virtual void			archive(int8_t * val, int& len) {archive_impl(val, len);}
		virtual void			archive(uint8_t * val, int& len) {archive_impl(val, len);}
	protected:
		template<class Ty_>
		void				archive_impl(Ty_ & val){
			typename TypeCategory<Ty_>::CategoryType ct;
			deseralize(val, ct);
		};
		template <class Ty_, size_t N>
		void				archive_impl(Ty_ (& val)[N])
		{
			typename TypeCategory<Ty_>::CategoryType ct;
			deseralize(val, ct);
		}
		
		template<class Ty_, class CTy_>
		void				archive_impl(Ty_ * &val, CTy_ & count)
		{
			typename TypeCategory<Ty_>::CategoryType ct;
			deseralize(val, count, ct);
		}
		
		
	protected:
		template<class Ty_, class TyCat_>
		void				deseralize(Ty_ &val, TyCat_)
		{
			assert(0 && "this type is not supported by now!");
			exit(0);
		}
		template<class Ty_>
		void				deseralize(Ty_ &val, BasicType _)
		{
			assert( m_iBase +sizeof(Ty_) <= m_iBufLen && "buffer overflow");
			std::memcpy((char *)&val, m_pBuf + m_iBase, sizeof(val));
			m_iBase += sizeof(val);
		}
		void 				deseralize(std::string &val, StringType)
		{
			int32_t len;
			BasicType _;
			deseralize(len, _);
			
			assert( m_iBase + len <= m_iBufLen && "buffer overflow");
			val = std::string(len, 0);
			std::memcpy(const_cast<char *>(val.data()), m_pBuf + m_iBase,  len);
			m_iBase += len;
		}
		template<class Ty_>
		void				deseralize(Ty_ * &val, BasicType _)
		{
			assert( m_iBase +sizeof(Ty_) <= m_iBufLen && "buffer overflow");
			std::memcpy((const char *)val, m_pBuf + m_iBase, sizeof(Ty_));
			m_iBase += sizeof(Ty_);
		}

		template<class Ty_, size_t N>
		void				deseralize(Ty_ (&val)[N], BasicType _)
		{
			assert( m_iBase +sizeof(Ty_) *N <= m_iBufLen && "buffer overflow");
			std::memcpy((char *)val, m_pBuf + m_iBase, sizeof(Ty_) * N);
			m_iBase += sizeof(Ty_) * N;
		}
		
		//////////////////////////////////
		template<class Ty_,class CTy_,  class TyCat_>
		void				deseralize(Ty_ * &val, CTy_ & count, TyCat_)
		{
			assert(0 && "this type is not supported by now!");
			exit(0);
		}
		
		template<class Ty_,class CTy_>
		void				deseralize(Ty_ * &val, CTy_ & count, BasicType)
		{
			archive_impl(count);
			assert( m_iBase +sizeof(Ty_)*count <= m_iBufLen && "buffer overflow");
			val = (Ty_ *) (m_pBuf + m_iBase);
			//val = new char[count];
			//std::memcpy((char *) val,m_pBuf + m_iBase, sizeof(Ty_) * count);
			m_iBase += sizeof(Ty_) * count;
		}
		
	protected:
		size_t 			m_iBase;
		const char *		m_pBuf;
		size_t			m_iBufLen;
	};
}
#endif