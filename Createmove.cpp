#include "hooks.h"
#include "global.h"
#include "Misc.h"
#include "Menu.h"
#include "BacktrackingHelper.h"
#include "Math.h"
#include "GameUtils.h"
#include "Aimbot.h"
#include "PredictionSystem.h"
#include "Antiaim.h"
#include "GrenadePrediction.h"
#include "Correction.h"
#include "Hitmarker.h"
#include "Resolver2.h"
static CPredictionSystem* Prediction = new CPredictionSystem();

void __declspec(naked) LagFix()
{
	__asm {
		push edi
		xchg dword ptr[esp], edi
		push eax
		mov eax, 77
		mov eax, dword ptr[esp]
		add esp, 4
		pop edi

		cmp esp, 0
		jne fixentity

		_emit 0x88
		_emit 0xFF

		invlpg dword ptr[eax]

		int 2

		fixentity:
		sub esp, 4
			mov dword ptr[esp], ebp

			call cleanup

			pop ebp

			ret

			cleanup :

		ret
	}
}

static int Ticks = 0;
static int LastReserve = 0;

void setclantag(const char* tag)
{
	static auto pSetClanTag = (int(__fastcall*)(const char*, const char*))FindPatternIDA("engine.dll", "53 56 57 8B DA 8B F9 FF 15");

	pSetClanTag(tag, "gamesense");
}


bool __fastcall Hooks::CreateMove(void* thisptr, void*, float flInputSampleTime, CUserCmd* cmd)
{




	if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
	{


		if (Clientvariables->Misc.clantag)
			switch (int(g_pGlobals->curtime * 10) % 27)
			{
			case 0: setclantag(XorStr("")); break;
			case 1: setclantag(XorStr("")); break;
			case 2: setclantag(XorStr("")); break;
			case 3: setclantag(XorStr("")); break;
			case 4: setclantag(XorStr("")); break;
			case 5: setclantag(XorStr("")); break;
			case 6: setclantag(XorStr("")); break;
			case 7: setclantag(XorStr("")); break;
			case 8: setclantag(XorStr("")); break;
			case 9: setclantag(XorStr("")); break;
			case 10:setclantag(XorStr("")); break;
			case 11:setclantag(XorStr("")); break;
			case 12:setclantag(XorStr("")); break;
			case 13:setclantag(XorStr("")); break;
			case 14:setclantag(XorStr("")); break;
			case 15:setclantag(XorStr("")); break;
			case 16:setclantag(XorStr("")); break;
			case 17:setclantag(XorStr("")); break;
			case 18:setclantag(XorStr("")); break;
			case 19:setclantag(XorStr("")); break;
			case 20:setclantag(XorStr("")); break;
			case 22:setclantag(XorStr(" ")); break;
			case 23:setclantag(XorStr(" ")); break;
			case 24:setclantag(XorStr("")); break;
			case 25:setclantag(XorStr("")); break;
			case 26:setclantag(XorStr("")); break;
			case 27:setclantag(XorStr("")); break;

			}

		if (cmd) {

			if (!cmd->command_number)
				return true;

			g_pEngine->SetViewAngles(cmd->viewangles);
			QAngle org_view = cmd->viewangles;

			CBaseEntity* pLocalPlayer = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
			if (pLocalPlayer) {
				G::LocalPlayer = pLocalPlayer;
				G::UserCmd = cmd;
				G::UserCmdForBacktracking = cmd;

				if (pLocalPlayer->GetHealth() > 0) {
					CBaseCombatWeapon* pWeapon = pLocalPlayer->GetWeapon();
					if (pWeapon) {
						PVOID pebp;
						__asm mov pebp, ebp;
						bool* pbSendPacket = (bool*)(*(PDWORD)pebp - 0x1C);
						bool& bSendPacket = *pbSendPacket;

						G::vecUnpredictedVel = G::LocalPlayer->GetVelocity();

						G::MainWeapon = pWeapon;
						G::WeaponData = pWeapon->GetCSWpnData();

						bool IsLadder = pLocalPlayer->GetMoveType() == MOVETYPE_LADDER;
						G::StrafeAngle = G::UserCmd->viewangles;


						//xd  weapons

						{
							short cw = SafeWeaponID();
							if (cw == ItemDefinitionIndex::AK47)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.AK47hit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.AK47dmg;
							}

							if (cw == ItemDefinitionIndex::AUG)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.AUGhit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.AUGdmg;
							}

							if (cw == ItemDefinitionIndex::AWP)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.AWPhit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.AWPdmg;
							}

							if (cw == ItemDefinitionIndex::BIZON)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Bizonhit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Bizondmg;
							}

							if (cw == ItemDefinitionIndex::CZ75A)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Cz75hit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Cz75hit;
							}
							if (cw == ItemDefinitionIndex::DEAGLE)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Deaglehit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Deagledmg;
							}
							if (cw == ItemDefinitionIndex::ELITE)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Dualhit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Dualdmg;
							}

							if (cw == ItemDefinitionIndex::FAMAS)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.FAMAShit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.FAMASdmg;
							}

							if (cw == ItemDefinitionIndex::FIVESEVEN)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Fivehit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Fivedmg;
							}
							if (cw == ItemDefinitionIndex::G3SG1)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.G3sg1hit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.G3sg1dmg;
							}
							if (cw == ItemDefinitionIndex::GALILAR)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Galilhit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Galildmg;
							}
							if (cw == ItemDefinitionIndex::GLOCK)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Glockhit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Glockdmg;
							}

							if (cw == ItemDefinitionIndex::HKP2000)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.P2000hit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.P2000dmg;
							}
							if (cw == ItemDefinitionIndex::M249)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.M249hit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.M249dmg;
							}

							if (cw == ItemDefinitionIndex::M4A1)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.M4A4hit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.M4A4dmg;
							}
							if (cw == ItemDefinitionIndex::M4A1_SILENCER)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.M4A1Shit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.M4A1Sdmg;
							}
							if (cw == ItemDefinitionIndex::XM1014)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Xmhit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Xmdmg;
							}

							if (cw == ItemDefinitionIndex::USP_SILENCER)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.USPhit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.USPdmg;
							}
							if (cw == ItemDefinitionIndex::UMP45)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.UMP45hit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.UMP45dmg;
							}

							if (cw == ItemDefinitionIndex::TEC9)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.tec9hit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.tec9dmg;
							}

							if (cw == ItemDefinitionIndex::SSG08)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.SSG08hit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.SSG08dmg;
							}
							if (cw == ItemDefinitionIndex::SG556)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Sg553hit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Sg553dmg;
							}

							if (cw == ItemDefinitionIndex::SCAR20)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.SCAR20hit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.SCAR20dmg;
							}

							if (cw == ItemDefinitionIndex::SAWEDOFF)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Sawedhit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Saweddmg;
							}
							if (cw == ItemDefinitionIndex::REVOLVER)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Revolverhit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Revolverdmg;
							}


							if (cw == ItemDefinitionIndex::P90)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.P90hit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.P90dmg;
							}
							if (cw == ItemDefinitionIndex::P250)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.P250hit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.P250dmg;
							}


							if (cw == ItemDefinitionIndex::NOVA)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Novahit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Novadmg;
							}


							if (cw == ItemDefinitionIndex::NEGEV)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Negevhit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Negevdmg;
							}

							if (cw == ItemDefinitionIndex::MP9)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Mp9hit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Mp9dmg;
							}


							if (cw == ItemDefinitionIndex::MP7)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Mp7hit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Mp7dmg;
							}



							if (cw == ItemDefinitionIndex::MAC10)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Mac10hit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Mac10dmg;
							}

							if (cw == ItemDefinitionIndex::MAG7)
							{
								Clientvariables->Ragebot.Minhitchance = Clientvariables->Weaponxdfix.Maghit;
								Clientvariables->Ragebot.Mindamage = Clientvariables->Weaponxdfix.Magdmg;
							}


						}

						

						if (Clientvariables->Misc.lagfix)
							LagFix();
						g_Aimbot->DropTarget();

						if (Clientvariables->Misc.Walkbot)
							g_Misc->WalkBotCM(org_view);

						g_Misc->Bunnyhop();
						g_Misc->AutoStrafe();

						if ((G::UserCmd->buttons & IN_ATTACK || (G::UserCmd->buttons & IN_ATTACK2 && (G::MainWeapon->WeaponID() == REVOLVER || G::MainWeapon->IsKnife()))) && GameUtils::IsAbleToShoot())
							g_Aimbot->fired_in_that_tick = true;

						Prediction->EnginePrediction(cmd);

						g_Aimbot->Run(pLocalPlayer, pWeapon, cmd);
						g_Aimbot->CompensateInaccuracies();

						if (Clientvariables->Ragebot.perfectresolver)
							set->DoCM();

						if (Clientvariables->Misc.FakelagEnable)
							g_Misc->FakeLag();

						G::ForceRealAA = false;
						if (G::ChokedPackets >= 14)
						{
							G::SendPacket = true;
							G::ChokedPackets = 0;
							G::ForceRealAA = true;
						}



						FEATURES::RAGEBOT::antiaim.UpdateLBY();
						FEATURES::RAGEBOT::antiaim.Do(cmd);
						/*
						if (!IsLadder)
							g_Antiaim->Run(org_view);
							*/
						if ((G::UserCmd->buttons & IN_ATTACK || G::UserCmd->buttons & IN_ATTACK2 && G::MainWeapon->WeaponID() == REVOLVER) && (G::MainWeapon->IsPistol() || G::MainWeapon->WeaponID() == AWP || G::MainWeapon->WeaponID() == SSG08))
						{
							static bool bFlip = false;
							if (bFlip)
							{
								if (G::MainWeapon->WeaponID() == REVOLVER)
								{

								}
								else
									G::UserCmd->buttons &= ~IN_ATTACK;
							}
							bFlip = !bFlip;
						}

						if (G::SendPacket) {
							if (Clientvariables->Misc.TPangles == 0)
								G::AAAngle = G::UserCmd->viewangles;

							G::ChokedPackets = 0;
							G::FakeAngle = G::UserCmd->viewangles;
						}
						else {
							if (Clientvariables->Misc.TPangles == 1 && !g_Aimbot->aimbotted_in_current_tick)
								G::AAAngle = G::UserCmd->viewangles;

							G::ChokedPackets++;
							G::RealAngle = G::UserCmd->viewangles;
						}
						if (Clientvariables->Misc.TPangles == 2)
							G::AAAngle = QAngle(G::UserCmd->viewangles.x, G::LocalPlayer->LowerBodyYaw());

						g_Misc->FixMovement();
						g_Misc->FixCmd();

						cmd = G::UserCmd;
						bSendPacket = G::SendPacket;
						grenade_prediction::instance().Tick(G::UserCmd->buttons);
					}
				}
			}
			else {
				G::LocalPlayer = nullptr;
			}
		}
	}
	return false;
}

























































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































