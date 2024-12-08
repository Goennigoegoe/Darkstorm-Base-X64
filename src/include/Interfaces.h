#pragma once

#include <windows.h>
#include "getvfunc.h"

class CEntList;
class EngineClient;
class IPanel;
class ISurface;
class ClientModeShared;
//class CHLClient;
class IEngineVGui;
class CPlayerVariables;
class COffsets;
class IClientNetworkable;
//class ClientClass;

typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

class ClientClass
{
public:
	const char* GetName() { return m_pNetworkName; }

public:
	CreateClientClassFn	m_pCreateFn;
	CreateEventFn m_pCreateEventFn;
	const char* m_pNetworkName;
	RecvTable* m_pRecvTable;
	ClientClass* m_pNext;
	int m_ClassID;
};


class CHLClient
{
public:
	ClientClass* GetAllClasses( void )
	{
		typedef ClientClass* ( __thiscall* OriginalFn )( PVOID ); //Anything inside a VTable is a __thiscall unless it completly disregards the thisptr. You can also call them as __stdcalls, but you won't have access to the __thisptr.
		return find_vfunc<OriginalFn>( this, 8 )( this ); //Return the pointer to the head CClientClass.
	}
};

class CInterfaces
{
public:
	CEntList* EntList;
	EngineClient* Engine;
	IPanel* Panels;
	ISurface* Surface;
	ClientModeShared* ClientMode;
	CHLClient* Client;
  IEngineVGui* EngineVGui;
  //CMatSystemSurface* MatSystemSurface;
};

extern CInterfaces gInts;
extern CPlayerVariables gPlayerVars;
extern COffsets gOffsets;
