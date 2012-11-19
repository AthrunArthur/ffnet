#ifndef _NETWROK_COMMON_SERIALIZER_H_
#define _NETWROK_COMMON_SERIALIZER_H_
#include "common.h"
#include "common/type_traits.h"
#include "archive/archive.h"
#include <cstdlib>
#include <cstring>
namespace ffnet
{
	
	
	class Seralizer : public Archive
	{
	public:
		Seralizer(char * buf, size_t len)
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
		void				archive_impl(const Ty_ & val){
			typename TypeCategory<Ty_>::CategoryType ct;
			seralize(val, ct);
		};
		
		template <class Ty_, size_t N>
		void				archive_impl(Ty_ (& val)[N])
		{
			typename TypeCategory<Ty_>::CategoryType ct;
			seralize(val, ct);
		}
		
		template<class Ty_, class CTy_>
		void				archive_impl(const Ty_ *val, CTy_ & count)
		{
			typename TypeCategory<Ty_>::CategoryType ct;
			seralize(val, count, ct);
		}
		
		
	protected:
		template<class Ty_, class TyCat_>
		void				seralize(const Ty_ &val, TyCat_)
		{
			assert(0 && "this type is not supported by now!");
			exit(0);
		}
		template<class Ty_>
		void				seralize(const Ty_ &val, BasicType)
		{
			assert( m_iBase +sizeof(Ty_) <= m_iBufLen && "buffer overflow");
			std::memcpy(m_pBuf + m_iBase, (const char *)&val, sizeof(val));
			m_iBase += sizeof(val);
		}
		
		void 				seralize(const std::string &val, StringType)
		{
			int32_t len = static_cast<int32_t>(val.size());
			assert( m_iBase + len + sizeof(len) <= m_iBufLen && "buffer overflow");
			std::memcpy(m_pBuf + m_iBase, (const char *)&len, sizeof(len));
			m_iBase += sizeof(len);
			std::memcpy(m_pBuf + m_iBase, val.c_str(), len);
			m_iBase += len;
		}
		template<class Ty_>
		void				seralize(const Ty_ *val, BasicType)
		{
			assert( m_iBase +sizeof(Ty_) <= m_iBufLen && "buffer overflow");
			std::memcpy(m_pBuf + m_iBase, (const char *)val, sizeof(Ty_));
			m_iBase += sizeof(Ty_);
		}

		template<class Ty_, size_t N>
		void				seralize(const Ty_ (&val)[N], BasicType)
		{
			assert( m_iBase +sizeof(Ty_) *N <= m_iBufLen && "buffer overflow");
			std::memcpy(m_pBuf + m_iBase, (const char *)val, sizeof(Ty_) * N);
			m_iBase += sizeof(Ty_) * N;
		}
		
		
		
		//////////////////////////////////
		template<class Ty_,class CTy_,  class TyCat_>
		void				seralize(const Ty_ *val, CTy_ & count, TyCat_)
		{
			assert(0 && "this type is not supported by now!");
			exit(0);
		}
		
		template<class Ty_,class CTy_>
		void				seralize(const Ty_ *val, CTy_ & count, BasicType)
		{
			archive_impl(count);
			assert( m_iBase +sizeof(Ty_)*count <= m_iBufLen && "buffer overflow");
			std::memcpy(m_pBuf + m_iBase, (const char *) val, sizeof(Ty_) * count);
			m_iBase += sizeof(Ty_) * count;
		}
		
	protected:
		size_t 			m_iBase;
		char *			m_pBuf;
		size_t			m_iBufLen;
	};


}
#endif

