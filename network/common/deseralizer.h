#ifndef _NETWORK_COMMON_DESERALIZER_H_
#define _NETWORK_COMMON_DESERALIZER_H_
#include "common/type_traits.h"
#include <cassert>
#include <cstdlib>
#include <cstring>

namespace ffnet
{
class Deseralizer
	{
	public:
		Deseralizer(const char * buf, size_t len)
		: m_iBase(0)
		, m_pBuf(buf)
		, m_iBufLen(len){};
		
		template<class Ty_>
		void				archive(Ty_ & val){
			typename TypeCategory<Ty_>::CategoryType ct;
			deseralize(val, ct);
		};
		template <class Ty_, size_t N>
		void				archive(Ty_ (& val)[N])
		{
			typename TypeCategory<Ty_>::CategoryType ct;
			deseralize(val, ct);
		}
		
		template<class Ty_, class CTy_>
		void				archive(Ty_ * &val, CTy_ & count)
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
			archive(count);
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