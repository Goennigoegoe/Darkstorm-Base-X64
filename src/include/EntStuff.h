#pragma once

#include <windows.h>
#include <math.h>
#include <xstring>
#include "Vector.h"
#include "getvfunc.h"
#include "ClassIDList.h"

typedef float matrix3x4[3][4];

class IClientNetworkable;
class CBaseEntity;
class IClientRenderable;
class IClientEntity;
class IClientThinkable;

typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

/*class ClientClass
{
private:
	BYTE _chPadding[8];
public:
	char* chName;
	RecvTable* Table;
	ClientClass* pNextClass;
	int iClassID;
};*/

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

class IHandleEntity
{
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle(const DWORD& handle) = 0;
	virtual const DWORD& GetRefEHandle() const = 0;
};

class IClientUnknown : public IHandleEntity
{
public:
	virtual DWORD* GetCollideable() = 0;
	virtual IClientNetworkable* GetClientNetworkable() = 0;
	virtual IClientRenderable* GetClientRenderable() = 0;
	virtual IClientEntity* GetIClientEntity() = 0;
	virtual CBaseEntity* GetBaseEntity() = 0;
	virtual IClientThinkable* GetClientThinkable() = 0;
};

class IClientThinkable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void ClientThink() = 0;
	virtual DWORD	GetThinkHandle() = 0;
	virtual void SetThinkHandle(DWORD hThink) = 0;
	virtual void Release() = 0;
};

typedef unsigned short ClientShadowHandle_t;

enum
{
	CLIENTSHADOW_INVALID_HANDLE = (ClientShadowHandle_t)~0
};

enum ShadowType_t
{
	SHADOWS_NONE = 0,
	SHADOWS_SIMPLE,
	SHADOWS_RENDER_TO_TEXTURE,
	SHADOWS_RENDER_TO_TEXTURE_DYNAMIC,
	SHADOWS_RENDER_TO_DEPTH_TEXTURE
};

class IPVSNotify
{
public:
	virtual void OnPVSStatusChanged(bool bInPVS) = 0;
};

using ClientRenderHandle_t = void*;
using ModelInstanceHandle_t = unsigned short;
struct model_t;

class IClientRenderable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual Vector const& GetRenderOrigin(void) = 0;
	virtual Vector const& GetRenderAngles(void) = 0;
	virtual bool ShouldDraw(void) = 0;
	virtual bool IsTransparent(void) = 0;
	virtual bool UsesPowerOfTwoFrameBufferTexture() = 0;
	virtual bool UsesFullFrameBufferTexture() = 0;
	virtual ClientShadowHandle_t GetShadowHandle() const = 0;
	virtual ClientRenderHandle_t& RenderHandle() = 0;
	virtual const model_t* GetModel() const = 0;
	virtual int DrawModel(int flags) = 0;
	virtual int	GetBody() = 0;
	virtual void ComputeFxBlend() = 0;
	virtual int	GetFxBlend(void) = 0;
	virtual void GetColorModulation(float* color) = 0;
	virtual bool LODTest() = 0;
	virtual bool SetupBones(matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;
	virtual void SetupWeights(const matrix3x4* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights) = 0;
	virtual void DoAnimationEvents(void) = 0;
	virtual IPVSNotify* GetPVSNotifyInterface() = 0;
	virtual void GetRenderBounds(Vector& mins, Vector& maxs) = 0;
	virtual void GetRenderBoundsWorldspace(Vector& mins, Vector& maxs) = 0;
	virtual void GetShadowRenderBounds(Vector& mins, Vector& maxs, ShadowType_t shadowType) = 0;
	virtual bool ShouldReceiveProjectedTextures(int flags) = 0;
	virtual bool GetShadowCastDistance(float* pDist, ShadowType_t shadowType) const = 0;
	virtual bool GetShadowCastDirection(Vector* pDirection, ShadowType_t shadowType) const = 0;
	virtual bool IsShadowDirty() = 0;
	virtual void MarkShadowDirty(bool bDirty) = 0;
	virtual IClientRenderable* GetShadowParent() = 0;
	virtual IClientRenderable* FirstShadowChild() = 0;
	virtual IClientRenderable* NextShadowPeer() = 0;
	virtual ShadowType_t ShadowCastType() = 0;
	virtual void CreateModelInstance() = 0;
	virtual ModelInstanceHandle_t GetModelInstance() = 0;
	virtual const matrix3x4& RenderableToWorldTransform() = 0;
	virtual int LookupAttachment(const char* pAttachmentName) = 0;
	virtual	bool GetAttachment(int number, Vector& origin, Vector& angles) = 0;
	virtual bool GetAttachment(int number, matrix3x4& matrix) = 0;
	virtual float* GetRenderClipPlane(void) = 0;
	virtual int GetSkin() = 0;
	virtual bool IsTwoPass(void) = 0;
	virtual void OnThreadedDrawSetup() = 0;
	virtual bool UsesFlexDelayedWeights() = 0;
	virtual void RecordToolMessage() = 0;
	virtual bool IgnoresZBuffer(void) const = 0;
};

enum ShouldTransmitState_t
{
	SHOULDTRANSMIT_START = 0,
	SHOULDTRANSMIT_END
};

enum DataUpdateType_t
{
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED
};

class IClientNetworkable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void Release() = 0;
	virtual ClientClass* GetClientClass() = 0;
	virtual void NotifyShouldTransmit(ShouldTransmitState_t state) = 0;
	virtual void OnPreDataChanged(DataUpdateType_t updateType) = 0;
	virtual void OnDataChanged(DataUpdateType_t updateType) = 0;
	virtual void PreDataUpdate(DataUpdateType_t updateType) = 0;
	virtual void PostDataUpdate(DataUpdateType_t updateType) = 0;
	virtual bool IsDormant(void) = 0;
	virtual int entindex(void) const = 0;
	virtual void ReceiveMessage(int classID, DWORD& msg) = 0;
	virtual void* GetDataTableBasePtr() = 0;
	virtual void SetDestroyedOnRecreateEntities(void) = 0;
	virtual void OnDataUnchangedInPVS() = 0;
  
	inline ETFClassID GetClassID()
	{
		if (auto pClientClass = GetClientClass())
			return static_cast<ETFClassID>(pClientClass->m_ClassID);

		return static_cast<ETFClassID>(0);
	}
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	virtual void Release(void) = 0;
	virtual const Vector& GetAbsOrigin(void) const = 0;
	virtual const Vector& GetAbsAngles(void) const = 0;
	virtual DWORD* GetMouth(void) = 0;
	virtual bool GetSoundSpatialization(DWORD& info) = 0;

	inline bool IsPlayer()
	{
		return GetClassID() == ETFClassID::CTFPlayer;
	}

	inline bool IsSentrygun()
	{
		return GetClassID() == ETFClassID::CObjectSentrygun;
	}

	inline bool IsDispenser()
	{
		return GetClassID() == ETFClassID::CObjectDispenser;
	}

	inline bool IsTeleporter()
	{
		return GetClassID() == ETFClassID::CObjectTeleporter;
	}

	inline bool IsBaseCombatWeapon()
	{
		return GetClassID() == ETFClassID::CBaseCombatWeapon;
	}

	inline bool IsWearable()
	{
		return GetClassID() == ETFClassID::CTFWearable;
	}

	inline bool IsBuilding()
	{
		switch (GetClassID())
		{
		case ETFClassID::CObjectDispenser:
		case ETFClassID::CObjectSentrygun:
		case ETFClassID::CObjectTeleporter: return true;
		default: return false;
		}
	}

  // Implement Later
	/*inline bool IsPickup()
	{
		switch (GetClassID())
		{
		case ETFClassID::CBaseAnimating: return I::ModelInfoClient->GetModelName(GetModel())[24] != 'h';
		case ETFClassID::CTFAmmoPack: return true;
		default: return false;
		}
	}*/

	inline bool IsNPC()
	{
		switch (GetClassID())
		{
		case ETFClassID::CHeadlessHatman:
		case ETFClassID::CTFTankBoss:
		case ETFClassID::CMerasmus:
		case ETFClassID::CZombie:
		case ETFClassID::CEyeballBoss:
			return true;
		default: return false;
		}
	}

	inline bool IsBomb()
	{
		switch (GetClassID())
		{
		case ETFClassID::CTFPumpkinBomb:
		case ETFClassID::CTFGenericBomb:
			return true;
		default: return false;
		}
	}

	template <typename T> inline T* As() { return reinterpret_cast<T*>(this); }
};
