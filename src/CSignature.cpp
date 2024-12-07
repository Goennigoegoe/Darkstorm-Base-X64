#include "include/sdk.h"
#include <time.h>

#include <psapi.h>
#include <vector>

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

uintptr_t CSignature::dwFindPattern(const char* module_name, const char* signature)
{
	/*const char* pat = szPattern;
	DWORD firstMatch = NULL;
	for (DWORD pCur = dwAddress; pCur < dwLength; pCur++)
	{
		if (!*pat) return firstMatch;
		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
			if (!firstMatch) firstMatch = pCur;
			if (!pat[2]) return firstMatch;
			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
			else pat += 2;
		}
		else {
			pat = szPattern;
			firstMatch = 0;
		}
	}
	return NULL;*/

  /*if ( const auto module_handle = GetModuleHandleA( module_name ) ) {
			static auto signature_to_byte = [ ] ( const char* signature ) {
				auto bytes = std::vector< int >{ };
				auto start = const_cast< char* >( signature );
				auto end = const_cast< char* >( signature ) + strlen( signature );

				for ( auto current = start; current < end; ++current ) {
					if ( *current == '?' ) {
						++current;

						if ( *current == '?' )
							++current;

						bytes.push_back( -1 );
					}

					else bytes.push_back( strtoul( current, &current, 16 ) );
				}

				return bytes;
			};

			auto dos_header = reinterpret_cast< PIMAGE_DOS_HEADER >( module_handle );
			auto nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS >( reinterpret_cast< std::uint8_t* >( module_handle ) + dos_header->e_lfanew );

			auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
			auto pattern_bytes = signature_to_byte( signature );
			auto scan_bytes = reinterpret_cast< std::uint8_t* >( module_handle );

			auto s = pattern_bytes.size( );
			auto d = pattern_bytes.data( );

			for ( auto i = 0ul; i < size_of_image - s; ++i ) {
				bool found = true;

				for ( auto j = 0ul; j < s; ++j ) {
					if ( scan_bytes[ i + j ] != d[ j ] && d[ j ] != -1 ) {
						found = false;
						break;
					}
				}

				if ( found )
					return reinterpret_cast< DWORD >( &scan_bytes[ i ] );
			}
		}

		return (DWORD)0x0;*/

  auto sigToBytes = [](const char* const sig) -> std::vector<int>
  {
      std::vector<int> bytes{};

      char* const start{ const_cast<char*>(sig) };
      char* const end{ const_cast<char*>(sig) + strlen(sig) };

      for (char* current{ start }; current < end; ++current)
      {
          if (*current == '?')
          {
              ++current;

              if (*current == '?') {
                  ++current;
              }

              bytes.push_back(-1);
          }

          else {
              bytes.push_back(strtoul(current, &current, 16));
          }
      }

      return bytes;
  };

  const HMODULE hmod{ GetModuleHandleA(module_name) };

  if (!hmod) {
      return 0;
  }

  MODULEINFO mod_info{};

  if (!K32GetModuleInformation(GetCurrentProcess(), hmod, &mod_info, sizeof(MODULEINFO))) {
      return 0;
  }

  const DWORD image_size{ mod_info.SizeOfImage };

  if (!image_size) {
      return 0;
  }

  const std::vector<int> pattern_bytes{ sigToBytes(signature) };
  const unsigned char* const image_bytes{ reinterpret_cast<unsigned char*>(hmod) };
  const size_t signature_size{ pattern_bytes.size() };
  const int* signature_bytes{ pattern_bytes.data() };

  for (DWORD i{}; i < image_size - signature_size; ++i)
  {
      bool byte_sequence_found{ true };

      for (DWORD j{}; j < signature_size; ++j)
      {
          if (image_bytes[i + j] != signature_bytes[j] && signature_bytes[j] != -1) {
              byte_sequence_found = false;
              break;
          }
      }

      if (byte_sequence_found) {
          return reinterpret_cast<uintptr_t>(&image_bytes[i]);
      }
  }

  return 0x0;
}
//===================================================================================
HMODULE CSignature::GetModuleHandleSafe( const char* pszModuleName )
{
	HMODULE hmModuleHandle = NULL;

	do
	{
		hmModuleHandle = GetModuleHandle( pszModuleName );
		Sleep( 1 );
	}
	while(hmModuleHandle == NULL);

	return hmModuleHandle;
}
//===================================================================================
DWORD CSignature::GetClientSignature(const char* chPattern)
{
	/*static HMODULE hmModule = GetModuleHandleSafe("client.dll");
	static PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hmModule;
	static PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)(((DWORD)hmModule) + pDOSHeader->e_lfanew);
	return dwFindPattern(((DWORD)hmModule) + pNTHeaders->OptionalHeader.BaseOfCode, ((DWORD)hmModule) + pNTHeaders->OptionalHeader.SizeOfCode, chPattern);*/

  return dwFindPattern("client.dll", chPattern);
}
//===================================================================================
DWORD CSignature::GetEngineSignature(const char* chPattern)
{
	/*static HMODULE hmModule = GetModuleHandleSafe("engine.dll");
	static PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hmModule;
	static PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)(((DWORD)hmModule) + pDOSHeader->e_lfanew);
	return dwFindPattern(((DWORD)hmModule) + pNTHeaders->OptionalHeader.BaseOfCode, ((DWORD)hmModule) + pNTHeaders->OptionalHeader.SizeOfCode, chPattern);*/

  return dwFindPattern("engine.dll", chPattern);
}

CSignature gSignatures;
