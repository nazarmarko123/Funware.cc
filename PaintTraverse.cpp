#include "hooks.h"
#include "GameUtils.h"
#include "Draw.h"
#include "Menu.h"
#include "ESP.h"
#include "Misc.h"
#include "global.h"
#include "SpoofedConvar.h"
#include "Hitmarker.h"
#include "GrenadePrediction.h"
#include "global.h"
#include "Antiaim.h"
#include "aimware.h"
#include "EventLog/NewEventLog.h"
bool first_frame_passed = false;
std::string sPanel = XorStr("FocusOverlayPanel");

static DWORD* deathNotice;
static void(__thiscall *ClearDeathNotices)(DWORD);


template<class T>
static T* Find_Hud_Element(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(FindPatternIDA("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 85 C0 0F 84 ? ? ? ? 8D 58") + 1);
	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(FindPatternIDA("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39"));
	return (T*)find_hud_element(pThis, name);
}












void __fastcall Hooks::PaintTraverse(void* ecx, void* edx, unsigned int vgui_panel, bool force_repaint, bool allow_force)
{
	if (Clientvariables->Visuals.EspEnable && Clientvariables->Visuals.Noscope && strcmp("HudZoom", g_pPanel->GetName(vgui_panel)) == 0)
		return;



	panelVMT->GetOriginalMethod<PaintTraverseFn>(41)(ecx, vgui_panel, force_repaint, allow_force);

	const char* pszPanelName = g_pPanel->GetName(vgui_panel);

	if (!strstr(pszPanelName, sPanel.data()))
		return;


	g_pPanel->SetMouseInputEnabled(vgui_panel, G::Opened);

	int cur_height, cur_width;
	g_pEngine->GetScreenSize(cur_width, cur_height);

	if (!first_frame_passed || cur_width != G::Screen.width || cur_height != G::Screen.height) {
		first_frame_passed = true;
		g_Draw.Init();

		g_pEngine->GetScreenSize(cur_width, cur_height);
		G::Screen.height = cur_height;
		G::Screen.width = cur_width;
	}

	


	if (Clientvariables->Visuals.eventlog)
		FEATURES::MISC::in_game_logger.Do();

	if (g_pEngine->IsInGame() && g_pEngine->IsConnected() && G::LocalPlayer) {

		if (G::LocalPlayer)
		{
			if (!deathNotice)
				deathNotice = Find_Hud_Element<DWORD>("CCSGO_HudDeathNotice");

			if (deathNotice)
			{
				float* localDeathNotice = (float*)((DWORD)deathNotice + 0x50);

				if (localDeathNotice) *localDeathNotice = Clientvariables->Visuals.PreserveKillfeed ? FLT_MAX : 1.5f;

				if (0 && deathNotice - 20)
				{
					if (!ClearDeathNotices) ClearDeathNotices = (void(__thiscall*)(DWORD))FindPatternIDA("client_panorama.dll", "55 8B EC 83 EC 0C 53 56 8B 71 58");
					if (ClearDeathNotices) ClearDeathNotices(((DWORD)deathNotice - 20));
				}
			}
		}



		if (Clientvariables->Visuals.OutOfPOVArrows)
		{
			if (G::LocalPlayer && G::UserCmd)
			{
				for (int i = 1; i <= g_pEntitylist->GetHighestEntityIndex(); i++)
				{
					CBaseEntity* pBaseEntity = g_pEntitylist->GetClientEntity(i);

					if (!pBaseEntity)
						continue;

					g_ESP->DoEnemyCircle(G::LocalPlayer, pBaseEntity->GetAbsOrigin(), &G::UserCmd->viewangles.y);
				}

			}
		}
	

		

		g_Draw.StringA(g_Draw.font_build, false, 900, 1, 0, 255, 0, 255, "%s", "FunWare [recode]");
		g_Draw.StringA(g_Draw.font_build, false, 1, 1, 255, 255, 255, 255, "%s", "by FUNIKY");
		float real;
		if (!G::SendPacket)
		{
			real = G::UserCmd->viewangles.y;
		}

		if (Clientvariables->Antiaim.Freestanding != 0) {
			if (G::RealAngle.y == G::LocalPlayer->LowerBodyYaw())
				g_Draw.StringA(g_Draw.font_build, false, 10, G::Screen.height - 90, 188, 34, 0, 255, "%s", "LBY");
			else
				g_Draw.StringA(g_Draw.font_build, false, 10, G::Screen.height - 90, 100, 255, 0, 255, "%s", "LBY");
			//	else (G::LocalPlayer->GetVelocity().Length() > 51 && (G::LocalPlayer->LowerBodyYaw() - real) > 30);
			//		g_Draw.StringA(g_Draw.font_build, false, 10, G::Screen.height - 100, 0, 255, 0, 255, "%s", "lby");
			if (Clientvariables->Antiaim.AntiaimEnable > 0 && !(*G::LocalPlayer->GetFlags() & FL_ONGROUND))
			{
				if (!G::SendPacket)

					g_Draw.StringA(g_Draw.font_build, false, 10, G::Screen.height - 125, 188, 34, 0, 255, "%s", "LC");
				else
					g_Draw.StringA(g_Draw.font_build, false, 10, G::Screen.height - 125, 100, 255, 0, 255, "%s", "LC");

			}


		}



		int w, h;
		int centerW, centerh, topH;
		g_pEngine->GetScreenSize(w, h);
		centerW = w / 2;
		centerh = h / 2;

		if (Clientvariables->Antiaim.Freestanding == 2 && G::LocalPlayer->isAlive()) {
			int Right, Left, Back;
			Right = Clientvariables->Misc.Rightside;
			Left = Clientvariables->Misc.Leftside;
			Back = Clientvariables->Misc.Backside;

			static bool side1 = false;
			static bool side2 = false;
			static bool inside1 = false;
			static bool inside2 = false;
			static bool back1 = false;
			static bool back2 = false;

			if (GetAsyncKeyState(Right)) {
				side1 = true; //v
				side2 = false;//v
				inside1 = true; //f
				inside2 = false; //f
				back1 = false; //v
				back2 = true;//f
			}
			if (GetAsyncKeyState(Left)) {
				side1 = false; //v
				side2 = true; //v
				inside1 = false; //f
				inside2 = true; //f
				back1 = false;
				back2 = true;
			}
			if (GetAsyncKeyState(Back)) {
				side1 = false;
				side2 = false;
				inside1 = true;
				inside2 = true;
				back1 = true;
				back2 = false;
			}

			//if (side1) //v
			//	g_Draw.StringA(g_Draw.font_build, false, centerW + 50, centerh - 18, 0, 255, 0, 255, "%s", "->");
			//if (inside1) //f
			//	g_Draw.StringA(g_Draw.font_build, false, centerW - 80, centerh - 18, 255, 255, 255, 255, "%s", "<-");
			//if (side2) //v
			//	g_Draw.StringA(g_Draw.font_build, false, centerW - 80, centerh - 18, 0, 255, 0, 255, "%s", "<-");
			//if (inside2)//f
			//	g_Draw.StringA(g_Draw.font_build, false, centerW + 50, centerh - 18, 255, 255, 255, 255, "%s", "->");
			//if (back1) //v
			//	g_Draw.StringA(g_Draw.font_build, false, centerW - 10, centerh + 45, 0, 255, 0, 255, "%s", "V");
			//if (back2) //f
			//	g_Draw.StringA(g_Draw.font_build, false, centerW - 10, centerh + 45, 255, 255, 255, 255, "%s", "V");
		}

		if (Clientvariables->Antiaim.Freestanding == 3 && G::LocalPlayer->isAlive()) {
			int Right, Left, Back;
			Right = Clientvariables->Misc.Rightside;
			Left = Clientvariables->Misc.Leftside;
			Back = Clientvariables->Misc.Backside;

			static bool side1 = false;
			static bool side2 = false;
			static bool inside1 = false;
			static bool inside2 = false;
			static bool back1 = false;
			static bool back2 = false;

			if (GetAsyncKeyState(Right)) {
				side1 = true; //v
				side2 = false;//v
				inside1 = true; //f
				inside2 = false; //f
				back1 = false; //v
				back2 = true;//f
			}
			if (GetAsyncKeyState(Left)) {
				side1 = false; //v
				side2 = true; //v
				inside1 = false; //f
				inside2 = true; //f
				back1 = false;
				back2 = true;
			}
			if (GetAsyncKeyState(Back)) {
				side1 = false;
				side2 = false;
				inside1 = true;
				inside2 = true;
				back1 = true;
				back2 = false;
			}

			//if (side1) //v
			//	g_Draw.StringA(g_Draw.font_build, false, centerW + 50, centerh - 18, 0, 255, 0, 255, "%s", "->");
			//if (inside1) //f
			//	g_Draw.StringA(g_Draw.font_build, false, centerW - 80, centerh - 18, 255, 255, 255, 255, "%s", "<-");
			//if (side2) //v
			//	g_Draw.StringA(g_Draw.font_build, false, centerW - 80, centerh - 18, 0, 255, 0, 255, "%s", "<-");
			//if (inside2)//f
			//	g_Draw.StringA(g_Draw.font_build, false, centerW + 50, centerh - 18, 255, 255, 255, 255, "%s", "->");
			//if (back1) //v
			//	g_Draw.StringA(g_Draw.font_build, false, centerW - 10, centerh + 45, 0, 255, 0, 255, "%s", "V");
			//if (back2) //f
			//	g_Draw.StringA(g_Draw.font_build, false, centerW - 10, centerh + 45, 255, 255, 255, 255, "%s", "V");
		}

		if (Clientvariables->Antiaim.Freestanding == 4 && G::LocalPlayer->isAlive()) {
			int Right, Left, Back;
			Right = Clientvariables->Misc.Rightside;
			Left = Clientvariables->Misc.Leftside;
			Back = Clientvariables->Misc.Backside;

			static bool side1 = false;
			static bool side2 = false;
			static bool inside1 = false;
			static bool inside2 = false;
			static bool back1 = false;
			static bool back2 = false;

			if (GetAsyncKeyState(Right)) {
				side1 = true; //vw
				side2 = false;//v
				inside1 = true; //f
				inside2 = false; //f
				back1 = false; //v
				back2 = true;//f
			}
			if (GetAsyncKeyState(Left)) {
				side1 = false; //v
				side2 = true; //v
				inside1 = false; //f
				inside2 = true; //f
				back1 = false;
				back2 = true;
			}
			if (GetAsyncKeyState(Back)) {
				side1 = false;
				side2 = false;
				inside1 = true;
				inside2 = true;
				back1 = true;
				back2 = false;
			}

			//if (side1) //v
			//	g_Draw.StringA(g_Draw.font_build, false, centerW + 50, centerh - 18, 0, 255, 0, 255, "%s", "->");
			//if (inside1) //f
			//	g_Draw.StringA(g_Draw.font_build, false, centerW - 80, centerh - 18, 255, 255, 255, 255, "%s", "<-");
			//if (side2) //v
			//	g_Draw.StringA(g_Draw.font_build, false, centerW - 80, centerh - 18, 0, 255, 0, 255, "%s", "<-");
			//if (inside2)//f
			//	g_Draw.StringA(g_Draw.font_build, false, centerW + 50, centerh - 18, 255, 255, 255, 255, "%s", "->");
			//if (back1) //v
			//	g_Draw.StringA(g_Draw.font_build, false, centerW - 10, centerh + 45, 0, 255, 0, 255, "%s", "V");
			//if (back2) //f
			//	g_Draw.StringA(g_Draw.font_build, false, centerW - 10, centerh + 45, 255, 255, 255, 255, "%s", "V");
		}

		if (Clientvariables->Visuals.DamageIndicators) {
			G::CurrTime = g_pGlobals->interval_per_tick * (G::LocalPlayer->GetTickBase() + 1);

			if (!G::DamageHit.empty()) {
				for (auto it = G::DamageHit.begin(); it != G::DamageHit.end();) {
					if (G::CurrTime > it->ExpireTime) {
						it = G::DamageHit.erase(it);
						continue;
					}
					it->Draw();
					++it;
				}
			}
		}



		static bool bSpoofed;

		if (Clientvariables->Misc.TPKey > 0 && !bSpoofed) {
			ConVar* svcheats = g_pCvar->FindVar("sv_cheats");
			SpoofedConvar* svcheatsspoof = new SpoofedConvar(svcheats);
			svcheatsspoof->SetInt(1);
			bSpoofed = true;
		}

		if (Clientvariables->Visuals.RemoveParticles) {
			auto postprocessing = g_pCvar->FindVar("mat_postprocess_enable");
			auto postprocessingspoof = new SpoofedConvar(postprocessing);
			postprocessingspoof->SetInt(0);
		}



		aw_hitmarker->paint();// oMg YoU WiLl nEveR find ouT HoW to FiX HAHAHHA

		if (Clientvariables->Visuals.Noscope) {
			g_ESP->DrawScope(G::LocalPlayer);
			auto postprocessing = g_pCvar->FindVar("mat_postprocess_enable");
			auto postprocessingspoof = new SpoofedConvar(postprocessing);
			postprocessingspoof->SetInt(0);
		}

		g_ESP->Loop();
		grenade_prediction::instance().Paint();
	}
}






















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































