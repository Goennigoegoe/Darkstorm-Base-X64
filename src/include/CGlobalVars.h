#include "SDK.h"

class CPlayerVariables
{
public:
	int iWeaponID;
	bool bHasMeleeWeapon;
	bool bHasFlameThrower;
	int iClass;
	int iWeaponSlot;
	int iPlayerCond;
	int iPlayerCondEx;
	int iFlags;
	char chName[34];
};

class COffsets
{
public:
	unsigned int iCreateMoveOffset = 21u, iPaintTraverseOffset = 41u;
};
