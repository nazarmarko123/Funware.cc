#pragma once
enum WEAPONCLASS
{
	WEPCLASS_INVALID,
	WEPCLASS_RIFLE,
	WEPCLASS_PISTOL,
	WEPCLASS_SHOTGUN,
	WEPCLASS_SNIPER,
	WEPCLASS_SMG,
	WEPCLASS_MACHINEGUN,
	WEPCLASS_KNIFE,
};
enum ItemDefinitionIndex : int {
	DEAGLE = 1,
	ELITE = 2,
	FIVESEVEN = 3,
	GLOCK = 4,
	AK47 = 7,
	AUG = 8,
	AWP = 9,
	FAMAS = 10,
	G3SG1 = 11,
	GALILAR = 13,
	M249 = 14,
	M4A1 = 16,
	MAC10 = 17,
	P90 = 19,
	UMP45 = 24,
	XM1014 = 25,
	BIZON = 26,
	MAG7 = 27,
	NEGEV = 28,
	SAWEDOFF = 29,
	TEC9 = 30,
	TASER = 31,
	HKP2000 = 32,
	MP7 = 33,
	MP9 = 34,
	NOVA = 35,
	P250 = 36,
	SCAR20 = 38,
	SG556 = 39,
	SSG08 = 40,
	KNIFE = 42,
	FLASHBANG = 43,
	HEGRENADE = 44,
	SMOKEGRENADE = 45,
	MOLOTOV = 46,
	DECOY = 47,
	INCGRENADE = 48,
	C4 = 49,
	KNIFE_T = 59,
	M4A1_SILENCER = 60,
	USP_SILENCER = 61,
	CZ75A = 63,
	REVOLVER = 64,
	KNIFE_BAYONET = 500,
	KNIFE_FLIP = 505,
	KNIFE_GUT = 506,
	KNIFE_KARAMBIT = 507,
	KNIFE_M9_BAYONET = 508,
	KNIFE_TACTICAL = 509,
	KNIFE_FALCHION = 512,
	KNIFE_BUTTERFLY = 515,
	KNIFE_PUSH = 516,
	KNIFE_BOWIE = 514
};
struct CHudTexture
{
	char	szShortName[64];	//0x0000
	char	szTextureFile[64];	//0x0040
	bool	bRenderUsingFont;	//0x0080
	bool	bPrecached;			//0x0081
	char	cCharacterInFont;	//0x0082
	BYTE	pad_0x0083;			//0x0083
	int		hFont;				//0x0084
	int		iTextureId;			//0x0088
	float	afTexCoords[4];		//0x008C
	int		iPosX[4];			//0x009C
}; //Size=0x00AC
class CSWeaponInfo
{
public:
	char _0x0000[20];
	__int32 max_clip;	//0x0014 
	char _0x0018[12];
	__int32 max_reserved_ammo;	//0x0024 
	char _0x0028[96];
	char* hud_name;//0x0088 
	char* weapon_name;	//0x008C 
	char _0x0090[60];
	__int32 type;//0x00CC 
	__int32 price;//0x00D0 
	__int32 reward;//0x00D4 
	char _0x00D8[20];
	BYTE full_auto;//0x00EC 
	char _0x00ED[3];
	__int32 damage;//0x00F0 
	float armor_ratio;	//0x00F4 
	__int32 bullets;	//0x00F8 
	float penetration;	//0x00FC 
	char _0x0100[8];
	float range;//0x0108 
	float range_modifier;//0x010C 
	char _0x0110[16];
	BYTE silencer;//0x0120 
	char _0x0121[15];
	float max_speed;	//0x0130 
	float max_speed_alt;//0x0134 
	char _0x0138[76];
	__int32 recoil_seed;//0x0184 
	char _0x0188[32];
};
class CBomb
{
public:
	float GetC4BlowTime()
	{
		return *reinterpret_cast<float*>((DWORD)this + offys.m_flC4Blow);
	}
	HANDLE GetOwnerEntity()
	{
		return *reinterpret_cast<HANDLE*>((DWORD)this + offys.m_hOwnerEntity);
	}
	bool IsDefused()
	{
		return *reinterpret_cast<bool*>((DWORD)this + offys.m_bBombDefused);
	}
};
class CGrenade
{
public:
	bool IsPinPulled()
	{
		return *reinterpret_cast<bool*>((DWORD)this + offys.m_bPinPulled);
	}
	float GetThrowTime()
	{
		return *reinterpret_cast<float*>((DWORD)this + offys.m_fThrowTime);
	}
};
class CBaseCombatWeapon
{
	template<class T>
	T GetFieldValue(int offset) {
		return *(T*)((DWORD)this + offset);
	}
	template<class T>
	T* GetFieldPointer(int offset) {
		return (T*)((DWORD)this + offset);
	}
public:

	float GetNextPrimaryAttack()
	{
		return *reinterpret_cast<float*>(uintptr_t(this) + offys.m_flNextPrimaryAttack);
	}

	float GetNextSecondaryAttack()
	{
		return *reinterpret_cast<float*>(uintptr_t(this) + offys.m_flNextSecondaryAttack);
	}

	void SetPattern(int skin, int quality, int seed, int stattrak, const char* name)
	{

		*(int*)((DWORD)this + offys.m_nFallbackPaintKit) = skin;
		*(int*)((DWORD)this + offys.m_iEntityQuality) = quality;
		*(int*)((DWORD)this + offys.m_nFallbackSeed) = seed;
		*(int*)((DWORD)this + offys.m_nFallbackStatTrak) = stattrak;
		*(float*)((DWORD)this + offys.m_flFallbackWear) = 0.0001f;

		if (name != "") {
			char* a = (char*)((DWORD)this + offys.m_szCustomName);
			sprintf_s(a, 32, "%s", name);
		}

		*(int*)((DWORD)this + offys.m_iItemIDHigh) = -1;
	}
	void SeName( const char* name)
	{
		if (name != "") {
			char* a = (char*)((DWORD)this + offys.m_szCustomName);
			sprintf_s(a, 32, "%s", name);
		}

		*(int*)((DWORD)this + offys.m_iItemIDHigh) = -1;
	}
	//CBaseViewModel* GetViewModel()
	//{
	//	if (!this)
	//		return nullptr;
		//static std::uintptr_t offset = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("m_hViewModel[0]"));
		//return (CBaseViewModel*)I::ClientCBaseEntityList->GetClientEntityFromHandle(GetFieldValue<CHandle<CBaseViewModel>>(offset));
	//}

	HANDLE m_hWeaponWorldModel()
	{
		return *(HANDLE*)((uintptr_t)this + offys.m_hWeaponWorldModel);
	}

	int* OwnerXuidLow()
	{
		return (int*)((uintptr_t)this + offys.m_OriginalOwnerXuidLow);
	}
	int* OwnerXuidHigh()
	{
		return (int*)((uintptr_t)this + offys.m_OriginalOwnerXuidHigh);
	}

	int* GetEntityQuality() {
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iEntityQuality
		return (int*)((DWORD)this + 0x2F9C);
	}

	int* GetFallbackStatTrak() {
		// DT_BaseAttributableItem -> m_nFallbackStatTrak: 0x314C
		return (int*)((DWORD)this + 0x31B4);
	}
	float* FallbackWear()
	{
		return (float*)((uintptr_t)this + offys.m_flFallbackWear);
	}
	int* GetFallbackSeed() {
		// DT_BaseAttributableItem -> m_nFallbackSeed: 0x3144
		return (int*)((DWORD)this + offys.m_nFallbackSeed);
	}
	inline int* GetFallbackPaintKit() {
		// DT_BaseAttributableItem -> m_nFallbackPaintKit: 0x3140
		return (int*)((DWORD)this + offys.m_nFallbackPaintKit);
	}
	short* fixskins()
	{
		return (short*)((uintptr_t)this + offys.m_iItemDefinitionIndex);
	}
	int* ItemIDHigh()
	{
		return (int*)((uintptr_t)this + 0x2FB0);
	}
	unsigned int* GetAccountID()
	{
		return (unsigned int*)((DWORD)this + 0x2FB8);
	}
	CBaseHandle GetWorldModel() {

		return *(CBaseHandle*)((DWORD)this + offys.m_iWorldModelIndex);  //0x31F4
	}
	void SetModelIndex(int index)
	{
		typedef void(__thiscall* Fn)(void*, int);
		return CallVFunction<Fn>(this, 75)(this, index);
	}
	int* ViewModelIndex()
	{
		return (int*)((uintptr_t)this + offys.m_iViewModelIndex);
	}

	int* WorldModelIndex()
	{
		return (int*)((uintptr_t)this + offys.m_iWorldModelIndex);
	}

	int* ModelIndex()
	{
		return (int*)((uintptr_t)this + offys.m_nModeIndex);
	}

	int* ItemDefinitionIndex()
	{
		return (int*)((uintptr_t)this + 0x1D8);
	}

	model_t* CBaseCombatWeapon::GetModel()
	{
		return *(model_t**)((DWORD)this + 0x6C);
	}
	Vector GetOrigin()
	{
		return *reinterpret_cast<Vector*>((DWORD)this + 0x0134);
	}
	HANDLE GetOwnerCBaseEntity()
	{
		return *reinterpret_cast<HANDLE*>((DWORD)this + 0x0148);
	}
	float NextPrimaryAttack()
	{
		return *reinterpret_cast<float*>((DWORD)this + 0x3208);
	}
	float NextSecondaryAttack()
	{
		return *reinterpret_cast<float*>((DWORD)this + 0x31CC);
	}
	float GetAccuracyPenalty()
	{
		return *reinterpret_cast<float*>((DWORD)this + offys.m_fAccuracyPenalty);
	}
	int Clip1()
	{
		return *reinterpret_cast<int*>((DWORD)this + 0x3234);
	}
	int GetMaxAmmoReserve(void)
	{
		return *(int*)((DWORD)this + offys.m_iPrimaryReserveAmmoCount);
	}

	short WeaponID()
	{
		return *reinterpret_cast<short*>((DWORD)this + 0x2F9A);
		/*typedef int*(__thiscall* GetWeaponNameFn)(void*);
		GetWeaponNameFn Name = (GetWeaponNameFn)((*(PDWORD_PTR*)this)[458]);
		return Name(this);*/
	}
	int GetItemDefinitionIndex(void)
	{
		if (!this) return 0;
		return *(int*)((DWORD)this + offys.m_iItemDefinitionIndex);
	}
	float GetPostponeFireReadyTime()
	{
		return *reinterpret_cast<float*>((DWORD)this + offys.m_flPostponeFireReadyTime);
	}
	int GetZoomLevel()
	{
		return *reinterpret_cast<int*>((DWORD)this + 0x3330);
	}
	const char* GetWeaponName()
	{
		typedef const char*(__thiscall* GetWeaponNameFn)(void*);
		GetWeaponNameFn Name = (GetWeaponNameFn)((*(PDWORD_PTR*)this)[376]);
		return Name(this);
	}
	std::string GetName(bool Ammo)
	{
		const char* name = GetWeaponName();
		std::string Name = name;
		std::string NName;
			NName = Name.substr(7, Name.length() - 7);
	

		if (Ammo && !this->IsMiscWeapon())
		{
			char buffer[32]; sprintf_s(buffer, " [%i]", Clip1());
			NName.append(buffer);
			return NName;
		}
		return NName;
	}
	char* GetGunIcon()
	{
		int WeaponId = this->WeaponID();
		switch (WeaponId)
		{
		case KNIFE:
		case 500:
		case 505:
		case 506:
		case 507:
		case 508:
		case 509:
		case 512:
		case 514:
		case 515:
		case 516:
			return "]";
		case DEAGLE:
			return "A";
		case ELITE:
			return "B";
		case FIVESEVEN:
			return "C";
		case GLOCK:
			return "D";
		case HKP2000:
			return "E";
		case P250:
			return "F";
		case USP_SILENCER:
			return "G";
		case TEC9:
			return "H";
		case CZ75A:
			return "I";
		case REVOLVER:
			return "J";
		case MAC10:
			return "K";
		case UMP45:
			return "L";
		case BIZON:
			return "M";
		case MP7:
			return "N";
		case MP9:
			return "O";
		case P90:
			return "P";
		case GALILAR:
			return "Q";
		case FAMAS:
			return "R";
		case M4A1_SILENCER:
			return "S";
		case M4A1:
			return "T";
		case AUG:
			return "U";
		case SG556:
			return "V";
		case AK47:
			return "W";
		case G3SG1:
			return "X";
		case SCAR20:
			return "Y";
		case AWP:
			return "Z";
		case SSG08:
			return "a";
		case XM1014:
			return "b";
		case SAWEDOFF:
			return "c";
		case MAG7:
			return "d";
		case NOVA:
			return "e";
		case NEGEV:
			return "f";
		case M249:
			return "g";
		case TASER:
			return "h";
		case FLASHBANG:
			return "i";
		case HEGRENADE:
			return "j";
		case SMOKEGRENADE:
			return "k";
		case MOLOTOV:
			return "l";
		case DECOY:
			return "m";
		case INCGRENADE:
			return "n";
		case C4:
			return "o";
		default:
			return " ";
		}
	}
	int GetWeaponType()
	{
		if (!this) return WEPCLASS_INVALID;
		auto id = this->WeaponID();
		switch (id)
		{
		case DEAGLE:
		case ELITE:
		case FIVESEVEN:
		case HKP2000:
		case USP_SILENCER:
		case CZ75A:
		case TEC9:
		case REVOLVER:
		case GLOCK:
		case P250:
			return WEPCLASS_PISTOL;
			break;
		case AK47:
		case M4A1:
		case M4A1_SILENCER:
		case GALILAR:
		case AUG:
		case FAMAS:
		case SG556:
			return WEPCLASS_RIFLE;
			break;
		case P90:
		case BIZON:
		case MP7:
		case MP9:
		case MAC10:
		case UMP45:
			return WEPCLASS_SMG;
			break;
		case AWP:
		case G3SG1:
		case SCAR20:
		case SSG08:
			return WEPCLASS_SNIPER;
			break;
		case NEGEV:
		case M249:
			return WEPCLASS_MACHINEGUN;
			break;
		case MAG7:
		case SAWEDOFF:
		case NOVA:
		case XM1014:
			return WEPCLASS_SHOTGUN;
			break;
		case KNIFE:
		case KNIFE_BAYONET:
		case KNIFE_BUTTERFLY:
		case KNIFE_FALCHION:
		case KNIFE_FLIP:
		case KNIFE_GUT:
		case KNIFE_KARAMBIT:
		case KNIFE_TACTICAL:
		case KNIFE_M9_BAYONET:
		case KNIFE_PUSH:
		case KNIFE_BOWIE:
		case KNIFE_T:
			return WEPCLASS_KNIFE;
			break;

		default:
			return WEPCLASS_INVALID;
		}
	}
	float GetInaccuracy()
	{
		typedef float(__thiscall* GetInaccuracyFn)(void*);
		GetInaccuracyFn Inaccuracy = (GetInaccuracyFn)((*(PDWORD_PTR*)this)[ 467 ]); //459
		return Inaccuracy(this);
	}
	float GetFloatRecoilIndex()
	{
		return *reinterpret_cast<float*>((DWORD)this + 0x32D0);
	}

	float GetSpread()
	{
		typedef float(__thiscall* GetSpreadFn)(void*);
		GetSpreadFn Spread = (GetSpreadFn)((*(PDWORD_PTR*)this)[ 436 ]);//460
		return Spread(this);
	}

	void UpdateAccuracyPenalty()
	{
		typedef void(__thiscall* UpdateAccuracyPenaltyFn)(void*);
		UpdateAccuracyPenaltyFn Update = (UpdateAccuracyPenaltyFn)((*(PDWORD_PTR*)this)[ 468 ]);
		return Update(this);
	}
	bool IsRifle()
	{
		int iWeaponID = WeaponID();
	}
	bool IsScopeable()
	{
		int iWeaponID = WeaponID();
		return (iWeaponID == 38 || iWeaponID == 11 || iWeaponID == 9 || iWeaponID == 40 || iWeaponID == 8 || iWeaponID == SG556);
	}
	bool IsSniper()
	{
		int iWeaponID = WeaponID();
		return (iWeaponID == SSG08 || iWeaponID == AWP || iWeaponID == SCAR20 || iWeaponID == G3SG1);
	}
	bool IsPistol()
	{
		int iWeaponID = WeaponID();
		return (iWeaponID == GLOCK || iWeaponID == HKP2000
			|| iWeaponID == P250 || iWeaponID == DEAGLE
			|| iWeaponID == ELITE || iWeaponID == TEC9 || iWeaponID == USP_SILENCER
			|| iWeaponID == FIVESEVEN);
	}
	bool IsMiscWeapon()
	{
		int iWeaponID = WeaponID();
		return (iWeaponID == KNIFE
			|| iWeaponID == C4
			|| iWeaponID == HEGRENADE || iWeaponID == DECOY
			|| iWeaponID == FLASHBANG || iWeaponID == MOLOTOV
			|| iWeaponID == SMOKEGRENADE || iWeaponID == INCGRENADE || iWeaponID == KNIFE_T
			|| iWeaponID == 500 || iWeaponID == 505 || iWeaponID == 506
			|| iWeaponID == 507 || iWeaponID == 508 || iWeaponID == 509
			|| iWeaponID == 515);
	}
	bool IsGrenade()
	{

		int iWeaponID = WeaponID();
		return (iWeaponID == HEGRENADE || iWeaponID == DECOY
			|| iWeaponID == FLASHBANG || iWeaponID == MOLOTOV
			|| iWeaponID == SMOKEGRENADE || iWeaponID == INCGRENADE);
	}
	bool IsKnife()
	{
		int iWeaponID = WeaponID();
		return (iWeaponID == 42 || iWeaponID == 59 || iWeaponID == 41
			|| iWeaponID == 500 || iWeaponID == 505 || iWeaponID == 506
			|| iWeaponID == 507 || iWeaponID == 508 || iWeaponID == 509
			|| iWeaponID == 515 || iWeaponID == 520 || iWeaponID == 519 || iWeaponID == 523 || iWeaponID == 522 || iWeaponID == 521);
	}
	CSWeaponInfo* GetCSWpnData();
	bool IsReloadingVisually();
};

class CBaseViewModel
{
public:
	CBaseViewModel(void);
	~CBaseViewModel(void);


	bool IsViewable(void) { return false; }

	virtual void					UpdateOnRemove(void);

	virtual void			SendViewModelMatchingSequence(int sequence);
	virtual void			SetWeaponModel(const char *pszModelname, CBaseCombatWeapon *weapon);

	void SendViewModelMatchingSequenceManual(int sequence)
	{
		typedef void(__thiscall* OriginalFn)(void*, int);
		return CallVFunction<OriginalFn>(this, 241)(this, sequence);
	}
	inline int GetModelIndex() {
		return *(int*)((DWORD)this + 0x254);
	}
	inline DWORD GetOwner() {
		return *(PDWORD)((DWORD)this + 0x29BC);
	}
	inline DWORD GetWeapon() {
		return *(PDWORD)((DWORD)this + 0x29B8);
	}
};
enum DataUpdateType_t
{
	DATA_UPDATE_CREATED = 0,	// indicates it was created +and+ entered the pvs
								//	DATA_UPDATE_ENTERED_PVS,
								DATA_UPDATE_DATATABLE_CHANGED,
								//	DATA_UPDATE_LEFT_PVS,
								//	DATA_UPDATE_DESTROYED,		// FIXME: Could enable this, but it's a little worrying
								// since it changes a bunch of existing code
};
class C_BasePlayer
{
public:


	C_BasePlayer();
	virtual			~C_BasePlayer();
	virtual void	Spawn(void);
	virtual void	SharedSpawn(); // Shared between client and server.
	void*		Classify(void) { return 0; }
	// IClientCBaseEntity overrides.
	virtual void	OnPreDataChanged(DataUpdateType_t updateType);
	virtual void	OnDataChanged(DataUpdateType_t updateType);
	virtual void	PreDataUpdate(DataUpdateType_t updateType);
	virtual void	PostDataUpdate(DataUpdateType_t updateType);
	virtual void	ReceiveMessage(int classID, void* &msg);
	virtual void	OnRestore();
	virtual void	MakeTracer(const Vector &vecTracerSrc, const void* &tr, int iTracerType);
	virtual void	GetToolRecordingState(KeyValues *msg);
	void	SetAnimationExtension(const char *pExtension);
	CBaseViewModel			*GetViewModel(int viewmodelindex = 0);

};



class GlowObjectDefinition_t
{
public:
	GlowObjectDefinition_t() { memset(this, 0, sizeof(*this)); }

	class CBaseEntity* m_pEntity;    //0x0000
	Vector m_vGlowColor;           //0x0004
	float   m_flAlpha;                 //0x0010
	uint8_t pad_0014[4];               //0x0014
	float   m_flSomeFloat;             //0x0018
	uint8_t pad_001C[4];               //0x001C
	float   m_flAnotherFloat;          //0x0020
	bool    m_bRenderWhenOccluded;     //0x0024
	bool    m_bRenderWhenUnoccluded;   //0x0025
	bool    m_bFullBloomRender;        //0x0026
	uint8_t pad_0027[5];               //0x0027
	int32_t m_nGlowStyle;              //0x002C
	int32_t m_nSplitScreenSlot;        //0x0030
	int32_t m_nNextFreeSlot;           //0x0034

	bool IsUnused() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

	static const int END_OF_FREE_LIST = -1;
	static const int ENTRY_IN_USE = -2;
}; 



class CGlowObjectManager
{
public:

	int RegisterGlowObject(CBaseEntity *pEntity, const Vector &vGlowColor, float flGlowAlpha, bool bRenderWhenOccluded, bool bRenderWhenUnoccluded, int nSplitScreenSlot)
	{
		int nIndex;
		if (m_nFirstFreeSlot == GlowObjectDefinition_t::END_OF_FREE_LIST)
		{
			nIndex = m_GlowObjectDefinitions.AddToTail();
		}
		else
		{
			nIndex = m_nFirstFreeSlot;
			m_nFirstFreeSlot = m_GlowObjectDefinitions[nIndex].m_nNextFreeSlot;
		}

		m_GlowObjectDefinitions[nIndex].m_hEntity = pEntity;
		m_GlowObjectDefinitions[nIndex].m_vGlowColor = vGlowColor;
		m_GlowObjectDefinitions[nIndex].m_flGlowAlpha = flGlowAlpha;
		m_GlowObjectDefinitions[nIndex].flUnk = 0.0f;
		m_GlowObjectDefinitions[nIndex].m_flBloomAmount = 1.0f;
		m_GlowObjectDefinitions[nIndex].localplayeriszeropoint3 = 0.0f;
		m_GlowObjectDefinitions[nIndex].m_bRenderWhenOccluded = bRenderWhenOccluded;
		m_GlowObjectDefinitions[nIndex].m_bRenderWhenUnoccluded = bRenderWhenUnoccluded;
		m_GlowObjectDefinitions[nIndex].m_bFullBloomRender = false;
		m_GlowObjectDefinitions[nIndex].m_nFullBloomStencilTestValue = 0;
		m_GlowObjectDefinitions[nIndex].m_nSplitScreenSlot = nSplitScreenSlot;
		m_GlowObjectDefinitions[nIndex].m_nNextFreeSlot = GlowObjectDefinition_t::ENTRY_IN_USE;

		return nIndex;
	}

	void UnregisterGlowObject(int nGlowObjectHandle)
	{
		Assert(!m_GlowObjectDefinitions[nGlowObjectHandle].IsUnused());

		m_GlowObjectDefinitions[nGlowObjectHandle].m_nNextFreeSlot = m_nFirstFreeSlot;
		m_GlowObjectDefinitions[nGlowObjectHandle].m_hEntity = NULL;
		m_nFirstFreeSlot = nGlowObjectHandle;
	}

	class GlowObjectDefinition_t
	{
	public:
		void set(float r, float g, float b, float a)
		{
			m_vGlowColor = Vector(r, g, b);
			m_flGlowAlpha = a;
			m_bRenderWhenOccluded = true;
			m_bRenderWhenUnoccluded = false;
			m_flBloomAmount = 1.0f;
		}

		CBaseEntity* getEnt()
		{
			return m_hEntity;
		}

		bool IsUnused() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

	public:
		CBaseEntity*    m_hEntity;
		Vector            m_vGlowColor;
		float            m_flGlowAlpha;

		char            unknown[4]; //pad 
		float            flUnk; //confirmed to be treated as a float while reversing glow functions 
		float            m_flBloomAmount;
		float            localplayeriszeropoint3;


		bool            m_bRenderWhenOccluded;
		bool            m_bRenderWhenUnoccluded;
		bool            m_bFullBloomRender;
		char            unknown1[1]; //pad 


		int                m_nFullBloomStencilTestValue; // 0x28 
		int                iUnk; //appears like it needs to be zero  
		int                m_nSplitScreenSlot; //Should be -1 

											   // Linked list of free slots 
		int                m_nNextFreeSlot;


		// Special values for GlowObjectDefinition_t::m_nNextFreeSlot 
		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;

	};

	CUtlVector< GlowObjectDefinition_t > m_GlowObjectDefinitions;
	int m_nFirstFreeSlot;
};
