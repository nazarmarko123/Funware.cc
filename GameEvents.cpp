#include "hooks.h"
#include <time.h>
#include "Mmsystem.h"
#include <thread>
#include "Hitmarker.h"
#include "global.h"
#include "Menu.h"
#include "controls.h"
#include "aimware.h"
#include "Skinchanger/NewSkinchanger.h"
#pragma comment(lib, "winmm.lib")

#define EVENT_HOOK(x)
#define TICK_INTERVAL (Globals->interval_per_tick)
#define TIME_TO_TICKS(dt) ((int)(0.5f + (float)(dt) / TICK_INTERVAL))

cGameEvent g_Event;
std::vector<trace_info> trace_logs;




const char* GetKillIconWeapon(int si)
{
	
	switch (si)
	{
	case 0:
		return "bayonet";
	case 1:
		return "knife_flip";
	case 2:
		return "knife_gut";
	case 3:
		return "knife_karambit";
	case 4:
		return "knife_m9_bayonet";
	case 5:
		return "knife_tactical";
	case 6:
		return "knife_butterfly";
	case 7:
		return "knife_falchion";
	case 8:
		return "knife_push";
	case 9:
		return "knife_survival_bowie";
	case 10:
		return "knife_gypsy_jackknife";
	case 11:
		return "knife_stiletto";
	case 12:
		return "knife_ursus";
	case 13:
		return "knife_widowmaker";
	default:
		NULL;
	}
}





char* HitgroupToName(int hitgroup)
{
	switch (hitgroup)
	{
	case HITGROUP_HEAD:
		return "head";
	case HITGROUP_LEFTLEG:
		return "left leg";
	case HITGROUP_RIGHTLEG:
		return "right leg";
	case HITGROUP_LEFT_FOOT:
		return "left foot";
	case HITGROUP_RIGHT_FOOT:
		return "right foot";
	case HITGROUP_STOMACH:
		return "stomach";
	case HITGROUP_PELVIS:
		return "pelvis";
	default:
		return "body";
	}
}
float logged_hits[64];
int GetServerSideID(int CBaseEntityIndex)
{
	player_info_t pInfo;
	g_pEngine->GetPlayerInfo(CBaseEntityIndex, &pInfo);
	return pInfo.m_nUserID;
}
std::map<int, ResolveData> PlayerData;
void cGameEvent::FireGameEvent(IGameEvent* event)
{

	const char* szEventName = event->GetName();
	if (!szEventName)
		return;

	if (!strcmp(szEventName, "round_start"))
		G::NewRound = true;
	else
		G::NewRound = false;

	if (strcmp(szEventName, "player_connect") == 0) {
		memset(&G::Shots, 0, sizeof(G::Shots));
	}




	if (strcmp(szEventName, "weapon_fire") == 0) {
		CBaseEntity* player = g_pEntitylist->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("userID")));
		if (player == G::LocalPlayer) {
			if (G::Target != NULL) {
				G::Shots[G::Target->GetIndex()]++;
			}
		}
	}
	//dont work xd
	{
		/*
		if (strcmp(event->GetName(), "player_death") == 0)
	{
		int attacker = g_pEngine->GetPlayerForUserID(event->GetInt("attacker"));
		int iLocalPlayer = g_pEngine->GetLocalPlayer();
		if (attacker == iLocalPlayer)
		{
			short cw = SafeWeaponID();

			if (Clientvariables->NewSkinchanger.enabled && (cw == 59 || cw == 500 || cw == 42 || cw == 507 || cw == 506 || cw == 508 || cw == 509 || cw == 515 || cw == 516 || cw == 505 || cw == 512 || cw == 523 || cw == 519 || cw == 521 || cw == 520 || cw == 522))
			{
				if (GetKillIconWeapon(Clientvariables->NewSkinchanger.Knife) != "")
					event->SetString("weapon", GetKillIconWeapon(Clientvariables->NewSkinchanger.Knife));
			}
		}

	}
		
		*/
	}
	



	if (Clientvariables->Misc.buybot) {
		switch (Clientvariables->Misc.buybot2) {
		case 1:
			if (strcmp(event->GetName(), "round_start") == 0) {
				g_pEngine->ClientCmd_Unrestricted("buy ak47; buy m4a1;");
			}
			break;
		case 2: {
			if (strcmp(event->GetName(), "round_start") == 0)
				g_pEngine->ClientCmd_Unrestricted("buy ssg08;");
		} break;
		case 3: {
			if (strcmp(event->GetName(), "round_start") == 0)
				g_pEngine->ClientCmd_Unrestricted("buy scar20; buy g3sg1;");
		} break;
		}

		if (Clientvariables->Misc.buybot3) {

			if (strcmp(event->GetName(), "round_start") == 0)
				g_pEngine->ClientCmd_Unrestricted("buy smokegrenade; buy hegrenade; buy molotov; buy incgrenade; buy flashbang; buy vest; buy vesthelm;");
		}
	}

	if (Clientvariables->Visuals.DamageIndicators) {
		if (strcmp(szEventName, "player_hurt") == 0) {
			auto attacker = event->GetInt("attacker");
			auto dmgdone = event->GetInt("dmg_health");
			auto Hurt = event->GetInt("userid");
			CBaseEntity* pEnt = g_pEntitylist->GetClientEntity(g_pEngine->GetPlayerForUserID(Hurt));
			if (g_pEngine->GetPlayerForUserID(attacker) == g_pEngine->GetLocalPlayer()) {
				G::DamageDealt = dmgdone;
				G::DamageHit.push_back(FloatingText(pEnt, 1.f, dmgdone));
			}
		}
	}

	


	if (strcmp(szEventName, "player_hurt") == 0) {
		CBaseEntity* hurt = g_pEntitylist->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt(XorStr("userID"))));
		CBaseEntity* attacker = g_pEntitylist->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt(XorStr("attacker"))));

		if (attacker == G::LocalPlayer) {
			if (hurt == G::Target) {
				if (G::Target != NULL) {
					G::Shots[G::Target->GetIndex()] -= 1;
				}
			}
		}

		bool IsHeadshot = (event->GetInt(XorStr("hitgroup")) == 1);

		int damage = event->GetInt("dmg_health");

		int CBaseEntityServerID = event->GetInt(XorStr("userID"));
		int AttackerServerID = event->GetInt(XorStr("attacker"));
		int AttackerIndex = g_pEngine->GetPlayerForUserID(AttackerServerID);
		if (AttackerIndex != g_pEngine->GetLocalPlayer())
			return;
		int CBaseEntityIndex = g_pEngine->GetPlayerForUserID(CBaseEntityServerID);
		if (CBaseEntityIndex == g_pEngine->GetLocalPlayer())
			return;

		auto* entity = g_pEntitylist->GetClientEntity(CBaseEntityIndex);

		if (!entity)
			return;

		g_Hitmarker->update_end_time();
		g_Hitmarker->play_sound();

		player_info_t pinfo;

		g_pEngine->GetPlayerInfo(CBaseEntityIndex, &pinfo);
		//g_pCvar->(&Color(200, 255, 0, 255), "[hit] ");
		//Msg(XorStr("FunWare hit %s in the %s for %d damage (%d health remaining) \n"), pinfo.m_szPlayerName, HitgroupToName(event->GetInt("hitgroup")), event->GetInt("dmg_health"), event->GetInt("health")), (Color(200, 255, 0, 255), "hitgroup");
		//	Msg(XorStr("[cute dog hook] -%d in %s to %s\n"), event->GetInt("dmg_health"), HitgroupToName(event->GetInt("hitgroup")), pinfo.m_szPlayerName);
	}


	if (strcmp(szEventName, "player_death") == 0) {
		int CBaseEntityServerID = event->GetInt(XorStr("userID"));
		int AttackerServerID = event->GetInt(XorStr("attacker"));

		int AttackerIndex = g_pEngine->GetPlayerForUserID(AttackerServerID);
		if (AttackerIndex != g_pEngine->GetLocalPlayer())
			return;
		int CBaseEntityIndex = g_pEngine->GetPlayerForUserID(CBaseEntityServerID);
		if (CBaseEntityIndex == g_pEngine->GetLocalPlayer())
			return;

		player_info_t pinfo;
		g_pEngine->GetPlayerInfo(CBaseEntityIndex, &pinfo);
		//Msg(XorStr("FunWare killed %s %s\n"), pinfo.m_szPlayerName, event->GetBool("headshot") ? XorStr("(HEAD) ") : XorStr(" "));
	}
	

	Vector velocity = G::vecUnpredictedVel;


	if (strcmp(szEventName, "item_purchase") == 0) {
		int CBaseEntityServerID = event->GetInt(XorStr("userID"));
		int CBaseEntityIndex = g_pEngine->GetPlayerForUserID(CBaseEntityServerID);

		player_info_t pinfo;
		g_pEngine->GetPlayerInfo(CBaseEntityIndex, &pinfo);
		//	Msg("[cute dog hook]\n");
		//Msg(XorStr("FunWare %s bought %s \n"), pinfo.m_szPlayerName, event->GetString("weapon"));
	}

	if (Clientvariables->Visuals.Hitmarker) {
		if (strcmp(szEventName, "player_hurt") == 0) {
			int Attacker = event->GetInt("attacker");
			int UserID = event->GetInt("userid");

			CBaseEntity* Entity = g_pEntitylist->GetClientEntity(g_pEngine->GetPlayerForUserID(UserID));

			if (g_pEngine->GetPlayerForUserID(Attacker) == g_pEngine->GetLocalPlayer()) {

			}
		}
	}

	if (strcmp(szEventName, "bullet_impact") == 0 && Clientvariables->Visuals.Bullet_impacts) {
		auto* index = g_pEntitylist->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("userid")));

		Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));
		float x = event->GetFloat("x"), y = event->GetFloat("y"), z = event->GetFloat("z");
		Vector vec_hitpos = Vector(x, y, z);
		float Red = Clientvariables->Colors.bullet_impacts[0] * 255;
		float Green = Clientvariables->Colors.bullet_impacts[1] * 255;
		float Blue = Clientvariables->Colors.bullet_impacts[2] * 255;

		if (index)
			trace_logs.push_back(trace_info(index->GetEyePosition(), position, g_pGlobals->curtime, event->GetInt("userid")));

		g_pDebugOverlay->AddBoxOverlay(vec_hitpos = Vector(x, y, z), Vector(-2, -2, -2), Vector(2, 2, 2), QAngle(0, 0, 0), Red, Green, Blue, 130, 1.f);
	}
}

int cGameEvent::GetEventDebugID()
{
	return 42;
}

void cGameEvent::RegisterSelf()
{
	g_pGameEventManager->AddListener(this, "player_connect", false);
	g_pGameEventManager->AddListener(this, "player_hurt", false);
	g_pGameEventManager->AddListener(this, "round_start", false);
	g_pGameEventManager->AddListener(this, "round_end", false);
	g_pGameEventManager->AddListener(this, "player_death", false);
	g_pGameEventManager->AddListener(this, "weapon_fire", false);
	g_pGameEventManager->AddListener(this, "bullet_impact", false);
}

void cGameEvent::Register()
{
	EVENT_HOOK(FireEvent);
}
