#ifndef _NETWROK_COMMON_SERIALIZER_H_
#define _NETWROK_COMMON_SERIALIZER_H_
#include "common.h"
#include "common/type_traits.h"
#include <cstdlib>
#include <cstring>
namespace ffnet
{
	
	
	class Seralizer
	{
	public:
		Seralizer(char * buf, size_t len)
		: m_iBase(0)
		, m_pBuf(buf)
		, m_iBufLen(len){};
		
		template<class Ty_>
		void				archive(const Ty_ & val){
			typename TypeCategory<Ty_>::CategoryType ct;
			seralize(val, ct);
		};
		
		template <class Ty_, size_t N>
		void				archive(Ty_ (& val)[N])
		{
			typename TypeCategory<Ty_>::CategoryType ct;
			seralize(val, ct);
		}
		
		template<class Ty_, class CTy_>
		void				archive(const Ty_ *val, CTy_ & count)
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
			archive(count);
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

