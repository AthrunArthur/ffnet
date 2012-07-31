#ifndef _NETWORK_COMMON_LENGTH_RETRIVER_H_
#define _NETWORK_COMMON_LENGTH_RETRIVER_H_
#include "common/type_traits.h"
#include <cassert>
#include <cstdlib>
#include <cstring>

namespace ffnet
{
class LengthRetriver
	{
	public:
		LengthRetriver()
		: m_iBase(0){};
		
		uint32_t			getLength() const{return m_iBase;}
		template<class Ty_>
		void				archive(const Ty_ & val){
			typename TypeCategory<Ty_>::CategoryType ct;
			length(val, ct);
		};
		
		template <class Ty_, size_t N>
		void				archive(const Ty_ (& val)[N])
		{
			typename TypeCategory<Ty_>::CategoryType ct;
			length(val, ct);
		}
		
		template<class Ty_, class CTy_>
		void				archive(const Ty_ *val, const CTy_ & count)
		{
			typename TypeCategory<Ty_>::CategoryType ct;
			length(val, count, ct);
		}
		
		
	protected:
		template<class Ty_, class TyCat_>
		void				length(const Ty_ &val, TyCat_)
		{
			assert(0 && "this type is not supported by now!");
			exit(0);
		}
		template<class Ty_>
		void				length(const Ty_ &val, BasicType)
		{
			m_iBase += sizeof(val);
		}
		template<class Ty_>
		void				length(const Ty_ *val, BasicType)
		{
			m_iBase += sizeof(Ty_);
		}

		template<class Ty_, size_t N>
		void				length(const Ty_ (&val)[N], BasicType)
		{
			m_iBase += sizeof(Ty_) * N;
		}
		
		//////////////////////////////////
		template<class Ty_,class CTy_,  class TyCat_>
		void				length(const Ty_ *val, const CTy_ & count, TyCat_)
		{
			assert(0 && "this type is not supported by now!");
			exit(0);
		}
		
		template<class Ty_,class CTy_>
		void				length(const Ty_ *val, const CTy_ & count, BasicType)
		{
			archive(count);
			m_iBase += sizeof(Ty_) * count;
		}
		
	protected:
		uint32_t 			m_iBase;
	};
}
#endif