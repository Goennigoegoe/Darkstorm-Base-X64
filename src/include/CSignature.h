#pragma once

#include "SDK.h"

class CSignature
{
public:
	DWORD dwFindPattern(DWORD dwAddress, DWORD dwLength, const char* szPattern);
	HMODULE GetModuleHandleSafe( const char* pszModuleName );
	DWORD GetClientSignature ( char* chPattern );
	DWORD GetEngineSignature ( char* chPattern );

  /*template <typename T>
	inline T GetVFunc( void* ptr, const unsigned short index )
	{
		return ( *static_cast< T** >( ptr ) )[ index ];
	}*/
};

extern CSignature gSignatures;
