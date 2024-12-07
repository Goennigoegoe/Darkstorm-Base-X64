#include "include/SDK.h"
#include "include/Client.hpp"
#include "include/Panels.hpp"

#include <iomanip>
#include <psapi.h>
#include <thread>
#include <chrono>
#include <cstdint>
#include <array>
#include <algorithm>
#include <string>
#include <string_view>
#include <intrin.h>
#include <map>
#include <functional>
#include <sstream>
#include <fstream>
#include <winternl.h>
#include <ImageHlp.h>
#include <vector>
#include <list>
#include <cstdint>
//#include <d3d9.h>
//#include <d3dx9.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm")
#pragma comment(lib, "imagehlp.lib")

COffsets gOffsets;
CPlayerVariables gPlayerVars;
CInterfaces gInts;

CreateInterface_t EngineFactory = NULL;
CreateInterface_t ClientFactory = NULL;
CreateInterface_t VGUIFactory = NULL;
CreateInterface_t VGUI2Factory = NULL;

struct StackFrame
{
	DWORD address;
	std::string name;
	std::string module;
	unsigned int line;
	std::string file;
};

std::vector<StackFrame> GetStackTrace(PCONTEXT context) {
  HANDLE process = GetCurrentProcess();
	HANDLE thread = GetCurrentThread();

	if (SymInitialize(process, nullptr, TRUE) == FALSE)
		return {};

	SymSetOptions(SYMOPT_LOAD_LINES);

	STACKFRAME frame = {};
	frame.AddrPC.Offset = context->Rip;
	frame.AddrPC.Mode = AddrModeFlat;
	frame.AddrFrame.Offset = context->Rbp;
	frame.AddrFrame.Mode = AddrModeFlat;
	frame.AddrStack.Offset = context->Rsp;
	frame.AddrStack.Mode = AddrModeFlat;

	bool first = true;

	std::vector<StackFrame> frames;
	while (StackWalk(IMAGE_FILE_MACHINE_I386, process, thread, &frame, context, nullptr, SymFunctionTableAccess, SymGetModuleBase, nullptr)) {
		StackFrame f = {};
		f.address = frame.AddrPC.Offset;

		auto moduleBase = SymGetModuleBase(process, frame.AddrPC.Offset);

		char moduleBuf[MAX_PATH];
		if (moduleBase && GetModuleFileNameA((HINSTANCE)moduleBase, moduleBuf, MAX_PATH))
			f.module = moduleBuf;
		else
			f.module = "Unknown Module";

		PDWORD64 offset = 0;
		char symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 255];
		auto symbol = (PIMAGEHLP_SYMBOL)symbolBuffer;
		symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL) + 255;
		symbol->MaxNameLength = 254;

		if (SymGetSymFromAddr(process, frame.AddrPC.Offset, offset, symbol)) {
			f.name = symbol->Name;
		}
		else {
			f.name = "Unknown Function";
		}

		IMAGEHLP_LINE line{};
		line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

		DWORD offset_ln = 0;
		if (SymGetLineFromAddr(process, frame.AddrPC.Offset, &offset_ln, &line)) {
			f.file = line.FileName;
			f.line = line.LineNumber;
		}
		else {
			f.line = 0;
		}

		if (!first) {
			frames.push_back(f);
		}
		first = false;
	}

	SymCleanup(process);

	return frames;
}

long APIENTRY ExceptionFilter(PEXCEPTION_POINTERS Info)
{
	if (Info->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION)
	{
		return EXCEPTION_EXECUTE_HANDLER;
	}

	std::stringstream error;
	error << "Copy this error message and send it to nullptr by using CTRL+C and CTRL+V.\n";
	error << "Exception code: 0x" << std::hex << std::uppercase << Info->ExceptionRecord->ExceptionCode << " at 0x" << std::hex << std::uppercase << Info->ExceptionRecord->ExceptionAddress << "\n\n";
	error << "EAX: 0x" << std::hex << std::uppercase << Info->ContextRecord->Rax << "\n";
	error << "EBX: 0x" << std::hex << std::uppercase << Info->ContextRecord->Rbx << "\n";
	error << "ECX: 0x" << std::hex << std::uppercase << Info->ContextRecord->Rcx << "\n";
	error << "EDX: 0x" << std::hex << std::uppercase << Info->ContextRecord->Rdx << "\n";
	error << "EBP: 0x" << std::hex << std::uppercase << Info->ContextRecord->Rbp << "\n";
	error << "ESP: 0x" << std::hex << std::uppercase << Info->ContextRecord->Rsp << "\n";
	error << "ESI: 0x" << std::hex << std::uppercase << Info->ContextRecord->Rsi << "\n";
	error << "EDI: 0x" << std::hex << std::uppercase << Info->ContextRecord->Rdi << "\n";
	//error << "EIP: 0x" << std::hex << std::uppercase << Info->ContextRecord->Eip << "\n";
	error << "EFLAGS: 0x" << std::hex << std::uppercase << Info->ContextRecord->EFlags << "\n\n";

	for (auto& frame : GetStackTrace(Info->ContextRecord))
	{
		error << std::hex << std::uppercase << frame.address << std::resetiosflags << " in " << frame.module << " - " << frame.name << "() - " << frame.file << ": " << std::dec << frame.line << "\n";
	}

	MessageBoxA(nullptr, error.str().c_str(), "Exception found!", MB_OK);

	return EXCEPTION_EXECUTE_HANDLER;
}

DWORD WINAPI dwMainThread( LPVOID lpArguments )
{
  AllocConsole();

  FILE* fp;   
  freopen_s(&fp, "CONOUT$", "w", stdout);

  /* Initialize MinHook */
  if( MH_Initialize() != MH_OK )
  {
    if( MessageBoxA( 0, "Minhook failed to initialize", "DarkStorm", MB_OK ) )
    {
      fclose(fp);
      return 1;
    }
  }

  Sleep(50); // wait 50 ms, not sure if needed but in my older project it waits 50 ms after initializing MinHook

	if (gInts.Client == NULL) //Prevent repeat callings.
	{
		//Gottammove those factorys up.
		//Grab the factorys from their resptive module's EAT.
		ClientFactory = ( CreateInterfaceFn ) GetProcAddress( gSignatures.GetModuleHandleSafe( "client.dll" ), "CreateInterface" );
		gInts.Client = ( CHLClient* )ClientFactory( "VClient017", NULL);
		XASSERT(gInts.Client);
		gInts.EntList = ( CEntList* ) ClientFactory( "VClientEntityList003", NULL );
		XASSERT(gInts.EntList);
		EngineFactory = ( CreateInterfaceFn ) GetProcAddress( gSignatures.GetModuleHandleSafe( "engine.dll" ), "CreateInterface" );
		gInts.Engine = ( EngineClient* ) EngineFactory( "VEngineClient014", NULL );
		XASSERT(gInts.Engine);
		VGUIFactory = ( CreateInterfaceFn ) GetProcAddress( gSignatures.GetModuleHandleSafe( "vguimatsurface.dll" ), "CreateInterface" );
		gInts.Surface = ( ISurface* ) VGUIFactory( "VGUI_Surface030", NULL );
		XASSERT(gInts.Surface);
    gInts.EngineVGui = ( IEngineVGui* ) EngineFactory( "VEngineVGui002", NULL );
    XASSERT(gInts.EngineVGui);

		//Setup the Panel hook so we can draw.
		if( !gInts.Panels )
		{
			VGUI2Factory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("vgui2.dll"), "CreateInterface");
			gInts.Panels = ( IPanel* ) VGUI2Factory( "VGUI_Panel009", NULL );
			XASSERT( gInts.Panels );

			if( gInts.Panels )
			{
        /* Create PaintTraverse hook */
        InitPaintTraverseHook();
				/*VMTBaseManager* panelHook = new VMTBaseManager(); //Setup our VMTBaseManager for Panels.
				panelHook->Init(gInts.Panels);
				panelHook->HookMethod(&Hooked_PaintTraverse, gOffsets.iPaintTraverseOffset);
				panelHook->Rehook();*/
			}
		}

    /*std::uintptr_t dwClientModeAddress = gSignatures.GetClientSignature("48 8B 0D ? ? ? ? 48 8B 10 48 8B 19 48 8B C8 FF 92"); // This needs to be updated, Should be fixed now
		XASSERT(dwClientModeAddress);
		if (dwClientModeAddress == 0)
		{
      MessageBox(0, "ClientMode == 0", "DarkStorm", MB_OK);
      return 1;
		}
    std::uintptr_t absCM = *reinterpret_cast<std::uintptr_t*>(dwClientModeAddress + 0x3) + dwClientModeAddress + 0x7;
		gInts.ClientMode = **reinterpret_cast<ClientModeShared***>(absCM); // 0x2 should be fine but might need to add 0x3*/
    
    const auto vfunc10 = getvfunc<DWORD_PTR>(gInts.Client, 10);
    gInts.ClientMode = *reinterpret_cast<ClientModeShared**>(*reinterpret_cast<DWORD*>(vfunc10 + 0x03) + vfunc10 + 0x07);

		//LOGDEBUG("g_pClientModeShared_ptr client.dll+0x%X", (DWORD)gInts.ClientMode - dwClientBase);

    /* Create CreateMove hook */
    InitCreateMove();

    MH_EnableHook(MH_ALL_HOOKS);

		/*VMTBaseManager* clientModeHook = new VMTBaseManager(); //Setup our VMTBaseManager for ClientMode.
		clientModeHook->Init(gInts.ClientMode);
		clientModeHook->HookMethod(&Hooked_CreateMove, gOffsets.iCreateMoveOffset); //ClientMode create move is called inside of CHLClient::CreateMove, and thus no need for hooking WriteUserCmdDelta.
		clientModeHook->Rehook();*/
	}

  fclose(fp);
	return 0; //The thread has been completed, and we do not need to call anything once we're done. The call to Hooked_PaintTraverse is now our main thread.
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD dwReason, LPVOID lpReserved)
{
  AddVectoredExceptionHandler(1, ExceptionFilter);
	//If you manually map, make sure you setup this function properly.
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		//Log::Init(hInstance);
		CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)dwMainThread, NULL, 0, NULL ); //CreateThread > _BeginThread. (For what we're doing, of course.)
	}
	return true;
}
