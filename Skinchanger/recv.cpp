




#include "../SDK.h"
#include <unordered_map>
#include "NewSkinChanger.h"
#include "recv.h"



typedef void(*RecvVarProxyFn)(const CRecvProxyData* pData, void* pStruct, void* pOut);


using namespace std;
// Helper function to generate a random sequence number.
inline int RandomSequence(int low, int high) {
	return (rand() % (high - low + 1) + low);
}



#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);



RecvVarProxyFn fnSequenceProxyFn = nullptr;

RecvVarProxyFn oRecvnModelIndex;
void Hooked_RecvProxy_Viewmodel(CRecvProxyData *pData, void *pStruct, void *pOut)
{
	// Get the knife view model id's
	int default_t = g_pModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	int default_ct = g_pModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	int iBayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	int iButterfly = g_pModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	int iFlip = g_pModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
	int iGut = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
	int iKarambit = g_pModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	int iM9Bayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	int iHuntsman = g_pModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	int iFalchion = g_pModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	int iDagger = g_pModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
	int iBowie = g_pModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
	int iGunGame = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

	int Navaja = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
	int Stiletto = g_pModelInfo->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
	int Ursus = g_pModelInfo->GetModelIndex("models/weapons/v_knife_ursus.mdl");
	int Talon = g_pModelInfo->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");

	// Get local player (just to stop replacing spectators knifes)
	auto pLocal = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (Clientvariables->NewSkinchanger.enabled && pLocal && !Clientvariables->RankChanger.semiinventar)
	{
		// If we are alive and holding a default knife(if we already have a knife don't worry about changing)
		if (pLocal->isAlive() && (
			pData->m_Value.m_Int == default_t ||
			pData->m_Value.m_Int == default_ct ||
			pData->m_Value.m_Int == iBayonet ||
			pData->m_Value.m_Int == iFlip ||
			pData->m_Value.m_Int == iGunGame ||
			pData->m_Value.m_Int == iGut ||
			pData->m_Value.m_Int == iKarambit ||
			pData->m_Value.m_Int == iM9Bayonet ||
			pData->m_Value.m_Int == iHuntsman ||
			pData->m_Value.m_Int == iBowie ||
			pData->m_Value.m_Int == iButterfly ||
			pData->m_Value.m_Int == iFalchion ||
			pData->m_Value.m_Int == iDagger ||
			pData->m_Value.m_Int == Navaja ||
			pData->m_Value.m_Int == Stiletto ||
			pData->m_Value.m_Int == Ursus ||
			pData->m_Value.m_Int == Talon))
		{
			// Set whatever knife we want
			if (Clientvariables->NewSkinchanger.Knife == 0)
				pData->m_Value.m_Int = iBayonet;
			else if (Clientvariables->NewSkinchanger.Knife == 9)
				pData->m_Value.m_Int = iBowie;
			else if (Clientvariables->NewSkinchanger.Knife == 6)
				pData->m_Value.m_Int = iButterfly;
			else if (Clientvariables->NewSkinchanger.Knife == 7)
				pData->m_Value.m_Int = iFalchion;
			else if (Clientvariables->NewSkinchanger.Knife == 1)
				pData->m_Value.m_Int = iFlip;
			else if (Clientvariables->NewSkinchanger.Knife == 2)
				pData->m_Value.m_Int = iGut;
			else if (Clientvariables->NewSkinchanger.Knife == 5)
				pData->m_Value.m_Int = iHuntsman;
			else if (Clientvariables->NewSkinchanger.Knife == 3)
				pData->m_Value.m_Int = iKarambit;
			else if (Clientvariables->NewSkinchanger.Knife == 4)
				pData->m_Value.m_Int = iM9Bayonet;
			else if (Clientvariables->NewSkinchanger.Knife == 8)
				pData->m_Value.m_Int = iDagger;
			else if (Clientvariables->NewSkinchanger.Knife == 10)
				pData->m_Value.m_Int = Navaja;
			else if (Clientvariables->NewSkinchanger.Knife == 11)
				pData->m_Value.m_Int = Stiletto;
			else if (Clientvariables->NewSkinchanger.Knife == 12)
				pData->m_Value.m_Int = Ursus;
			else if (Clientvariables->NewSkinchanger.Knife == 13)
				pData->m_Value.m_Int = Talon;
		}
	}
	if (Clientvariables->RankChanger.semiinventar)
	{
		// If we are alive and holding a default knife(if we already have a knife don't worry about changing)
		if (pLocal->isAlive() && (
			pData->m_Value.m_Int == default_t ||
			pData->m_Value.m_Int == default_ct ||
			pData->m_Value.m_Int == iBayonet ||
			pData->m_Value.m_Int == iFlip ||
			pData->m_Value.m_Int == iGunGame ||
			pData->m_Value.m_Int == iGut ||
			pData->m_Value.m_Int == iKarambit ||
			pData->m_Value.m_Int == iM9Bayonet ||
			pData->m_Value.m_Int == iHuntsman ||
			pData->m_Value.m_Int == iBowie ||
			pData->m_Value.m_Int == iButterfly ||
			pData->m_Value.m_Int == iFalchion ||
			pData->m_Value.m_Int == iDagger ||
			pData->m_Value.m_Int == Navaja ||
			pData->m_Value.m_Int == Stiletto ||
			pData->m_Value.m_Int == Ursus ||
			pData->m_Value.m_Int == Talon))
		{
			// Set whatever knife we want
			pData->m_Value.m_Int = iM9Bayonet;
		
		}
	}
	// Carry on the to original proxy
	oRecvnModelIndex(pData, pStruct, pOut);
}





void SetViewModelSequence2(const CRecvProxyData *pDataConst, void *pStruct, void *pOut)
{

	// Make the incoming data editable.
	auto pData = const_cast<CRecvProxyData*>(pDataConst);

	// Confirm that we are replacing our view model and not someone elses.
	auto pViewModel = static_cast<CBaseViewModel*>(pStruct);

	if (pViewModel)
	{
		auto pOwner = (g_pEntitylist->GetClientEntity(pViewModel->GetOwner() & 0xFFF));

		// Compare the owner entity of this view model to the local player entity.
		if (pOwner && pOwner->GetIndex() == g_pEngine->GetLocalPlayer())
		{
			// Get the filename of the current view model.
			void* pModel = g_pModelInfo->GetModelvoid(pViewModel->GetModelIndex());
			std::string szModel = g_pModelInfo->GetModelNamevoid(pModel);

			// Store the current sequence.
			int m_nSequence = pData->m_Value.m_Int;
			if (szModel == "models/weapons/v_knife_butterfly.mdl")
			{
				// Fix animations for the Butterfly Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
				}
			}
			else if (szModel == "models/weapons/v_knife_falchion_advanced.mdl")
			{
				// Fix animations for the Falchion Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
					break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (szModel == "models/weapons/v_knife_push.mdl")
			{
				// Fix animations for the Shadow Daggers.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
					break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
					break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (szModel == "models/weapons/v_knife_survival_bowie.mdl")
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1;
					break;
				default:
					m_nSequence--;
				}
			}
			else if (szModel == "models/weapons/v_knife_ursus.mdl")
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (szModel == "models/weapons/v_knife_stiletto.mdl")
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(12, 13);
					break;
				}
			}
			else if (szModel == "models/weapons/v_knife_widowmaker.mdl")
			{
				// Fix animations for the Bowie Knife.
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(14, 15);
					break;
				}
			}
			pData->m_Value.m_Int = m_nSequence;
		}
	}
	fnSequenceProxyFn(pData, pStruct, pOut);
}


void AnimationFixHook()
{
	for (ClientClass* pClass = g_pClient->GetAllClasses(); pClass; pClass = pClass->m_pNext) {
		if (!strcmp(pClass->m_pNetworkName, "CBaseViewModel")) {
			// Search for the 'm_nModelIndex' property.
			RecvTable* pClassTable = pClass->m_pRecvTable;

			for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++) {
				RecvProp* pProp = &pClassTable->m_pProps[nIndex];

				if (!pProp || strcmp(pProp->m_pVarName, "m_nSequence"))
					continue;

				// Store the original proxy function.
				fnSequenceProxyFn = static_cast<RecvVarProxyFn>(pProp->m_ProxyFn);

				// Replace the proxy function with our sequence changer.
				pProp->m_ProxyFn = static_cast<RecvVarProxyFn>(SetViewModelSequence2);

				break;
			}

			break;
		}
	}
}

void NetvarHook()
{
	AnimationFixHook();
	ClientClass *pClass = g_pClient->GetAllClasses();
	while (pClass)
	{
		const char *pszName = pClass->m_pRecvTable->m_pNetTableName;
		if (!strcmp(pszName, "DT_SmokeGrenadeProjectile"))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;
				if (!strcmp(name, "m_bDidSmokeEffect"))
				{
					//fnNoSmoke = (RecvVarProxyFn)pProp->m_ProxyFn;
					//pProp->m_ProxyFn = NoSmoke;
				}

			}
		}
		else if (!strcmp(pszName, "DT_BaseViewModel"))
		{
			for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
			{
				RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;
				if (!strcmp(name, "m_nModelIndex"))
				{
					oRecvnModelIndex = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = Hooked_RecvProxy_Viewmodel;
				}
			}
		}
		pClass = pClass->m_pNext;
	}
}






