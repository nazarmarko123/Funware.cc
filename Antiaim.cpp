#include "sdk.h"
#include "Antiaim.h"
#include "global.h"
#include "GameUtils.h"
#include "Math.h"
#include "Aimbot.h"
#include "Autowall.h"

CAntiaim* g_Antiaim = new CAntiaim;

float get3ddist2(Vector myCoords, Vector enemyCoords)
{
	return sqrt(
		pow(double(enemyCoords.x - myCoords.x), 2.0) + pow(double(enemyCoords.y - myCoords.y), 2.0) + pow(double(enemyCoords.z - myCoords.z), 2.0));
}

bool ShouldPredict()
{
	INetChannelInfo* nci = g_pEngine->GetNetChannelInfo();

	float server_time = g_pGlobals->curtime + nci->GetLatency(FLOW_OUTGOING);

	if (g_pGlobals->frametime != 0.0 | G::UserCmd->hasbeenpredicted)
		server_time++;

	static bool initialized;
	bool will_update = false;

	if (!initialized && !G::LocalPlayer->IsMoving())
	{
		initialized = true;
		G::PredictedTime = server_time + 0.22f;
	}
	else if (G::LocalPlayer->IsMoving())
	{
		initialized = false;
	}

	if (server_time >= (G::PredictedTime) && *G::LocalPlayer->GetFlags() & FL_ONGROUND)
	{
		G::PredictedTime = server_time + 1.1f;
		will_update = true;
	}
	return will_update;
}


void AimAtPlayers()
{
	int value = G::UserCmd->viewangles.y, num = 0;
	float best_dist = G::MainWeapon->GetCSWpnData()->range;
	for (int i = 1; i <= g_pGlobals->maxClients; ++i)
	{
		if (i == g_pEngine->GetLocalPlayer())
			continue;

		CBaseEntity* pTarget = g_pEntitylist->GetClientEntity(i);
		Vector pos;
		if (!pTarget || pTarget->GetHealth() < 1)
			continue;
		if (G::LocalPlayer->GetTeamNum() != pTarget->GetTeamNum()) {
			pos = pTarget->GetEyePosition();
			float dist = get3ddist2(pos, G::LocalPlayer->GetEyePosition());
			if (Clientvariables->Antiaim.AtPlayer == 1) {
				if (dist >= best_dist)
					continue;
			}
			best_dist = dist;

			QAngle angle = GameUtils::CalculateAngle(G::LocalPlayer->GetEyePosition(), pos);
			angle.y = Math::NormalizeYaw(angle.y);
			Math::ClampAngles(angle);

			if (Clientvariables->Antiaim.AtPlayer == 1)
				value = angle.y;
			else {
				num++;
				value += angle.y;
			}
		}
	}
	if (Clientvariables->Antiaim.AtPlayer == 2 && num != 0)
		value /= num;

	G::UserCmd->viewangles.y = value;
}

void CAntiaim::Pitch_AA()
{
	float temp = 0;
	int velocity = G::LocalPlayer->GetVelocity().Length();
	int LowerbodyDelta = Clientvariables->Antiaim.FreestandingDelta;

	switch (Clientvariables->Antiaim.Pitch) {
	case 1:
		temp = 88.f;
		break;
	case 2:
		temp -= 88.9f;
		break;
	case 3:
		temp = 0.f;
		break;
	case 4:
		temp = 0.f + Math::RandomFloat2(75.f, 85.f);
		break;
	case 5:
	{
		if (ShouldPredict())
			temp -= 88.999f;
		else
		{
			temp = 86.f;
		}
	}
	break;
	case 6:
		temp = 1080.f;
		break;
	case 7:
		temp = 1089.f;
		break;

	default:
		break;
	}
	G::UserCmd->viewangles.x = temp;
}

float BestHeadDirection(float yaw)
{
	for (int i = 0; i < g_pEngine->GetMaxClients(); i++) {
		CBaseEntity* player = g_pEntitylist->GetClientEntity(i);

		if (player->IsValidTarget()) {
			QAngle CalculatedAngle = GameUtils::CalculateAngle(G::LocalPlayer->GetEyePosition(), player->GetOrigin());
			float Atangle = CalculatedAngle.y;

			Vector Difference = G::LocalPlayer->GetOrigin() - player->GetOrigin();

			if (Difference.y > 0)
				return (Atangle - 85.f);
			else if (Difference.y < 0)
				return (Atangle + 85.f);
		}
	}
	return 0;
}

float GetBestHeadAngle(float yaw)
{
	float Back, Right, Left;

	Vector src3D, dst3D, forward, right, up, src, dst;
	trace_t tr;
	Ray_t ray, ray2, ray3, ray4, ray5;
	CTraceFilter filter;

	QAngle engineViewAngles;
	g_pEngine->GetViewAngles(engineViewAngles);

	engineViewAngles.x = 0;




	Math::AngleVectors(engineViewAngles, &forward, &right, &up);

	filter.pSkip = G::LocalPlayer;
	src3D = G::LocalPlayer->GetEyePosition();
	dst3D = src3D + (forward * 384);

	ray.Init(src3D, dst3D);

	g_pEngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	Back = (tr.endpos - tr.start).Length();

	ray2.Init(src3D + right * 35, dst3D + right * 35);

	g_pEngineTrace->TraceRay(ray2, MASK_SHOT, &filter, &tr);

	Right = (tr.endpos - tr.start).Length();

	ray3.Init(src3D - right * 35, dst3D - right * 35);

	g_pEngineTrace->TraceRay(ray3, MASK_SHOT, &filter, &tr);

	Left = (tr.endpos - tr.start).Length();

	if (Left > Right) {
		return (yaw - 85);
	}
	else if (Right > Left) {
		return (yaw + 85);
	}
	else if (Back > Right || Back > Left) {
		return (yaw - 177);
	}
	return 0;
}

void BreakLowerbodyFreestand()
{
	QAngle Angles;
	g_pEngine->GetViewAngles(Angles);
	float BestHeadPosition = GetBestHeadAngle(Angles.y);

	int LowerbodyDelta = Clientvariables->Antiaim.FreestandingDelta;

	if (G::SendPacket)
	{
		G::UserCmd->viewangles.y = BestHeadPosition + LowerbodyDelta + Math::RandomFloat2(-65.f, 65.f); // 6
	}
	else
	{
		if (ShouldPredict())
			G::UserCmd->viewangles.y = BestHeadPosition + LowerbodyDelta;
		else
			G::UserCmd->viewangles.y = BestHeadPosition;
	}
}

void Senpaii_AA()
{
	INetChannelInfo* nci = g_pEngine->GetNetChannelInfo();
	float server_time = g_pGlobals->curtime + nci->GetLatency(FLOW_OUTGOING);

	if (G::LocalPlayer->GetVelocity().Length() > 0.1f && !G::LocalPlayer->GetFlags() & FL_ONGROUND)
	{
		G::UserCmd->viewangles.y + Math::RandomFloat2(-160.0f, 160.0f);
	}
	else if (!G::LocalPlayer->GetFlags() & FL_DUCKING)
	{
		G::UserCmd->viewangles.y = -(float)(fmod(server_time / 1.5f * 170.0f, -170.0f));
	}
	else if (G::LocalPlayer->GetShotsFired() > 1)
	{
		G::UserCmd->viewangles.y + Math::RandomFloat2(-170.0f, 170.0f);
	}
	else
	{
		G::UserCmd->viewangles.y + Math::RandomFloat2(-90.0f, 90.0f);
	}

}


void Manual_AA()
{
	int LowerbodyDelta, Right, Left, Back;

	LowerbodyDelta = Clientvariables->Antiaim.FreestandingDelta;
	Right = Clientvariables->Misc.Rightside;	Left = Clientvariables->Misc.Leftside;	Back = Clientvariables->Misc.Backside;

	static bool side1 = false;	static bool side2 = false;	static bool back = false;

	if (GetKeyState(VK_XBUTTON2)) {
		side1 = true;	side2 = false;	back = false;
	}
	if (GetKeyState(VK_XBUTTON1)) {
		side1 = false;	side2 = true;	back = false;
	}
	if (GetAsyncKeyState(Back)) {
		side1 = false;	side2 = false;	back = true;
	}

	if (side1)
		if (G::SendPacket) {
			G::UserCmd->viewangles.y = LowerbodyDelta + Math::RandomFloat2(-55.f, 55.f);
		}
		else {
			if (ShouldPredict())
				G::UserCmd->viewangles.y -= 90 + LowerbodyDelta;
			else
				G::UserCmd->viewangles.y -= 90;
		}
		if (side2)
			if (G::SendPacket) {
				G::UserCmd->viewangles.y = LowerbodyDelta + Math::RandomFloat2(-55.f, 55.f);
			}
			else {
				if (ShouldPredict())
					G::UserCmd->viewangles.y += 90 + LowerbodyDelta;
				else
					G::UserCmd->viewangles.y += 90;
			}

			if (back)
				if (G::SendPacket) {
					G::UserCmd->viewangles.y = LowerbodyDelta + Math::RandomFloat2(-55.f, 55.f);
				}
				else {
					if (ShouldPredict())
						G::UserCmd->viewangles.y -= 180 + LowerbodyDelta;
					else
						G::UserCmd->viewangles.y -= 180;
				}
}


void Jitter_AAKey()
{
	int LowerbodyDelta, Right, Left, Back;

	LowerbodyDelta = Clientvariables->Antiaim.FreestandingDelta;
	Right = Clientvariables->Misc.Rightside;
	Left = Clientvariables->Misc.Leftside;
	Back = Clientvariables->Misc.Backside;

	static bool side1 = false;
	static bool side2 = false;
	static bool back = false;

	if (GetKeyState(VK_XBUTTON2)) {
		side1 = true;
		side2 = false;
		back = false;
	}
	if (GetKeyState(VK_XBUTTON1)) {
		side1 = false;
		side2 = true;
		back = false;
	}
	if (GetAsyncKeyState(Back)) {
		side1 = false;
		side2 = false;
		back = true;
	}

	if (side1)
		if (G::SendPacket) {
			G::UserCmd->viewangles.y = LowerbodyDelta + Math::RandomFloat2(-6.f, 6.f);
		}
		else {
			if (ShouldPredict())
				G::UserCmd->viewangles.y -= 90 + LowerbodyDelta;
			else
				G::UserCmd->viewangles.y -= 90 + Math::RandomFloat2(-2.f, 2.f);
		}


		if (side2)
			if (G::SendPacket) {
				G::UserCmd->viewangles.y = LowerbodyDelta + Math::RandomFloat2(-6.f, 6.f);
			}
			else {
				if (ShouldPredict())
					G::UserCmd->viewangles.y += 90 + LowerbodyDelta;
				else
					G::UserCmd->viewangles.y += 90 + Math::RandomFloat2(-2.f, 2.f);
			}

			if (back)
				if (G::SendPacket) {
					G::UserCmd->viewangles.y = LowerbodyDelta + Math::RandomFloat2(-6.f, 6.f);
				}
				else {
					if (ShouldPredict())
						G::UserCmd->viewangles.y -= 180 + LowerbodyDelta;
					else
						G::UserCmd->viewangles.y -= 180 + Math::RandomFloat2(-2.f, 2.f);
				}
}

void CAntiaim::AntiAimYaw_Real()
{
	if (G::LocalPlayer->GetVelocity().Length() > 0.1f) {
		float temp = G::UserCmd->viewangles.y;

		static float add = 0;
		static bool reset = true;
		static int Ticks = 0;
		float temp_base = temp;

		switch (Clientvariables->Antiaim.YawRunning) {
		case 1:
			temp -= 180.f;
			break;

		case 2:
			temp -= 180.f + Math::RandomFloat2(-25.f, 25.f);
			break;

		case 3:
			if (reset) {
				add = 0;
				reset = false;
			}

			temp += 135;
			temp += add;
			add += 15;

			if (temp_base + 225 < temp) {
				reset = true;
				temp = temp_base + 225;
			}
			break;

		case 4:
			temp -= Ticks;
			Ticks += 3;

			if (Ticks > 235)
				Ticks = 125;
			break;

		case 5:
			temp -= Ticks;
			Ticks += 5;

			if (Ticks > 360)
				Ticks = 0;
			break;

		case 6:
			temp -= G::LocalPlayer->LowerBodyYaw() + Math::RandomFloat2(-72.f, 72.f);
			break;

		default:
			break;
		}
		G::UserCmd->viewangles.y = temp + Clientvariables->Antiaim.YawRunningAdd;
	}
	else 
	{
		float temp = G::UserCmd->viewangles.y;

		static float add = 0;
		static bool reset = true;
		static int Ticks = 120;
		float temp_base = temp;

		switch (Clientvariables->Antiaim.Yaw) 
		{
		case 1:
			temp -= 180.f;
			break;

		case 2:
			temp -= 180.f + Math::RandomFloat2(-25.f, 25.f);
			break;

		case 3:
			if (reset) {
				add = 0;
				reset = false;
			}

			temp += 135;
			temp += add;
			add += 15;

			if (temp_base + 225 < temp) {
				reset = true;
				temp = temp_base + 225;
			}
			break;

		case 4:
			temp -= Ticks;
			Ticks += 3;

			if (Ticks > 235)
				Ticks = 125;
			break;

		case 5:
			temp -= Ticks;
			Ticks += 5;

			if (Ticks > 360)
				Ticks = 0;
			break;

		case 6:
			temp -= G::LocalPlayer->LowerBodyYaw() + Math::RandomFloat2(-25.f, 25.f);
			break;

	

		default:
			break;
		}

		G::UserCmd->viewangles.y = temp + Clientvariables->Antiaim.YawAdd;
	}


	if (!G::LocalPlayer->GetFlags() & FL_ONGROUND)
	{

		float temp = G::UserCmd->viewangles.y;

		switch (Clientvariables->Antiaim.Air)
		{
		case 1:
			if (!G::LocalPlayer->GetFlags() & FL_ONGROUND)
			{


				static int Ticks;

				G::UserCmd->viewangles.x = 89;
				G::UserCmd->viewangles.y += Ticks;
				Ticks += 3;
				if (Ticks > 0)
					Ticks = 180;

				return;

			}
			break;
		}

		G::UserCmd->viewangles.y = temp + Clientvariables->Antiaim.AirAdd;
	}

}

void CAntiaim::AntiAimYaw_Fake()
{
	float temp = G::UserCmd->viewangles.y;
	static int Ticks;
	static int Side;

	switch (Clientvariables->Antiaim.FakeYaw) {
	case 1:
		temp -= 180.f;  // static
		break;

	case 2:
		temp -= G::RealAngle.y - G::RealAngle.y; // opposite
		break;

	case 3:
		temp -= 180.f + Math::RandomFloat2(-70.f, 70.f); // jitter //45
		break;

	case 4:
		temp -= Ticks; // spin
		Ticks += 2;

		if (Ticks > 360)
			Ticks = 0;
		break;

	case 5:
		temp -= Ticks;   // 180 tick
		Ticks += 3;

		if (Ticks > 235)
			Ticks = 125;
		break;
	case 6:
		temp = G::LocalPlayer->LowerBodyYaw() + Math::RandomFloat(-55.f, 55.f); //recode aa
		break;
	case 7:
		temp = G::LocalPlayer->LowerBodyYaw() + Math::RandomFloat2(-120.f, 120.f); // lowerbody
		break;
	case 8:
		temp = (G::LocalPlayer->LowerBodyYaw() + 140 + rand() % 123) - rand() % 20; // 180 treehouse
		break;
	case 9:
		Senpaii_AA(); //Senpaii aa
		break;

	default:
		break;
	}
	G::UserCmd->viewangles.y = temp + Clientvariables->Antiaim.FakeYawAdd;
}

bool GetAlive(CBaseEntity* pLocal)
{
	for (int i = 1; i < 64; ++i) {
		if (i == g_pEngine->GetLocalPlayer())
			continue;

		CBaseEntity* target = g_pEntitylist->GetClientEntity(i);
		player_info_t info;
		Vector pos;
		if (!target || target->GetHealth() < 1)
			continue;
		if (pLocal->GetTeamNum() != target->GetTeamNum()) {
			return true;
		}
	}
	return false;
}

void CAntiaim::Run(QAngle org_view)
{
	if (Clientvariables->Antiaim.AntiaimEnable) {
		static int iChokedPackets = -1;

		if ((g_Aimbot->fired_in_that_tick && iChokedPackets < 4 && GameUtils::IsAbleToShoot()) && !G::ForceRealAA) {
			G::SendPacket = false;
			iChokedPackets++;
		}
		else {
			iChokedPackets = 0;

			CGrenade* pCSGrenade = (CGrenade*)G::LocalPlayer->GetWeapon();

			if (G::UserCmd->buttons & IN_USE
				|| !GetAlive(G::LocalPlayer) && !Clientvariables->Misc.AntiUT
				|| G::LocalPlayer->GetMoveType() == MOVETYPE_LADDER && G::LocalPlayer->GetVelocity().Length() > 0
				|| G::LocalPlayer->GetMoveType() == MOVETYPE_NOCLIP
				|| pCSGrenade && pCSGrenade->GetThrowTime() > 0.f)
				return;

			choke = !choke;
			if (!Clientvariables->Misc.FakelagEnable || (*G::LocalPlayer->GetFlags() & FL_ONGROUND && !Clientvariables->Misc.FakelagOnground || *G::LocalPlayer->GetFlags() & FL_ONGROUND && G::LocalPlayer->GetVelocity().Length() < 0.1f))
				G::SendPacket = choke;

			G::UserCmd->viewangles = org_view;

			if (Clientvariables->Antiaim.AtPlayer > 0)
				AimAtPlayers();

			Fakewalk(G::UserCmd);

			if (G::bShouldChoke)
				G::SendPacket = G::bShouldChoke = false;

			if (!G::SendPacket)
				G::nChokedTicks++;
			else
				G::nChokedTicks = 0;

			if (Clientvariables->Antiaim.Freestanding == 1 && G::LocalPlayer->GetVelocity().Length() <= 100.f && *G::LocalPlayer->GetFlags() & FL_ONGROUND)
			{
				Pitch_AA();
				BreakLowerbodyFreestand();

				if (G::SendPacket)
					AntiAimYaw_Fake();
			}
			else if (Clientvariables->Antiaim.Freestanding == 2 && G::LocalPlayer->GetVelocity().Length() < 60000.f)
			{
				Pitch_AA();
				Manual_AA();

				if (G::SendPacket)
					AntiAimYaw_Fake();
			}
			else if (Clientvariables->Antiaim.Freestanding == 3 && G::LocalPlayer->GetVelocity().Length() < 60000.f)
			{
				Pitch_AA();
				Jitter_AAKey();

				if (G::SendPacket)
					AntiAimYaw_Fake();
			}
			else {
				Pitch_AA();
				if (G::SendPacket)
					AntiAimYaw_Fake();
				else {
					AntiAimYaw_Real();
				}
			}
		}
	}
}

void Accelerate(CBaseEntity* player, Vector& wishdir, float wishspeed, float accel, Vector& outVel)
{
	float currentspeed = outVel.Dot(wishdir);

	float addspeed = wishspeed - currentspeed;

	if (addspeed <= 0)
		return;

	float accelspeed = accel * g_pGlobals->frametime * wishspeed * player->m_surfaceFriction();

	if (accelspeed > addspeed)
		accelspeed = addspeed;

	for (int i = 0; i < 3; i++)
		outVel[i] += accelspeed * wishdir[i];
}

void WalkMove(CBaseEntity* player, Vector& outVel)
{
	Vector forward, right, up, wishvel, wishdir, dest;
	float_t fmove, smove, wishspeed;

	Math::AngleVectors(player->GetEyeAngles(), forward, right, up);

	g_pMoveHelper->SetHost(player);
	fmove = g_pMoveHelper->m_flForwardMove;
	smove = g_pMoveHelper->m_flSideMove;
	g_pMoveHelper->SetHost((CBaseEntity*)nullptr);

	if (forward[2] != 0) {
		forward[2] = 0;
		Math::NormalizeVector(forward);
	}

	if (right[2] != 0) {
		right[2] = 0;
		Math::NormalizeVector(right);
	}

	for (int i = 0; i < 2; i++)
		wishvel[i] = forward[i] * fmove + right[i] * smove;

	wishvel[2] = 0;

	wishdir = wishvel;
	wishspeed = wishdir.Normalize();

	g_pMoveHelper->SetHost(player);
	if ((wishspeed != 0.0f) && (wishspeed > g_pMoveHelper->m_flMaxSpeed)) {
		VectorMultiply(wishvel, player->m_flMaxspeed() / wishspeed, wishvel);
		wishspeed = player->m_flMaxspeed();
	}
	g_pMoveHelper->SetHost(nullptr);
	outVel[2] = 0;
	Accelerate(player, wishdir, wishspeed, g_pCvar->FindVar(XorStr("sv_accelerate"))->GetFloat(), outVel);
	outVel[2] = 0;

	VectorAdd(outVel, player->GetBaseVelocity(), outVel);

	float spd = outVel.Length();

	if (spd < 1.0f) {
		outVel.Init();
		VectorSubtract(outVel, player->GetBaseVelocity(), outVel);
		return;
	}

	g_pMoveHelper->SetHost(player);
	g_pMoveHelper->m_outWishVel += wishdir * wishspeed;
	g_pMoveHelper->SetHost(nullptr);

	if (!(*player->GetFlags() & FL_ONGROUND)) {
		VectorSubtract(outVel, player->GetBaseVelocity(), outVel);
		return;
	}

	VectorSubtract(outVel, player->GetBaseVelocity(), outVel);
}

void CAntiaim::Fakewalk(CUserCmd* userCMD)
{
	if (!Clientvariables->Misc.FakewalkEnable || !GetAsyncKeyState(Clientvariables->Misc.FakewalkKey))
		return;
	Vector velocity = G::vecUnpredictedVel;

	int Iterations = 0;
	for (; Iterations < 15; ++Iterations) {
		if (velocity.Length() < Clientvariables->Misc.fakewalk_speed)
		{
			//g_pCvar->ConsolePrintf("Ticks till stop %d\n", Iterations);
			Msg("Ticks till stop %d\n", Iterations);
			break;
		}

		Friction(velocity);
		WalkMove(G::LocalPlayer, velocity);
	}

	int choked_ticks = G::nChokedTicks;

	if (Iterations > 7 - choked_ticks || !choked_ticks) // 7
	{
		float_t speed = velocity.Length();

		QAngle direction;
		Math::VectorAngles(velocity, direction);

		direction.y = userCMD->viewangles.y - direction.y;

		Vector forward;
		Math::AngleVectors(direction, forward);
		Vector negated_direction = forward * -speed;

		static int choked = 0;
		choked = choked > 7 ? 0 : choked + 1; // choked 7
		userCMD->forwardmove = choked < 2 || choked > 5 ? 0 : userCMD->forwardmove;
		userCMD->sidemove = choked < 2 || choked > 5 ? 0 : userCMD->sidemove;
		G::SendPacket = choked < 1; // 1
	}

	if (G::nChokedTicks < 4)
		G::bShouldChoke = true;
}


void CAntiaim::Friction(Vector& outVel)
{
	float speed, newspeed, control;
	float friction;
	float drop;

	speed = outVel.Length();

	if (speed <= 0.1f)
		return;

	drop = 0;

	if (*G::LocalPlayer->GetFlags() & FL_ONGROUND) {
		friction = g_pCvar->FindVar(XorStr("sv_friction"))->GetFloat() * G::LocalPlayer->m_surfaceFriction();

		control = (speed < g_pCvar->FindVar(XorStr("sv_stopspeed"))->GetFloat()) ? g_pCvar->FindVar(XorStr("sv_stopspeed"))->GetFloat() : speed;

		drop += control * friction * g_pGlobals->frametime;
	}

	newspeed = speed - drop;
	if (newspeed < 0)
		newspeed = 0;

	if (newspeed != speed) {

		newspeed /= speed;

		VectorMultiply(outVel, newspeed, outVel);
	}
}


bool CanShoot(bool right_click)
{
	auto local_player = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0 || local_player->GetNextAttack() > GetCurtime())
		return false;

	auto weapon = reinterpret_cast<CBaseCombatWeapon*>(g_pEntitylist->GetClientEntity(local_player->GetActiveWeaponIndex()));
	if (!weapon)
		return false;


	if (right_click)
	{
		if (weapon->GetNextSecondaryAttack() > GetCurtime())
			return false;
	}
	else
	{
		if (weapon->GetNextPrimaryAttack() > GetCurtime())
			return false;

		/// revolver
		if (weapon->GetItemDefinitionIndex() == ItemDefinitionIndex::REVOLVER && weapon->GetPostponeFireReadyTime() > GetCurtime())
			return false;
	}

	return true;
}







template <class T>
constexpr const T& Max(const T& x, const T& y)
{
	return (x < y) ? y : x;
}








bool IsPressingMovementKeys(CUserCmd* cmd)
{
	if (!cmd)
		return false;

	if (cmd->buttons & IN_FORWARD ||
		cmd->buttons & IN_BACK || cmd->buttons & IN_LEFT || cmd->buttons & IN_RIGHT ||
		cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_MOVERIGHT)
		return true;

	return false;
}



namespace FEATURES
{
	namespace RAGEBOT
	{
		Antiaim antiaim;

		void Antiaim::Do(CUserCmd* cmd)
		{
			auto local_player = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
			if (!local_player || local_player->GetHealth() <= 0 || local_player->GetMoveType() == MOVETYPE_NOCLIP || local_player->GetMoveType() == MOVETYPE_LADDER || !Clientvariables->NewAntiAim.antiaim_enabled)
				return;
			
			auto weapon = local_player->GetWeapon();
			CGrenade* pCSGrenade = (CGrenade*)G::LocalPlayer->GetWeapon();
			if (!weapon || (pCSGrenade && pCSGrenade->GetThrowTime() > 0.f) && CanShoot(false))
				return;

			auto IsPickingUpWeapon = [local_player]() -> bool
			{
				Vector viewangles;
				g_pEngine->GetViewAngles(viewangles);

				Vector direction;
				Math::AngleVectors(viewangles, &direction);

				const auto start = local_player->GetVecOrigin() + local_player->GetViewOffset();
				const auto end = start + (direction * 130.f);

				Ray_t ray;
				ray.Init(start, end);

				trace_t trace;
				CTraceFilter filter;
				filter.pSkip = local_player;

				g_pEngineTrace->TraceRay(ray, MASK_ALL, &filter, &trace);
				if (!trace.m_pEnt)
					return false;

				const auto client_class = trace.m_pEnt->GetClientClass();

				if (!client_class || !trace.m_pEnt->IsWeapon())
					return false;

				return true;
			};

			if ((cmd->buttons & IN_USE && IsPickingUpWeapon()) || (cmd->buttons & IN_ATTACK && CanShoot(false)) || (cmd->buttons & IN_ATTACK2 && CanShoot(true)))
				return;

			cmd->viewangles.x = DoPitch();
			cmd->viewangles.y = DoYaw();

			cmd->viewangles = Math::NormalizeAngle(cmd->viewangles);
		}

		void Antiaim::UpdateLBY()
		{
			auto local_player = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
			if (!local_player || !(local_player->GetFlagsxd() & FL_ONGROUND))
				return;

			auto animstate = local_player->GetAnimState();
			if (!animstate)
				return;

			auto net_channel = g_pEngine->GetNetChannelInfo();
			if (!net_channel || net_channel->m_nChokedPackets)
				return;

			const float curtime = GetCurtime();
			if (animstate->speed_2d > 0.1 && !G::is_fakewalking)
			{
				m_next_lby_update_time = curtime + 0.22;
				m_last_move_time = curtime;
			}
			static float last_lby;

			if (last_lby != local_player->LowerBodyYaw())
			{
				last_lby = local_player->LowerBodyYaw();
				if (fabs(curtime - m_last_move_time) > 0.3 && fabs(Math::NormalizeYaw(local_player->LowerBodyYaw() - m_last_attempted_lby)) > 35.f)
					m_next_lby_update_time = curtime + 1.1 + g_pGlobals->interval_per_tick;
			}

			if (m_next_lby_update_time < curtime)
			{
				m_next_lby_update_time = curtime + 1.1;
				m_will_lby_update = true;
			}
		}

		float Antiaim::BreakLBY(float real_yaw, float desired_lby, bool double_flick)
		{
			auto local_player = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
			if (!local_player)
				return real_yaw;

			desired_lby = Math::GetYawDelta(desired_lby, local_player->LowerBodyYaw()) < 35.f ? local_player->LowerBodyYaw() : desired_lby;
			if (m_will_lby_update)
			{
				static bool switch_bool = false;
				switch_bool = !switch_bool;
				const float lby_delta = Math::NormalizeYaw(desired_lby - real_yaw);
				if (fabs(lby_delta) < 100.f)
					desired_lby = real_yaw + (switch_bool ? lby_delta : -lby_delta);

				m_will_lby_update = false;
				m_last_attempted_lby = desired_lby;
				return desired_lby;
			}

			if (fabs(Math::NormalizeYaw(real_yaw - desired_lby)) > 130.f && fabs(Math::NormalizeYaw(desired_lby - local_player->LowerBodyYaw())) < 35.f)
			{
				if (fabs(GetCurtime() - m_next_lby_update_time) < g_pGlobals->interval_per_tick)
					return desired_lby - 115.f;
				if (fabs(GetCurtime() - m_next_lby_update_time) > 1.1 - (g_pGlobals->interval_per_tick * 5))
					return desired_lby + 115.f;
			}

			return real_yaw;
		}

		float Antiaim::DoYaw()
		{
			

			auto local_player = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
			if (!local_player)
				return 0.f;

			bool is_jumping = !(local_player->GetFlagsxd() & FL_ONGROUND);
			bool is_moving = (local_player->GetVelocity().Length2D() > 0.1f) && !is_jumping && !G::is_fakewalking;

			/// returns viewangles/at target viewangles
			auto GetViewangles = [local_player](bool at_target_enabled) -> float
			{
				Vector viewangles;
				g_pEngine->GetViewAngles(viewangles);

				if (!at_target_enabled)
					return viewangles.y;

				/// get at target based on closest fov
				float closest_fov = 360.f, closest_fov_yaw = 181.f;
				for (int i = 0; i < 64; i++)
				{
					auto entity = g_pEntitylist->GetClientEntity(i);
					if (!entity || entity->GetHealth() <= 0 || entity->GetTeamNum() == local_player->GetTeamNum() || entity->IsDormant())
						continue;

					const float current_yaw = Math::CalcAnglexd(local_player->GetVecOrigin(), entity->GetVecOrigin()).y;
					const float current_fov = fabs(Math::NormalizeYaw(current_yaw - viewangles.y));
					if (current_fov < closest_fov)
					{
						closest_fov = current_fov;
						closest_fov_yaw = current_yaw;
					}
				}

				/// couldn't find a valid entity
				if (closest_fov == 360.f)
					return viewangles.y;

				return closest_fov_yaw;
			};

			/// returns the base real angle from the combobox setting
			auto GetBaseRealAngle = [local_player, GetViewangles](int setting, bool at_target_enabled, float spin_speed) -> float
			{
				/// base yaw
				switch (setting)
				{
				case 0: /// viewangle
					return GetViewangles(at_target_enabled);
				case 1: /// static
					return 0.f;
				case 2: /// lby
					return local_player->LowerBodyYaw();
				case 3: /// adaptive
					return GetViewangles(at_target_enabled);
				case 4: /// spin
					return GetCurtime() * spin_speed * 100.f;
				}

				return 0.f;
			};

			/// returns the base fake angle from the combobox setting
			auto GetBaseFakeAngle = [local_player, GetViewangles](int setting, bool at_target_enabled, float spin_speed) -> float
			{
				/// base yaw
				switch (setting)
				{
				case 0: /// viewangle
					return GetViewangles(at_target_enabled);
				case 1: /// static
					return 0.f;
				case 2: /// lby
					return local_player->LowerBodyYaw();
				case 3: /// adaptive
					return GetViewangles(at_target_enabled);
				case 4: /// spin
					return GetCurtime() * spin_speed * 100.f;
				case 5: /// real inverse
					return G::real_angles.y + 180.f;
				}

				return 0.f;
			};

			auto DoRealYaw = [this, is_jumping, is_moving, local_player, GetViewangles, GetBaseRealAngle]() -> float
			{
				float real_yaw = GetViewangles(false), lby = 0.f;

				if (is_jumping)
				{
					
					real_yaw = GetBaseRealAngle(Clientvariables->NewAntiAim.antiaim_jumping_real_yaw,
						Clientvariables->NewAntiAim.antiaim_jumping_at_target_enabled, Clientvariables->NewAntiAim.antiaim_jumping_real_spin_speed);

					/// offset
					if (Clientvariables->NewAntiAim.antiaim_jumping_real_yaw == 3) /// adaptive
						real_yaw += Math::NormalizeYaw(GetViewangles(false) - GetViewangles(true)) > 0.f ? Clientvariables->NewAntiAim.antiaim_jumping_real_offset : -Clientvariables->NewAntiAim.antiaim_jumping_real_offset;
					else
						real_yaw += Clientvariables->NewAntiAim.antiaim_jumping_real_offset;

					/// jitter range
					real_yaw += Math::RandomNumber(Clientvariables->NewAntiAim.antiaim_jumping_real_jrange * -0.5f, Clientvariables->NewAntiAim.antiaim_jumping_real_jrange * 0.5f);
				}
				else if (is_moving)
				{
					real_yaw = GetBaseRealAngle(Clientvariables->NewAntiAim.antiaim_moving_real_yaw,
						Clientvariables->NewAntiAim.antiaim_moving_at_target_enabled, Clientvariables->NewAntiAim.antiaim_moving_real_spin_speed);

					/// offset
					if (Clientvariables->NewAntiAim.antiaim_moving_real_yaw == 3) /// adaptive
						real_yaw += Math::NormalizeYaw(GetViewangles(false) - GetViewangles(true)) > 0.f ? Clientvariables->NewAntiAim.antiaim_moving_real_offset : -Clientvariables->NewAntiAim.antiaim_moving_real_offset;
					else
						real_yaw += Clientvariables->NewAntiAim.antiaim_moving_real_offset;

					/// jitter range
					real_yaw += Math::RandomNumber(Clientvariables->NewAntiAim.antiaim_moving_real_jrange * -0.5f, Clientvariables->NewAntiAim.antiaim_moving_real_jrange * 0.5f);
				}
				else /// standing
				{
					real_yaw = GetBaseRealAngle(Clientvariables->NewAntiAim.antiaim_standing_real_yaw,
						Clientvariables->NewAntiAim.antiaim_standing_at_target_enabled, Clientvariables->NewAntiAim.antiaim_standing_real_spin_speed);

					/// offset
					if (Clientvariables->NewAntiAim.antiaim_standing_real_yaw == 3) /// adaptive
						real_yaw += Math::NormalizeYaw(GetViewangles(false) - GetViewangles(true)) > 0.f ? Clientvariables->NewAntiAim.antiaim_standing_real_offset : -Clientvariables->NewAntiAim.antiaim_standing_real_offset;
					else
						real_yaw += Clientvariables->NewAntiAim.antiaim_standing_real_offset;

					/// make sure to do this before we add the jitter shit
					lby = real_yaw + Clientvariables->NewAntiAim.antiaim_standing_real_lby_delta;

					/// jitter range
					real_yaw += Math::RandomNumber(Clientvariables->NewAntiAim.antiaim_standing_real_jrange * -0.5f, Clientvariables->NewAntiAim.antiaim_standing_real_jrange * 0.5f);
				}

				if (DoEdging(real_yaw, lby))
					real_yaw += Math::RandomNumber(Clientvariables->NewAntiAim.antiaim_edging_jrange * -0.5f, Clientvariables->NewAntiAim.antiaim_edging_jrange * 0.5f);

				return BreakLBY(real_yaw, lby);
			};
			auto DoFakeYaw = [this, is_jumping, is_moving, local_player, GetViewangles, GetBaseFakeAngle]() -> float
			{
				float fake_yaw = GetViewangles(false);

				if (is_jumping)
				{
					fake_yaw = GetBaseFakeAngle(Clientvariables->NewAntiAim.antiaim_jumping_fake_yaw,
						Clientvariables->NewAntiAim.antiaim_jumping_at_target_enabled, Clientvariables->NewAntiAim.antiaim_jumping_fake_spin_speed);

					/// offset
					if (Clientvariables->NewAntiAim.antiaim_jumping_fake_yaw == 3) /// adaptive
						fake_yaw += Math::NormalizeYaw(GetViewangles(false) - GetViewangles(true)) > 0.f ? Clientvariables->NewAntiAim.antiaim_jumping_fake_offset : -Clientvariables->NewAntiAim.antiaim_jumping_fake_offset;
					else
						fake_yaw += Clientvariables->NewAntiAim.antiaim_jumping_fake_offset;

					/// jitter range
					fake_yaw += Math::RandomNumber(Clientvariables->NewAntiAim.antiaim_jumping_fake_jrange * -0.5f, Clientvariables->NewAntiAim.antiaim_jumping_fake_jrange * 0.5f);
				}
				else if (is_moving)
				{
					fake_yaw = GetBaseFakeAngle(Clientvariables->NewAntiAim.antiaim_moving_fake_yaw,
						Clientvariables->NewAntiAim.antiaim_moving_at_target_enabled, Clientvariables->NewAntiAim.antiaim_moving_fake_spin_speed);

					/// offset
					if (Clientvariables->NewAntiAim.antiaim_moving_fake_yaw == 3) /// adaptive
						fake_yaw += Math::NormalizeYaw(GetViewangles(false) - GetViewangles(true)) > 0.f ? Clientvariables->NewAntiAim.antiaim_moving_fake_offset : -Clientvariables->NewAntiAim.antiaim_moving_fake_offset;
					else
						fake_yaw += Clientvariables->NewAntiAim.antiaim_moving_fake_offset;

					/// jitter range
					fake_yaw += Math::RandomNumber(Clientvariables->NewAntiAim.antiaim_moving_fake_jrange * -0.5f, Clientvariables->NewAntiAim.antiaim_moving_fake_jrange * 0.5f);
				}
				else /// standing
				{
					fake_yaw = GetBaseFakeAngle(Clientvariables->NewAntiAim.antiaim_standing_fake_yaw,
						Clientvariables->NewAntiAim.antiaim_standing_at_target_enabled, Clientvariables->NewAntiAim.antiaim_standing_fake_spin_speed);

					/// offset
					if (Clientvariables->NewAntiAim.antiaim_standing_fake_yaw == 3) /// adaptive
						fake_yaw += Math::NormalizeYaw(GetViewangles(false) - GetViewangles(true)) > 0.f ? Clientvariables->NewAntiAim.antiaim_standing_fake_offset : -Clientvariables->NewAntiAim.antiaim_standing_fake_offset;
					else
						fake_yaw += Clientvariables->NewAntiAim.antiaim_standing_fake_offset;

					/// jitter range
					fake_yaw += Math::RandomNumber(Clientvariables->NewAntiAim.antiaim_standing_fake_jrange * -0.5f, Clientvariables->NewAntiAim.antiaim_standing_fake_jrange * 0.5f);
				}

				float tmp_float;
				if (DoEdging(fake_yaw, tmp_float))
					fake_yaw += 180.f + Math::RandomNumber(-75.f, 75.f);

				if (Clientvariables->Visuals.fakechams)
					Clientvariables->Visuals.lineFakeAngle = fake_yaw;

				return fake_yaw;
			};

			return G::should_send_packet ? DoFakeYaw() : DoRealYaw();
		}

		float Antiaim::DoPitch()
		{
			auto local_player = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
			if (!local_player)
				return 0.f;

			auto GetPitchFromSetting = [](int setting) -> float
			{
				Vector viewangles;
				g_pEngine->GetViewAngles(viewangles);

				switch (setting)
				{
				case 0:
					return viewangles.x;
				case 1:
					return 89.f;
				case 2:
					return -89.f;
				default:
					return Math::RandomNumber(-89.f, 89.f);
				}
			};

			bool is_jumping = !(local_player->GetFlagsxd() & FL_ONGROUND);
			bool is_moving = (local_player->GetVelocity().Length2D() > 0.1f) && !is_jumping;

			if (is_jumping)
				return GetPitchFromSetting(Clientvariables->NewAntiAim.antiaim_jumping_pitch);
			else if (is_moving)
				return GetPitchFromSetting(Clientvariables->NewAntiAim.antiaim_moving_pitch);
			else /// standing pitch
				return GetPitchFromSetting(Clientvariables->NewAntiAim.antiaim_standing_pitch);
		}

		bool Antiaim::DoEdging(float& yaw, float& lby)
		{
			auto local_player = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
			if (!local_player)
				return false;

			/// check if enabled
			const bool is_jumping = !(local_player->GetFlagsxd() & FL_ONGROUND);
			const bool is_moving = (local_player->GetVelocity().Length2D() > 0.1) && !is_jumping;
			if (is_jumping)
			{
				if (!Clientvariables->NewAntiAim.antiaim_edging_jumping_enabled)
					return false;
			}
			else if (is_moving)
			{
				if (!Clientvariables->NewAntiAim.antiaim_edging_moving_enabled)
					return false;
			}
			else if (!Clientvariables->NewAntiAim.antiaim_edging_standing_enabled)
				return false;

			Vector viewangles;
			g_pEngine->GetViewAngles(viewangles);

			auto DoWallDetection = [local_player, viewangles](float& yaw, float& lby) -> bool
			{
				trace_t trace;
				Ray_t ray;
				CTraceWorldOnly filter;

				static constexpr float trace_distance = 25.f;
				const auto head_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

				float last_fraction = 1.f;
				for (int i = 0; i < 360; i += 2)
				{
					Vector direction;
					Math::AngleVectors(Vector(0, i, 0), &direction);

					ray.Init(head_position, head_position + (direction * trace_distance));
					g_pEngineTrace->TraceRay(ray, MASK_ALL, &filter, &trace);

					if (trace.fraction > last_fraction)
					{
						yaw = i - 2;
						lby = yaw + Clientvariables->NewAntiAim.antiaim_edging_lby_delta;

						return true;
					}

					last_fraction = trace.fraction;
				}

				return false;
			};
			auto DoFreestanding = [local_player, viewangles, is_moving, this](float& yaw, float& lby) -> bool
			{
				static constexpr int damage_tolerance = 10;

				std::vector<CBaseEntity*> enemies;

				/// Find the lowest fov enemy
				CBaseEntity* closest_enemy = nullptr;
				float lowest_fov = 360.f;
				for (int i = 0; i < 64; i++)
				{
					auto entity = g_pEntitylist->GetClientEntity(i);
					if (!entity || entity->IsDormant() || entity->GetTeamNum() == local_player->GetTeamNum() || entity->GetHealth() <= 0)
						continue;

					const float current_fov = fabs(Math::NormalizeYaw(Math::CalcAnglexd(local_player->GetVecOrigin(), entity->GetVecOrigin()).y - viewangles.y));
					if (current_fov < lowest_fov)
					{
						lowest_fov = current_fov;
						closest_enemy = entity;
					}

					enemies.push_back(entity);
				}

				/// No enemies
				if (closest_enemy == nullptr)
					return false;

				const float at_target_yaw = Math::CalcAnglexd(local_player->GetVecOrigin(), closest_enemy->GetVecOrigin()).y;
				const float right_yaw = at_target_yaw - 90.f;
				const float left_yaw = at_target_yaw + 90.f;

				/// Misc functions
				auto CalcDamage = [local_player, enemies](Vector point) -> int
				{
					int damage = 0;
					for (auto& enemy : enemies)
					{
						damage += Max<int>(autowall.CalculateDamage(enemy->GetVecOrigin() + Vector(0, 0, 64.f), point, enemy, local_player, 1).damage,
							autowall.CalculateDamage(enemy->GetVecOrigin() + Vector(0, 0, 49.f), point, enemy, local_player, 1).damage);
					}
					return damage;
				};
				auto RotateAndExtendPosition = [](Vector position, float yaw, float distance) -> Vector
				{
					Vector direction;
					Math::AngleVectors(Vector(0, yaw, 0), &direction);

					return position + (direction * distance);
				};
				auto RotateLBYAndYaw = [right_yaw, left_yaw, local_player](int right_damage, int left_damage, float lby_delta, float& yaw, float& lby, bool prefect_angle = false, bool symetrical = true) -> bool
				{
					const bool prefer_right = right_damage < left_damage;

					yaw = prefer_right ? right_yaw : left_yaw;
					if (symetrical)
						lby_delta = lby_delta * (prefer_right ? -1.f : 1.f);

					/// If not moving
					if (prefect_angle)
					{
						if (local_player->GetVelocity().Length2D() < 0.1f)
							yaw = GetLBYRotatedYaw(yaw + lby_delta, yaw);
						else
							yaw = GetLBYRotatedYaw(local_player->LowerBodyYaw(), yaw);
					}

					lby = yaw + lby_delta;

					if (fabs(Math::NormalizeYaw(local_player->LowerBodyYaw() - lby)) < 35.f)
					{
						yaw = local_player->LowerBodyYaw() - lby_delta;
						lby = yaw + lby_delta;
					}

					return true;
				};
				auto DoBackFreestanding = [at_target_yaw, local_player](float& yaw, float& lby) -> bool
				{
					yaw = at_target_yaw + 180.f;
					lby = yaw;

					return true;
				};

				const auto head_position = local_player->GetVecOrigin() + Vector(0, 0, 74.f);
				const auto back_head_position = RotateAndExtendPosition(head_position, at_target_yaw + 180.f, 17.f);
				auto right_head_position = RotateAndExtendPosition(head_position, right_yaw, 17.f);
				auto left_head_position = RotateAndExtendPosition(head_position, left_yaw, 17.f);

				int right_damage = CalcDamage(right_head_position), left_damage = CalcDamage(left_head_position), back_damage = CalcDamage(back_head_position);

				/// too much damage to both sides
				if (right_damage > damage_tolerance && left_damage > damage_tolerance)
				{
					/// do backwards if valid
					if (back_damage < damage_tolerance)
						return DoBackFreestanding(yaw, lby);

					return false;
				}

				/// keep searching for a better angle
				if (right_damage == left_damage)
				{
					/// if on top of them, prioritise backwards antiaim
					if (Math::NormalizePitch(Math::CalcAnglexd(local_player->GetVecOrigin(), closest_enemy->GetVecOrigin()).x) > 15.f && back_damage < damage_tolerance)
						return DoBackFreestanding(yaw, lby);

					/// do some traces a bit further out
					right_head_position = RotateAndExtendPosition(head_position, right_yaw, 50.f);
					left_head_position = RotateAndExtendPosition(head_position, left_yaw, 50.f);

					right_damage = CalcDamage(right_head_position), left_damage = CalcDamage(left_head_position);
					if (right_damage == left_damage)
					{
						/// just return the side closest to a wall
						right_head_position = TraceToEnd(head_position, RotateAndExtendPosition(head_position, right_yaw, 17.f));
						left_head_position = TraceToEnd(head_position, RotateAndExtendPosition(head_position, left_yaw, 17.f));

						float distance_1, distance_2;
						trace_t trace;
						Ray_t ray;
						CTraceWorldOnly filter;
						const auto end_pos = closest_enemy->GetVecOrigin() + Vector(0, 0, 64.f);

						/// right position
						ray.Init(right_head_position, end_pos);
						g_pEngineTrace->TraceRay(ray, MASK_ALL, &filter, &trace);
						distance_1 = (right_head_position - trace.endpos).Length();

						/// left position
						ray.Init(left_head_position, end_pos);
						g_pEngineTrace->TraceRay(ray, MASK_ALL, &filter, &trace);
						distance_2 = (left_head_position - trace.endpos).Length();

						if (fabs(distance_1 - distance_2) > 15.f)
						{
							RotateLBYAndYaw(distance_1, distance_2, Clientvariables->NewAntiAim.antiaim_edging_lby_delta, yaw, lby);

							if ((is_moving && !IsPressingMovementKeys(reinterpret_cast<CUserCmd*>(G::last_cmd))) ||
								(GetCurtime() - m_last_move_time < 0.22 && !is_moving))
								yaw = lby + 60.f;

							return true;
						}

						return DoBackFreestanding(yaw, lby);
					}
					else
					{
						RotateLBYAndYaw(right_damage, left_damage, Clientvariables->NewAntiAim.antiaim_edging_lby_delta, yaw, lby);

						if ((is_moving && !IsPressingMovementKeys(reinterpret_cast<CUserCmd*>(G::last_cmd))) ||
							(GetCurtime() - m_last_move_time < 0.22 && !is_moving))
							yaw = lby + 60.f;

						return true;
					}
				}
				else /// found an angle that does less damage than the other
				{
					/// if on top of them, prioritise backwards antiaim
					if (Math::NormalizePitch(Math::CalcAnglexd(local_player->GetVecOrigin(), closest_enemy->GetVecOrigin()).x) > 15.f && back_damage < damage_tolerance)
						return DoBackFreestanding(yaw, lby);

					const bool prefer_right = (right_damage < left_damage);
					const float lby_delta = Clientvariables->NewAntiAim.antiaim_edging_lby_delta;

					yaw = prefer_right ? right_yaw : left_yaw;
					lby = yaw + lby_delta;

					if (fabs(Math::NormalizeYaw(local_player->LowerBodyYaw() - lby)) < 35.f)
					{
						yaw = local_player->LowerBodyYaw() - lby_delta;
						lby = yaw + lby_delta;
					}

					if (CalcDamage(RotateAndExtendPosition(head_position, lby, 18.f)) > 0)
					{
						lby = yaw + (prefer_right ? -115.f : 115.f);
						if (CalcDamage(RotateAndExtendPosition(head_position, lby, 18.f)) > 0)
							lby = yaw;
					}
					else
					{
						if ((is_moving && !IsPressingMovementKeys(reinterpret_cast<CUserCmd*>(G::last_cmd))) ||
							(GetCurtime() - m_last_move_time < 0.22 && !is_moving))
							yaw = lby + 60.f;
					}

					return true;
				}

				return false;
			};

			const int edging_type = Clientvariables->NewAntiAim.antiaim_edging_type;
			switch (edging_type)
			{
			case 0: /// wall detection
				return DoWallDetection(yaw, lby);
			case 1: /// freestanding
				if (DoFreestanding(yaw, lby))
					return true;

				return false;
			case 2: /// freestanding + wall detection
				if (DoWallDetection(yaw, lby))
					return true;

				return DoFreestanding(yaw, lby);
			}

			return false;
		}
	}
}

