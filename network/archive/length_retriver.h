#ifndef _NETWORK_COMMON_LENGTH_RETRIVER_H_
#define _NETWORK_COMMON_LENGTH_RETRIVER_H_
#include "common/type_traits.h"
#include "archive/archive.h"
#include <cassert>
#include <cstdlib>
#include <cstring>


namespace ffnet
{
class LengthRetriver : public Archive
	{
	public:
		LengthRetriver()
		: m_iBase(0){};
		
		uint32_t			getLength() const{return m_iBase;}
		
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
			length(val, ct);
		};
		
		template <class Ty_, size_t N>
		void				archive_impl(const Ty_ (& val)[N])
		{
			typename TypeCategory<Ty_>::CategoryType ct;
			length(val, ct);
		}
		
		template<class Ty_, class CTy_>
		void				archive_impl(const Ty_ *val, const CTy_ & count)
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
		void 				length(std::string &val, StringType)
		{
			int32_t len = sizeof(int32_t) + static_cast<int32_t>(val.size());
			
			m_iBase += len;
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
			archive_impl(count);
			m_iBase += sizeof(Ty_) * count;
		}
		
	protected:
		uint32_t 			m_iBase;
	};
}
#endif