#pragma once
#include "dt_recv2.h"
#include "Interfaces.h"
//#include "SDK.h"
#include "getvfunc.h"

//#include <cstdint>

#include <cstdint>
#include <cstddef>
#include <string.h>

//class ClientClass;

namespace FNV1A
{
	inline constexpr uint32_t uHash32 = 0x811C9DC5;
	inline constexpr uint32_t uPrime32 = 0x1000193;
	inline constexpr uint64_t uHash64 = 0xcbf29ce484222325;
	inline constexpr uint64_t uPrime64 = 0x100000001b3;

	// compile-time hashes
	constexpr uint32_t Hash32Const(const char* szString, const uint32_t uValue = uHash32) noexcept
	{
		return (szString[0] == '\0') ? uValue : Hash32Const(&szString[1], (uValue ^ uint32_t(szString[0])) * uPrime32);
	}
	constexpr uint64_t Hash64Const(const char* szString, const uint64_t uValue = uHash64) noexcept
	{
		return (szString[0] == '\0') ? uValue : Hash64Const(&szString[1], (uValue ^ uint64_t(szString[0])) * uPrime64);
	}

	// runtime hashes
	inline uint32_t Hash32(const char* szString)
	{
		uint32_t uHashed = uHash32;

		for (std::size_t i = 0U; i < strlen(szString); ++i)
		{
			uHashed ^= szString[i];
			uHashed *= uPrime32;
		}

		return uHashed;
	}
	inline uint64_t Hash64(const char* szString)
	{
		uint64_t uHashed = uHash64;

		for (std::size_t i = 0U; i < strlen(szString); ++i)
		{
			uHashed ^= szString[i];
			uHashed *= uPrime64;
		}

		return uHashed;
	}
}

class CNetVars
{
public:
  int GetOffset(RecvTable* pTable, const char* szNetVar);
  int GetNetVar(const char* szClass, const char* szNetVar);
};

inline CNetVars* gNetVars = new CNetVars(); // not how DarkStorm base would originally do it, but it should work fine

#define NETVAR(_name, type, table, name) inline type& _name() \
{ \
	static int nOffset = gNetVars->GetNetVar(table, name); \
	return *reinterpret_cast<type*>(uintptr_t(this) + nOffset); \
}

#define NETVAR_OFF(_name, type, table, name, offset) inline type& _name() \
{ \
	static int nOffset = gNetVars->GetNetVar(table, name) + offset; \
	return *reinterpret_cast<type*>(uintptr_t(this) + nOffset); \
}

/*#define OFFSET(name, type, offset) inline type& name() \
{ \
	return *reinterpret_cast<type*>(uintptr_t(this) + offset); \
}

#define VIRTUAL(name, type, fn, base, index) inline type name() \
{ \
	return reinterpret_cast<fn>(find_vfunc(base, index))(base); \
}*/

/*#define CONDGET(name, conditions, cond) inline bool name() \
{ \
	return (conditions & cond); \
}*/
