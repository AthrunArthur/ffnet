#ifndef _NETWORK_COMMON_TYPE_TRAITS_H_
#define _NETWORK_COMMON_TYPE_TRAITS_H_
#include "common.h"
#include <boost/cstdint.hpp>


namespace ffnet
{
	struct BasicType{};
	struct STLContainerType{};
	struct UnknownType{};
	
	template <class Ty_>
	class TypeCategory
	{
	public:
		typedef UnknownType	CategoryType;
	};
	template<>
	class TypeCategory<int64_t>
	{
	public:
		typedef BasicType	CategoryType;
	};
	template<>
	class TypeCategory<uint64_t>
	{
	public:
		typedef BasicType	CategoryType;
	};
	template<>
	class TypeCategory<int32_t>
	{
	public:
		typedef BasicType	CategoryType;
	};
	template<>
	class TypeCategory<uint32_t>
	{
	public:
		typedef BasicType	CategoryType;
	};
	template<>
	class TypeCategory<int16_t>
	{
	public:
		typedef BasicType	CategoryType;
	};
	template<>
	class TypeCategory<uint16_t>
	{
	public:
		typedef BasicType	CategoryType;
	};
	template<>
	class TypeCategory<int8_t>
	{
	public:
		typedef BasicType	CategoryType;
	};
	template<>
	class TypeCategory<uint8_t>
	{
	public:
		typedef BasicType	CategoryType;
	};
	
	template<>
	class TypeCategory<char>
	{
	public:
		typedef BasicType CategoryType;
	};
	
	template<>
	class TypeCategory<bool>
	{
	public:
		typedef BasicType CategoryType;
	};
	
	
}
#endif