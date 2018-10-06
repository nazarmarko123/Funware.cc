#include "sdk.h"
#include <Psapi.h>
#include "xor.h"
#include "global.h"
#include "Math.h"
offsets_t offys;


DWORD FindSig(DWORD dwAddress, DWORD dwLength, const char* szPattern)
{
#define INRANGE(x,a,b)  (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),XorStr('A'),XorStr('F')) ? ((x&(~0x20)) - XorStr('A') + 0xa) : (INRANGE(x,XorStr('0'),XorStr('9')) ? x - XorStr('0') : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

	if (!dwAddress || !dwLength || !szPattern)
		return 0;

	const char* pat = szPattern;
	DWORD firstMatch = NULL;

	for (DWORD pCur = dwAddress; pCur < dwLength; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else pat += 2;
		}
		else
		{
			pat = szPattern;
			firstMatch = 0;
		}
	}

	return 0;
}
DWORD FindSignaturenew(const char* szModuleName, const char* PatternName, char* szPattern)
{
	HMODULE hModule = GetModuleHandleA(szModuleName);
	PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)(((DWORD)hModule) + pDOSHeader->e_lfanew);

	DWORD ret = FindSig(((DWORD)hModule) + pNTHeaders->OptionalHeader.BaseOfCode, ((DWORD)hModule) + pNTHeaders->OptionalHeader.SizeOfCode, szPattern);;

	return ret;
}

CBaseCombatWeapon* xd(CBaseEntity* xz)
{
	if (!g_pEngine->IsConnected())
		return nullptr;
	if (!xz->isAlive())
		return nullptr;

	HANDLE weaponData = *(HANDLE*)((DWORD)xz + 0x2EE8);
	return (CBaseCombatWeapon*)g_pEntitylist->GetClientEntityFromHandle(weaponData);
}
short SafeWeaponID()
{
	CBaseEntity* local_player = G::LocalPlayer;
	if (!(local_player))
		return 0;

	CBaseCombatWeapon* WeaponC = xd(local_player);

	if (!(WeaponC))
		return 0;

	return WeaponC->GetItemDefinitionIndex();
}

CBaseCombatWeapon* GetActiveBaseCombatWeapon(CBaseEntity* local_player)
{
	//CANCRUSH
	CBaseHandle pWeepEhandle = *reinterpret_cast<CBaseHandle*>(uintptr_t(local_player) + 0x2EE8);
	return (CBaseCombatWeapon*)(g_pEntitylist->GetClientEntityFromHandle(pWeepEhandle));
}





uint64_t FindPatternIDA(const char* szModule, const char* szSignature)
{

#define INRANGE(x, a, b) (x >= a && x <= b)
#define getBits(x) (INRANGE((x & (~0x20)), XorStr('A'), XorStr('F')) ? ((x & (~0x20)) - XorStr('A') + 0xa) : (INRANGE(x, XorStr('0'), XorStr('9')) ? x - XorStr('0') : 0))
#define getByte(x) (getBits(x[0]) << 4 | getBits(x[1]))

	MODULEINFO modInfo;
	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
	DWORD startAddress = (DWORD)modInfo.lpBaseOfDll;
	DWORD endAddress = startAddress + modInfo.SizeOfImage;
	const char* pat = szSignature;
	DWORD firstMatch = 0;
	for (DWORD pCur = startAddress; pCur < endAddress; pCur++) {
		if (!*pat)
			return firstMatch;
		if (*(PBYTE)pat == XorStr('\?') || *(BYTE*)pCur == getByte(pat)) {
			if (!firstMatch)
				firstMatch = pCur;
			if (!pat[2])
				return firstMatch;
			if (*(PWORD)pat == XorStr('\?\?') || *(PBYTE)pat != XorStr('\?'))
				pat += 3;
			else
				pat += 2;
		}
		else {
			pat = szSignature;
			firstMatch = 0;
		}
	}
	return NULL;
}

static bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
{
	for (; *szMask; ++szMask, ++Mask, ++Data) {
		if (*szMask == 'x' && *Mask != *Data) {
			return false;
		}
	}
	return (*szMask) == 0;
}

DWORD WaitOnModuleHandle(std::string moduleName)
{
	DWORD ModuleHandle = NULL;
	while (!ModuleHandle) {
		ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
		if (!ModuleHandle)
			Sleep(50);
	}
	return ModuleHandle;
}

DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo;
	GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;
	for (DWORD c = 0; c < Length; c += 1) {
		if (bCompare((BYTE*)(Address + c), Mask, szMask)) {
			return DWORD(Address + c);
		}
	}
	return 0;
}



Vector TraceToEnd(Vector start, Vector end)
{
	trace_t trace;
	CTraceWorldOnly filter;
	Ray_t ray;

	ray.Init(start, end);
	g_pEngineTrace->TraceRayxd(ray, MASK_ALL, &filter, &trace);

	return trace.endpos;
}
const char* GetHitgroupName(int hitgroup)
{
	switch (hitgroup)
	{
	case 1:
		return "Head";
	case 2:
		return "Chest";
	case 3:
		return "Pelvis";
	case 4:
		return "Left Arm";
	case 5:
		return "Right Arm";
	case 6:
		return "Left Leg";
	case 7:
		return "Right Leg";
	default:
		return "";
	}
}

float GetCurtime()
{
	if (!g_pEngine->IsConnected())
		return 0.f;

	if (!g_pEngine->IsInGame())
		return 0.f;

	CBaseEntity* local_player = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!local_player)
		return 0.f;

	return local_player->GetTickBase() * g_pGlobals->interval_per_tick;
}


float GetLBYRotatedYaw(float lby, float yaw)
{
	float delta = Math::NormalizeYaw(yaw - lby);
	if (fabs(delta) < 25.f)
		return lby;

	if (delta > 0.f)
		return yaw + 25.f;

	return yaw;
}



void SetupOffsets()
{
	printf(XorStr(":::::Adresses & Offsets:::::\n"));
	g_pNetVars = new CNetVars();

	offys.nWriteableBones = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_nForceBone")) + 0x20;
	offys.dwOcclusionArray = *(uintptr_t*)(FindPatternIDA(XorStr("client_panorama.dll"), XorStr("A1 ? ? ? ? 8B B7 ? ? ? ? 89 75 F8")) + 0x1);
	offys.bDidCheckForOcclusion = *(uintptr_t*)(FindPatternIDA(XorStr("client_panorama.dll"), XorStr("A1 ? ? ? ? 8B B7 ? ? ? ? 89 75 F8")) + 0x7);
	offys.InvalidateBoneCache = FindPatternIDA("client_panorama.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");

	printf(XorStr("Array: 0x%X\n"), (DWORD)offys.dwOcclusionArray);
	printf(XorStr("Bool: 0x%X\n"), (DWORD)offys.bDidCheckForOcclusion);

	offys.dwInitKeyValues = FindPatternIDA(XorStr("client_panorama.dll"), XorStr("8B 0E 33 4D FC 81 E1 ? ? ? ? 31 0E 88 46 03 C1 F8 08 66 89 46 12 8B C6")) - 0x45;
	printf(XorStr("InitKeyValues: 0x%X\n"), (DWORD)offys.dwInitKeyValues);

	offys.dwLoadFromBuffer = FindPatternIDA(XorStr("client_panorama.dll"), XorStr("55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04"));
	printf(XorStr("LoadFromBuffer: 0x%X\n"), (DWORD)offys.dwLoadFromBuffer);

	offys.m_ArmorValue = g_pNetVars->GetOffset("DT_CSPlayer", "m_ArmorValue");
	offys.m_bHasHelmet = g_pNetVars->GetOffset("DT_CSPlayer", "m_bHasHelmet");

	offys.m_angRotation = g_pNetVars->GetOffset("DT_CSPlayer", "m_angRotation");

	offys.animstate = 0x3884;
	offys.m_hObserverTarget = g_pNetVars->GetOffset("DT_CSPlayer", "m_hObserverTarget");
	offys.m_flNextAttack = g_pNetVars->GetOffset("DT_BCCLocalPlayerExclusive", "m_flNextAttack");

	offys.m_flPoseParameter = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_flPoseParameter"));
	offys.m_flCycle = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_flCycle"));
	offys.m_flSimulationTime = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_flSimulationTime"));
	offys.m_flAnimTime = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_flAnimTime"));
	offys.m_nSequence = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_nSequence"));
	offys.m_flLowerBodyYawTarget = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_flLowerBodyYawTarget"));
	offys.m_flFlashMaxAlpha = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_flFlashMaxAlpha"));
	offys.m_angEyeAngles = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_angEyeAngles[0]"));
	offys.m_nTickBase = g_pNetVars->GetOffset(XorStr("DT_BasePlayer"), XorStr("m_nTickBase"));
	offys.m_fFlags = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_fFlags"));
	offys.m_flNextPrimaryAttack = g_pNetVars->GetOffset(XorStr("DT_BaseCombatWeapon"), XorStr("m_flNextPrimaryAttack"));
	offys.m_vecOrigin = g_pNetVars->GetOffset(XorStr("DT_BaseEntity"), XorStr("m_vecOrigin"));
	offys.m_vecViewOffset = g_pNetVars->GetOffset(XorStr("DT_BasePlayer"), XorStr("m_vecViewOffset[0]"));
	offys.m_vecVelocity = g_pNetVars->GetOffset("DT_CSPlayer", "m_vecVelocity[0]");
	offys.m_vecBaseVelocity = g_pNetVars->GetOffset("DT_CSPlayer", "m_vecBaseVelocity");
	offys.m_flFallVelocity = g_pNetVars->GetOffset("DT_CSPlayer", "m_flFallVelocity");
	offys.m_bPinPulled = g_pNetVars->GetOffset(XorStr("DT_BaseCSGrenade"), XorStr("m_bPinPulled"));
	offys.m_fThrowTime = g_pNetVars->GetOffset(XorStr("DT_BaseCSGrenade"), XorStr("m_fThrowTime"));
	offys.m_fAccuracyPenalty = g_pNetVars->GetOffset("DT_WeaponCSBase", "m_fAccuracyPenalty");
	offys.m_bReloadVisuallyComplete = g_pNetVars->GetOffset("DT_WeaponFiveSeven", "m_bReloadVisuallyComplete");
	offys.m_iPrimaryReserveAmmoCount = g_pNetVars->GetOffset("DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount");

	offys.m_flC4Blow = g_pNetVars->GetOffset(XorStr("DT_PlantedC4"), XorStr("m_flC4Blow"));
	offys.m_bBombDefused = g_pNetVars->GetOffset(XorStr("DT_PlantedC4"), XorStr("m_bBombDefused"));
	offys.m_hOwnerEntity = g_pNetVars->GetOffset(XorStr("DT_PlantedC4"), XorStr("m_hOwnerEntity"));

	offys.m_flFriction = g_pNetVars->GetOffset("DT_CSPlayer", "m_flFriction");
	offys.m_CollisionGroup = g_pNetVars->GetOffset("DT_BaseEntity", "m_CollisionGroup");
	offys.m_bIsScoped = g_pNetVars->GetOffset("DT_CSPlayer", "m_bIsScoped");
	offys.m_iAccountID = g_pNetVars->GetOffset("DT_BaseCombatWeapon", "m_iAccountID");
	uintptr_t player_resource_pointer = uintptr_t((uintptr_t)GetModuleHandle("client_panorama.dll") + 0x2EC5ADC);
	offys.dw_CSPlayerResource = (DWORD)player_resource_pointer;
	offys.m_iCompetetiveRanking = g_pNetVars->GetOffset(XorStr("DT_CSPlayerResource"), XorStr("m_iCompetitiveRanking"));
	offys.m_iCompetetiveWins = g_pNetVars->GetOffset(XorStr("DT_CSPlayerResource"), XorStr("m_iCompetitiveWins"));
	offys.m_iPing = g_pNetVars->GetOffset(XorStr("DT_CSPlayerResource"), XorStr("m_iPing"));

	offys.m_bIsValveDS = g_pNetVars->GetOffset(XorStr("DT_CSGameRulesProxy"), XorStr("m_bIsValveDS"));

	offys.m_hMyWearables = g_pNetVars->GetOffset(XorStr("DT_CSPlayer"), XorStr("m_hMyWearables"));

	offys.m_hMyWeapons = g_pNetVars->GetOffset(XorStr("DT_BasePlayer"), XorStr("m_hMyWeapons"));
	offys.m_hActiveWeapon = g_pNetVars->GetOffset(XorStr("DT_BasePlayer"), XorStr("m_hActiveWeapon"));


	offys.m_flNextSecondaryAttack = g_pNetVars->GetOffset(XorStr("DT_LocalActiveWeaponData"), XorStr("m_flNextSecondaryAttack"));


	offys.m_flFriction = g_pNetVars->GetOffset("DT_CSPlayer", "m_flFriction");
	offys.m_flMaxspeed = g_pNetVars->GetOffset("DT_BasePlayer", "m_flMaxspeed");
	offys.m_flStepSize = g_pNetVars->GetOffset("DT_CSPlayer", "m_flStepSize");
	offys.m_bGunGameImmunity = g_pNetVars->GetOffset("DT_CSPlayer", "m_bGunGameImmunity");

	offys.m_flPostponeFireReadyTime = g_pNetVars->GetOffset("DT_WeaponCSBaseGun", "m_flPostponeFireReadyTime");
	offys.m_iShotsFired = g_pNetVars->GetOffset("DT_CSPlayer", "m_iShotsFired");

	offys.m_iItemDefinitionIndex = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	offys.m_iItemIDHigh = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_iItemIDHigh");
	offys.m_iItemIDLow = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_iItemIDLow");
	offys.m_iEntityQuality = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_iEntityQuality");
	offys.m_szCustomName = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_szCustomName");
	offys.m_OriginalOwnerXuidLow = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
	offys.m_OriginalOwnerXuidHigh = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
	offys.m_nFallbackPaintKit = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_nFallbackPaintKit");
	offys.m_nFallbackSeed = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_nFallbackSeed");
	offys.m_flFallbackWear = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_flFallbackWear");
	offys.m_nFallbackStatTrak = g_pNetVars->GetOffset("DT_BaseAttributableItem", "m_nFallbackStatTrak");
	offys.m_nAccountID = g_pNetVars->GetOffset("DT_FEPlayerDecal", "m_unAccountID");
	offys.m_iViewModelIndex = g_pNetVars->GetOffset("DT_BaseCombatWeapon", "m_iViewModelIndex");
	offys.m_iWorldModelIndex = g_pNetVars->GetOffset("DT_BaseCombatWeapon", "m_iWorldModelIndex");
	offys.m_hWeaponWorldModel = g_pNetVars->GetOffset("DT_BaseCombatWeapon", "m_hWeaponWorldModel");
	//offys.m_iClip1 = g_pNetVars->GetOffset("DT_BaseCombatWeapon", "m_iClip1");
	offys.m_nModeIndex = g_pNetVars->GetOffset("DT_BaseViewModel", "m_nModelIndex");
	offys.m_hViewModel = g_pNetVars->GetOffset("DT_CSPlayer", "m_hViewModel[0]");
	offys.m_hWeapon = g_pNetVars->GetOffset("DT_BaseViewModel", "m_hWeapon");


	offys.IDA_FullUpdate = FindPatternIDA(XorStr("engine.dll"), XorStr("A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85"));
}