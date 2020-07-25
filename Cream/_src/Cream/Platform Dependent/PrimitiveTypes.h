#pragma once
#include <memory>

// Unsigned Integers
typedef unsigned char      U8;
typedef unsigned short     U16;
typedef unsigned int       U32;
typedef unsigned long long U64;

// Integers
typedef signed char        I8;
typedef short              I16;
typedef int                I32;
typedef long long          I64;

// Fast Unsigned Integers
typedef unsigned char      U8F;
typedef unsigned int       U16F;
typedef unsigned int       U32F;
typedef unsigned long long U64F;

// Fast Integers
typedef signed char        I8F;
typedef int                I16F;
typedef int                I32F;
typedef long long          I64F;

// Floats
typedef float F32;

namespace Cream
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename...Args>
	constexpr Scope<T> CreateScope(Args&&...args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename...Args>
	constexpr Ref<T> CreateRef(Args&&...args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}