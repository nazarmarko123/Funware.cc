#include "sdk.h"
#include "Resolver2.h"
#include "global.h"
#include "BacktrackingHelper.h"
#include "Math.h"
#include "GameUtils.h"
#include "Menu.h"
#include "Autowall.h"


PerfectResolver* set = new PerfectResolver;

bool PerfectResolver::AntiFreestanding(CBaseEntity* entity, float& yaw)
{

	auto local_player = G::LocalPlayer;
	if (!local_player)
		return false;

	const float height = 64;

	const auto latency_based_eye_pos = entity->GetEyePosition();
	Vector direction_1, direction_2;
	Math::AngleVectors(Vector(0.f, GameUtils::CalculateAngle(local_player->GetOrigin(), entity->GetOrigin()).y - 90.f, 0.f), &direction_1);
	Math::AngleVectors(Vector(0.f, GameUtils::CalculateAngle(local_player->GetOrigin(), entity->GetOrigin()).y + 90.f, 0.f), &direction_2);

	const auto left_eye_pos = entity->GetOrigin() + Vector(0, 0, height) + (direction_1 * 16.f);
	const auto right_eye_pos = entity->GetOrigin() + Vector(0, 0, height) + (direction_2 * 16.f);

	Ray_t ray;
	trace_t trace;
	CTraceWorldOnly filter;

	ray.Init(left_eye_pos, latency_based_eye_pos);
	g_pEngineTrace->TraceRay_NEW(ray, MASK_ALL, &filter, &trace);
	pResolverData[entity->GetIndex()].left_fraction = trace.fraction;

	ray.Init(right_eye_pos, latency_based_eye_pos);
	g_pEngineTrace->TraceRay_NEW(ray, MASK_ALL, &filter, &trace);
	pResolverData[entity->GetIndex()].right_fraction = trace.fraction;

	const float at_target_yaw = GameUtils::CalculateAngle(local_player->GetOrigin(), entity->GetOrigin()).y;

	if (pResolverData[entity->GetIndex()].right_fraction < pResolverData[entity->GetIndex()].left_fraction)
		yaw = at_target_yaw - 90.f;
	else
		yaw = at_target_yaw + 90.f;//
	return true;
}

void PerfectResolver::DoCM()
{
	auto local_player = G::LocalPlayer;
	if (!local_player || local_player->GetHealth() <= 0)
		return;


	auto nci = g_pEngine->GetNetChannelInfo();
	if (!nci)
		return;

	for (int i = 0; i < 64; i++)
	{
		CBaseEntity* entity = g_pEntitylist->GetClientEntity(i);

		if (!entity || entity->GetHealth() <= 0 || entity->GetTeamNum() == local_player->GetTeamNum() || entity->IsDormant())
			continue;
		const auto latency_based_eye_pos = entity->GetEyePosition();

		const float height = 64;

		Vector direction_1, direction_2;
		Math::AngleVectors(Vector(0.f, GameUtils::CalculateAngle(local_player->GetOrigin(), entity->GetOrigin()).y - 90.f, 0.f), &direction_1);
		Math::AngleVectors(Vector(0.f, GameUtils::CalculateAngle(local_player->GetOrigin(), entity->GetOrigin()).y + 90.f, 0.f), &direction_2);

		const auto left_eye_pos = entity->GetOrigin() + Vector(0, 0, height) + (direction_1 * 16.f);
		const auto right_eye_pos = entity->GetOrigin() + Vector(0, 0, height) + (direction_2 * 16.f);
		//bameware things xd lol roflan
		pResolverData[entity->GetIndex()].left_damage = autowall.CalculateDamage(latency_based_eye_pos,
			left_eye_pos, local_player, entity, 1).damage;
		pResolverData[entity->GetIndex()].right_damage = autowall.CalculateDamage(latency_based_eye_pos,
			right_eye_pos, local_player, entity, 1).damage;

		Ray_t ray;
		trace_t trace;
		CTraceWorldOnly filter;

		ray.Init(left_eye_pos, latency_based_eye_pos);
		g_pEngineTrace->TraceRay_NEW(ray, MASK_ALL, &filter, &trace);
		pResolverData[entity->GetIndex()].left_fraction = trace.fraction;

		ray.Init(right_eye_pos, latency_based_eye_pos);
		g_pEngineTrace->TraceRay_NEW(ray, MASK_ALL, &filter, &trace);
		pResolverData[entity->GetIndex()].right_fraction = trace.fraction;

	}
}
bool PerfectResolver::IsYawSideways(CBaseEntity* entity, float yaw)
{
	auto local_player = G::LocalPlayer;
	if (!local_player)
		return false;
	Vector local_target_angle;
	const auto at_target_yaw = GameUtils::CalculateAngle(local_player->GetOrigin(), entity->GetOrigin()).y;
	const float delta = fabs(Math::NormalizeYaw(at_target_yaw - yaw));

	return delta > 20.f && delta < 160.f;
}

void PerfectResolver::DoResolver()
{
	for (int i = 1; i < g_pGlobals->maxClients; i++)
	{
		if (i == g_pEngine->GetLocalPlayer())
			continue;
		CBaseEntity* pEntity = g_pEntitylist->GetClientEntity(i);
		if (pEntity && pEntity->GetHealth() > 0)
		{
			if (pEntity->IsDormant())
				continue;
			if (pEntity->GetTeamNum() == G::LocalPlayer->GetTeamNum())
				continue;
			CBaseAnimState* anim_state = pEntity->GetAnimState();

			float real_velocity = 0.0f;
			if (anim_state)
				real_velocity = anim_state->speed_2d;
			else
				real_velocity = pEntity->GetVelocity().Length2D();
			for (int i = 0; i < 15; i++) /// go through each animation layer
			{
				pResolverData[pEntity->GetIndex()].anim_layers[i] = pEntity->GetAnimOverlay(i);
				if (pResolverData[pEntity->GetIndex()].anim_layers[4].m_flWeight != 0.0 || pResolverData[pEntity->GetIndex()].anim_layers[6].m_flCycle != 0.0 || pResolverData[pEntity->GetIndex()].anim_layers[5].m_flWeight != 0.0 && *pEntity->GetFlags() & FL_ONGROUND && 0.1f < real_velocity)
				{
					pResolverData[pEntity->GetIndex()].bfakewalk = true;
				}
				else
				{
					pResolverData[pEntity->GetIndex()].bfakewalk = false;
				}
			}
			if (pEntity->GetVelocity().Length2D() > 40 && *pEntity->GetFlags() & FL_ONGROUND)
				pResolverData[pEntity->GetIndex()].is_moving = true;
			else
				pResolverData[pEntity->GetIndex()].is_moving = false;

			if (pEntity->GetVelocity().Length2D() > 0.1 && !pResolverData[pEntity->GetIndex()].bfakewalk) {
				pResolverData[pEntity->GetIndex()].next_lby_update = pEntity->GetSimulationTime() + g_pGlobals->interval_per_tick + 0.5f;
			}

			if (!pResolverData[pEntity->GetIndex()].is_moving && !pResolverData[pEntity->GetIndex()].bfakewalk && pEntity->GetSimulationTime() < pResolverData[pEntity->GetIndex()].next_lby_update) {
				pResolverData[pEntity->GetIndex()].lastlbystand = pEntity->LowerBodyYaw();
			}


			if (G::UserCmd->buttons & IN_ATTACK) {
				pResolverData[pEntity->GetIndex()].shotaimangles++;
			}
			if (pResolverData[pEntity->GetIndex()].shotaimangles > 6 || !G::LocalPlayer->isAlive()) {
				pResolverData[pEntity->GetIndex()].shotaimangles = 0;
			}

			if (G::UserCmd->buttons & IN_ATTACK && !G::playerhurtcalled == true) {
				pResolverData[pEntity->GetIndex()].missed++;
			}
			if (pResolverData[pEntity->GetIndex()].missed > 5 || !G::LocalPlayer->isAlive()) {
				pResolverData[pEntity->GetIndex()].missed = 0;
			}
			if (abs(pEntity->GetEyeAnglesPtr()->y - pEntity->LowerBodyYaw()) > 35.f)
				pResolverData[pEntity->GetIndex()].using_fake_angles = true;
			else
				pResolverData[pEntity->GetIndex()].using_fake_angles = false;

			if (!*pEntity->GetFlags() & FL_ONGROUND)
				pResolverData[pEntity->GetIndex()].is_jumping = true;
			else
				pResolverData[pEntity->GetIndex()].is_jumping = false;

			if (Clientvariables->Ragebot.perfectresolver)
			{
				
				Vector* pAngles = pEntity->GetEyeAnglesPtr();
				Vector local_target_angle1;
				Math::CalcAngle(G::LocalPlayer->GetOrigin(), pEntity->GetOrigin(), local_target_angle1);

				if (pResolverData[pEntity->GetIndex()].is_moving)
				{
					pResolverData[pEntity->GetIndex()].lastmovinglby = pEntity->LowerBodyYaw();
					pAngles->y = pEntity->LowerBodyYaw();
				}
				else if (!pResolverData[pEntity->GetIndex()].is_moving)
				{
					pAngles->y = pResolverData[pEntity->GetIndex()].lastmovinglby;
				}
				if (pEntity->LowerBodyYaw() != pResolverData[pEntity->GetIndex()].lby1)
				{
					pAngles->y = pEntity->LowerBodyYaw();
					pResolverData[pEntity->GetIndex()].lby1 = pEntity->LowerBodyYaw();
					pResolverData[pEntity->GetIndex()].nextlbyup = pEntity->GetSimulationTime() + g_pGlobals->interval_per_tick + 1.1;
					pResolverData[pEntity->GetIndex()].lbywasupdated = true;
				}
				else if (pEntity->GetSimulationTime() >= pResolverData[pEntity->GetIndex()].nextlbyup)
				{
					pAngles->y = pEntity->LowerBodyYaw();
					pResolverData[pEntity->GetIndex()].nextlbyup = pEntity->GetSimulationTime() + g_pGlobals->interval_per_tick + 1.1;
					pResolverData[pEntity->GetIndex()].lbywasupdated = true;
				}
				if (GetAsyncKeyState(0x56))
				{
					Vector pos_enemy;
					Vector local_target_angle;
					if (GameUtils::WorldToScreen(pEntity->GetOrigin(), pos_enemy)) {
						Math::CalcAngle(G::LocalPlayer->GetOrigin(), pEntity->GetOrigin(), local_target_angle);
						ImVec2 ImGui::GetMousePos();
						float delta = ImGui::GetMousePos().x - pos_enemy.x;
						if (delta < 0) pEntity->GetEyeAnglesPtr()->y = local_target_angle.y + 90;
						else pEntity->GetEyeAnglesPtr()->y = local_target_angle.y - 90;
					}
				}
			}
			
			if (!Clientvariables->Misc.AntiUTxd) {
				std::string strPitch = std::to_string(pEntity->GetEyeAnglesPtr()->x);
				if (pEntity->GetEyeAnglesPtr()->x < -179.f)
					pEntity->GetEyeAnglesPtr()->x += 360.f;
				else if (pEntity->GetEyeAnglesPtr()->x > 90.0 || pEntity->GetEyeAnglesPtr()->x < -90.0)
					pEntity->GetEyeAnglesPtr()->x = 89.f;
				else if (pEntity->GetEyeAnglesPtr()->x > 89.0 && pEntity->GetEyeAnglesPtr()->x < 91.0)
					pEntity->GetEyeAnglesPtr()->x -= 90.f;
				else if (pEntity->GetEyeAnglesPtr()->x > 179.0 && pEntity->GetEyeAnglesPtr()->x < 181.0)
					pEntity->GetEyeAnglesPtr()->x -= 180;
				else if (pEntity->GetEyeAnglesPtr()->x > -179.0 && pEntity->GetEyeAnglesPtr()->x < -181.0)
					pEntity->GetEyeAnglesPtr()->x += 180;
				else if (fabs(pEntity->GetEyeAnglesPtr()->x) == 0)
					pEntity->GetEyeAnglesPtr()->x = std::copysign(89.0f, pEntity->GetEyeAnglesPtr()->x);
			}
		}
	}
}