#pragma once

#include "SDK.h"

class CSignature
{
public:
	uintptr_t dwFindPattern(const char* module_name, const char* signature);
	HMODULE GetModuleHandleSafe( const char* pszModuleName );
	DWORD GetClientSignature ( const char* chPattern );
	DWORD GetEngineSignature ( const char* chPattern );

  /*template <typename T>
	inline T GetVFunc( void* ptr, const unsigned short index )
	{
		return ( *static_cast< T** >( ptr ) )[ index ];
	}*/
};

extern CSignature gSignatures;
