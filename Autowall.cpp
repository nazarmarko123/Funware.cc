#include "sdk.h"
#include "Autowall.h"
#include "GameUtils.h"
#include "Math.h"
#include "Menu.h"
#include "global.h"
CAutowall* g_Autowall = new CAutowall;

#define    HITGROUP_GENERIC    0
#define    HITGROUP_HEAD        1
#define    HITGROUP_CHEST        2
#define    HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4    
#define HITGROUP_RIGHTARM    5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG    7
#define HITGROUP_GEAR        10

/*
$qch: oh ye you will
$qch: like handlebulletpen doesn't check for the hitbox or something
$qch: it only check for materials
$qch: between you and point
D3dC0m: But the damage will always be 0
$qch: no
$qch: it's based of you current weapon damage
$qch: than it check for the materials and lower damage based on this
$qch: then scale based of hitbox group
$qch: All you will have to fix
D3dC0m: OH
D3dC0m: Use a traceray
$qch: is that you will need to manually pass hitgroup
D3dC0m: That ignores eveerything
D3dC0m: than players
D3dC0m: except*
D3dC0m: to get hitgroup
$qch: ye
$qch: I made a func to dump hitgroup based of hitbox
$qch: so now I have a func hitbox to hitgroup
D3dC0m: ye

*/

float GetHitgroupDamageMult(int iHitGroup)
{
	/* Head	400% (4X)
	Chest & Arm	100% (1X)
	Stomach	125% (1.25X)
	Leg	75% (0.75X) */

	switch (iHitGroup) {
	case HITGROUP_GENERIC:
		return 1.0f;
	case HITGROUP_HEAD:
		return 4.0f;
	case HITGROUP_CHEST:
		return 1.0f;
	case HITGROUP_STOMACH:
		return 1.25f;
	case HITGROUP_LEFTARM:
		return 1.0f;
	case HITGROUP_RIGHTARM:
		return 1.0f;
	case HITGROUP_LEFTLEG:
		return 0.75f;
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	case HITGROUP_GEAR:
		return 1.0f;
	default:
		break;
	}
	return 1.0f;
}

void ScaleDamage(int hitgroup, CBaseEntity *enemy, float weapon_armor_ratio, float &current_damage)
{
	current_damage *= GetHitgroupDamageMult(hitgroup);

	if (enemy->GetArmor() > 0) {
		if (hitgroup == HITGROUP_HEAD) {
			if (enemy->HasHelmet())
				current_damage *= weapon_armor_ratio;
		}
		else {
			current_damage *= weapon_armor_ratio;
		}
	}
}

bool DidHitNonWorldEntity(CBaseEntity* entity)
{
	return entity != nullptr && entity->GetIndex() != 0;
}

bool IsBreakableEntity(CBaseEntity* entity)
{
	ClientClass* pClass = (ClientClass*)entity->GetClientClass();

	if (!pClass)
		return false;

	return pClass->m_ClassID == (int)CSClasses::CBreakableProp || pClass->m_ClassID == (int)CSClasses::CBreakableSurface;
}

bool CAutowall::TraceToExit(Vector &end, trace_t *enter_trace, Vector start, Vector dir, trace_t *exit_trace)
{
	float distance = 0.0f;

	while (distance <= 90.0f)
	{
		distance += 4.0f;
		end = start + dir * distance;

		auto point_contents = g_pEngineTrace->GetPointContents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL);

		if (point_contents & MASK_SHOT_HULL && (!(point_contents & CONTENTS_HITBOX)))
			continue;

		auto new_end = end - (dir * 4.0f);

		GameUtils::UTIL_TraceLine(end, new_end, 0x4600400B, G::LocalPlayer, 0, exit_trace);

		if (exit_trace->startSolid && exit_trace->surface.flags & SURF_HITBOX)
		{
			GameUtils::UTIL_TraceLine(end, new_end, 0x4600400B, exit_trace->m_pEnt, 0, exit_trace);

			if ((exit_trace->fraction < 1.0f || exit_trace->allSolid) && !exit_trace->startSolid)
			{
				end = exit_trace->endpos;
				return true;
			}
			continue;
		}

		if (!(exit_trace->fraction < 1.0 || exit_trace->allSolid || exit_trace->startSolid) || exit_trace->startSolid)
		{
			if (exit_trace->m_pEnt)
			{
				if (DidHitNonWorldEntity(exit_trace->m_pEnt) && IsBreakableEntity(enter_trace->m_pEnt))
					return true;
			}
			continue;
		}

		if (((exit_trace->surface.flags >> 7) & 1) && !((enter_trace->surface.flags >> 7) & 1))
			continue;

		if (exit_trace->plane.normal.Dot(dir) <= 1.0f)
		{
			auto fraction = exit_trace->fraction * 4.0f;
			end = end - (dir * fraction);
			return true;
		}
	}
	return false;
}

bool CAutowall::HandleBulletPenetration(CSWeaponInfo *wpn_data, FireBulletData &data)
{
	surfacedata_t *enter_surface_data = g_pPhysics->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;


	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= (float)(pow(wpn_data->range_modifier, (data.trace_length * 0.002)));

	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	Vector dummy;
	trace_t trace_exit;

	if (!TraceToExit(dummy, &data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
		return false;

	surfacedata_t *exit_surface_data = g_pPhysics->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
	{
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
	}

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->penetration) * 1.25f);
	float thickness = VectorLength(trace_exit.endpos - data.enter_trace.endpos);

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;


	float lost_damage = fmaxf(0.0f, v35 + thickness);

	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;
}

void GameUtils::UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, CBaseEntity*ignore, int collisionGroup, trace_t *ptr)
{
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	CTraceFilter traceFilter;
	traceFilter.pSkip = ignore;
	g_pEngineTrace->TraceRay(ray, mask, &traceFilter, ptr);
}

void GameUtils::UTIL_ClipTraceToPlayers(CBaseEntity* pEntity, Vector start, Vector end, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{
	trace_t playerTrace;
	Ray_t ray;
	float smallestFraction = tr->fraction;

	ray.Init(start, end);

	if (!pEntity || !pEntity->isAlive() || pEntity->IsDormant())
		return;

	if (filter && filter->ShouldHitCBaseEntity(pEntity, mask) == false)
		return;

	g_pEngineTrace->ClipRayToCBaseEntity(ray, mask | CONTENTS_HITBOX, pEntity, &playerTrace);
	if (playerTrace.fraction < smallestFraction)
	{
		*tr = playerTrace;
		smallestFraction = playerTrace.fraction;
	}
}

bool CAutowall::SimulateFireBullet(CBaseEntity* entity, CBaseEntity *local, CBaseCombatWeapon *weapon, FireBulletData &data)
{
	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	auto *wpn_data = weapon->GetCSWpnData();
	if (!wpn_data) return false;

	data.current_damage = static_cast<float>(wpn_data->damage);

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = wpn_data->range - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		GameUtils::UTIL_TraceLine(data.src, end, 0x4600400B, local, 0, &data.enter_trace);
		GameUtils::UTIL_ClipTraceToPlayers(entity, data.src, end + data.direction * 40.f, 0x4600400B, &data.filter, &data.enter_trace);

		if (data.enter_trace.fraction == 1.0f)
			break;

		if ((data.enter_trace.hitgroup <= 7)
			&& (data.enter_trace.hitgroup > 0)
			&& (local->GetTeamNum() != data.enter_trace.m_pEnt->GetTeamNum()))
		{
			data.trace_length += (float)(data.enter_trace.fraction * data.trace_length_remaining);
			data.current_damage *= (float)(pow(wpn_data->range_modifier, data.trace_length * 0.002));
			ScaleDamage(data.enter_trace.hitgroup, data.enter_trace.m_pEnt, wpn_data->armor_ratio, data.current_damage);

			return true;
		}

		if (!HandleBulletPenetration(wpn_data, data))
			break;
	}
	return false;
}

bool CAutowall::CanHit(CBaseEntity* entity, const Vector &point, float *damage_given)
{
	auto data = FireBulletData(G::LocalPlayer->GetEyePosition());
	data.filter = CTraceFilter();
	data.filter.pSkip = G::LocalPlayer;

	Vector angles = GameUtils::CalculateAngle(data.src, point);
	Math::AngleVectors(angles, &data.direction);
	VectorNormalize(data.direction);

	if (SimulateFireBullet(entity, G::LocalPlayer, G::LocalPlayer->GetWeapon(), data))
	{
		*damage_given = data.current_damage;
		return true;
	}

	return false;
}


bool CAutowall::CanWallBang(float& dmg)
{
	auto m_local = G::LocalPlayer;

	if (m_local && m_local->isAlive()) {
		FireBulletData data = FireBulletData(m_local->GetEyePosition());
		data.filter = CTraceFilter();
		data.filter.pSkip = m_local;
		Vector eye_angle; g_pEngine->GetViewAngles(eye_angle);
		Vector dst, forward;
		Math::AngleVectors(eye_angle, &forward);
		dst = data.src + (forward * 8196.f);
		Vector angles;
		Math::CalcAngle(data.src, dst, angles);
		Math::AngleVectors(angles, &data.direction);
		Math::NormalizeVector(data.direction);
		auto m_weapon = m_local->GetWeapon();
		if (m_weapon) {
			data.penetrate_count = 1;
			data.trace_length = 0.0f;
			CSWeaponInfo *weapon_data = m_weapon->GetCSWpnData();
			if (weapon_data) {
				data.current_damage = weapon_data->damage;
				data.trace_length_remaining = weapon_data->range - data.trace_length;
				Vector end = data.src + data.direction * data.trace_length_remaining;
				GameUtils::TraceLine(data.src, end, MASK_SHOT | CONTENTS_GRATE, m_local, &data.enter_trace);
				if (data.enter_trace.fraction == 1.0f) return false;

				if (HandleBulletPenetration(weapon_data, data))
				{
					dmg = data.current_damage;
					return true;
				}
			}
		}
	}

	return false;
}




#define CHAR_TEX_ANTLION		'A'
#define CHAR_TEX_BLOODYFLESH	'B'
#define	CHAR_TEX_CONCRETE		'C'
#define CHAR_TEX_DIRT			'D'
#define CHAR_TEX_EGGSHELL		'E' ///< the egg sacs in the tunnels in ep2.
#define CHAR_TEX_FLESH			'F'
#define CHAR_TEX_GRATE			'G'
#define CHAR_TEX_ALIENFLESH		'H'
#define CHAR_TEX_CLIP			'I'
//#define CHAR_TEX_UNUSED		'J'
#define CHAR_TEX_SNOW			'K'
#define CHAR_TEX_PLASTIC		'L'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_SAND			'N'
#define CHAR_TEX_FOLIAGE		'O'
#define CHAR_TEX_COMPUTER		'P'
//#define CHAR_TEX_UNUSED		'Q'
#define CHAR_TEX_REFLECTIVE		'R'
#define CHAR_TEX_SLOSH			'S'
#define CHAR_TEX_TILE			'T'
#define CHAR_TEX_CARDBOARD		'U'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_WOOD			'W'
//#define CHAR_TEX_UNUSED		'X' ///< do not use - "fake" materials use this (ladders, wading, clips, etc)
#define CHAR_TEX_GLASS			'Y'
#define CHAR_TEX_WARPSHIELD		'Z' ///< wierd-looking jello effect for advisor shield.

#define CHAR_TEX_STEAM_PIPE		11


#define enc_str(s) std::string(s)
#define enc_char(s) enc_str(s).c_str()
#define enc_wstr(s) std::wstring(enc_str(s).begin(), enc_str(s).end())
#define enc_wchar(s) enc_wstr(s).c_str()




DWORD dwClipTraceToPlayer;
void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{

	if (!dwClipTraceToPlayer)
		dwClipTraceToPlayer = FindPatternIDA("client_panorama.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10");


	_asm
	{
		MOV		EAX, filter
		LEA		ECX, tr
		PUSH	ECX
		PUSH	EAX
		PUSH	mask
		LEA		EDX, vecAbsEnd
		LEA		ECX, vecAbsStart
		CALL	dwClipTraceToPlayer
		ADD		ESP, 0xC
	}
}






Autowall autowall;

void Autowall::ScaleDamage(CBaseEntity* entity, CSWeaponInfo* weapon_info, int hitgroup, float& current_damage)
{
	//Cred. to N0xius for reversing this.
	//TODO: _xAE^; look into reversing this yourself sometime

	bool hasHeavyArmor = false;
	int armorValue = entity->GetArmor();

	//Fuck making a new function, lambda beste. ~ Does the person have armor on for the hitbox checked?
	auto IsArmored = [&entity, &hitgroup]()-> bool
	{
		CBaseEntity* targetEntity = entity;
		switch (hitgroup)
		{
		case HITGROUP_HEAD:
			return targetEntity->HasHelmet();
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			return true;
		default:
			return false;
		}
	};

	switch (hitgroup)
	{
	case HITGROUP_HEAD:
		current_damage *= hasHeavyArmor ? 2.f : 4.f; //Heavy Armor does 1/2 damage
		break;
	case HITGROUP_STOMACH:
		current_damage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		current_damage *= 0.75f;
		break;
	default:
		break;
	}

	if (armorValue > 0 && IsArmored())
	{
		float bonusValue = 1.f, armorBonusRatio = 0.5f, armorRatio = weapon_info->armor_ratio / 2.f;

		//Damage gets modified for heavy armor users
		if (hasHeavyArmor)
		{
			armorBonusRatio = 0.33f;
			armorRatio *= 0.5f;
			bonusValue = 0.33f;
		}

		auto NewDamage = current_damage * armorRatio;

		if (hasHeavyArmor)
			NewDamage *= 0.85f;

		if (((current_damage - (current_damage * armorRatio)) * (bonusValue * armorBonusRatio)) > armorValue)
			NewDamage = current_damage - (armorValue / armorBonusRatio);

		current_damage = NewDamage;
	}
}
bool Autowall::CanPenetrate(CBaseEntity* attacker, Autowall_Info& info, CSWeaponInfo* weapon_data)
{
	if (!weapon_data || !attacker)
		return false;

	if (!this)
		return false;

	//typedef bool(__thiscall* HandleBulletPenetrationFn)(SDK::CBaseEntity*, float&, int&, int*, SDK::trace_t*, Vector*, float, float, float, int, int, float, int*, Vector*, float, float, float*);
	//CBaseEntity *pLocalPlayer, float *flPenetration, int *SurfaceMaterial, char *IsSolid, trace_t *ray, Vector *vecDir, int unused, float flPenetrationModifier, float flDamageModifier, int unused2, int weaponmask, float flPenetration2, int *hitsleft, Vector *ResultPos, int unused3, int unused4, float *damage)
	typedef bool(__thiscall* HandleBulletPenetrationFn)(CBaseEntity*, float*, int*, char*, trace_t*, Vector*, int, float, float, int, int, float, int*, Vector*, int, int, float*);
	static HandleBulletPenetrationFn HBP = reinterpret_cast<HandleBulletPenetrationFn>(Utilities::Memory::FindPattern("client_panorama.dll",
		(PBYTE)"\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF8\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x83\xEC\x78\x8B\x53\x14",
		"xxxxxxxxxxxxxxxxxxxxxxxxxxxx"));

	auto enter_surface_data = g_pPhysics->GetSurfaceData(info.enter_trace.surface.surfaceProps);
	if (!enter_surface_data)
		return true;

	char is_solid = 0;
	int material = enter_surface_data->game.material;
	int mask = 0x1002;
	ClientClass * pClass = (ClientClass*)info.enter_trace.m_pEnt->GetClientClass();

	// glass and shit gg
	if (info.enter_trace.m_pEnt && !strcmp(enc_char("CBreakableSurface"),
		pClass->m_pNetworkName))
		*reinterpret_cast<byte*>(uintptr_t(info.enter_trace.m_pEnt + 0x27C)) = 2;

	is_autowalling = true;
	bool return_value = !HBP(attacker, &weapon_data->penetration, &material, &is_solid, &info.enter_trace, &info.direction, 0, enter_surface_data->game.flPenetrationModifier, enter_surface_data->game.flDamageModifier, 0, mask, weapon_data->penetration, &info.penetration_count, &info.current_position, 0, 0, &info.current_damage);
	is_autowalling = false;
	return return_value;
}
Autowall::Autowall_Return_Info Autowall::CalculateDamage(Vector start, Vector end, CBaseEntity* from_entity, CBaseEntity* to_entity, int specific_hitgroup)
{
	// default values for return info, in case we need to return abruptly
	Autowall_Return_Info return_info;
	return_info.damage = -1;
	return_info.hitgroup = -1;
	return_info.hit_entity = nullptr;
	return_info.penetration_count = 4;
	return_info.thickness = 0.f;
	return_info.did_penetrate_wall = false;

	Autowall_Info autowall_info;
	autowall_info.penetration_count = 4;
	autowall_info.start = start;
	autowall_info.end = end;
	autowall_info.current_position = start;
	autowall_info.thickness = 0.f;

	// direction 
	Math::AngleVectors(GameUtils::CalculateAngle(start, end), &autowall_info.direction);

	// attacking entity
	if (!from_entity)
		from_entity = G::LocalPlayer;
	if (!from_entity)
		return return_info;

	auto filter_player = CTraceFilter1();
	filter_player.pEntity = to_entity;
	auto filter_local = CTraceFilter();
	filter_local.pSkip = from_entity;

	// setup filters
	if (to_entity)
		autowall_info.filter = &filter_player;
	else
		autowall_info.filter = &filter_player;

	// weapon
	auto weapon = reinterpret_cast<CBaseCombatWeapon*>(g_pEntitylist->GetClientEntity(from_entity->GetActiveWeaponIndex()));
	if (!weapon)
		return return_info;

	// weapon data
	auto weapon_info = weapon->GetCSWpnData();
	if (!weapon_info)
		return return_info;

	// client class
	auto weapon_client_class = reinterpret_cast<CBaseEntity*>(weapon)->GetClientClass();
	if (!weapon_client_class)
		return return_info;

	// weapon range
	float range = min(weapon_info->range, (start - end).Length());
	end = start + (autowall_info.direction * range);
	autowall_info.current_damage = weapon_info->damage;

	while (autowall_info.current_damage > 0 && autowall_info.penetration_count > 0)
	{
		return_info.penetration_count = autowall_info.penetration_count;

		GameUtils::TraceLine(autowall_info.current_position, end, MASK_SHOT | CONTENTS_GRATE, from_entity, &autowall_info.enter_trace);
		UTIL_ClipTraceToPlayers(autowall_info.current_position, autowall_info.current_position + autowall_info.direction * 40.f, MASK_SHOT | CONTENTS_GRATE, autowall_info.filter, &autowall_info.enter_trace);

		const float distance_traced = (autowall_info.enter_trace.endpos - start).Length();
		autowall_info.current_damage *= pow(weapon_info->range_modifier, (distance_traced / 500.f));

		/// if reached the end
		if (autowall_info.enter_trace.fraction == 1.f)
		{
			if (to_entity && specific_hitgroup != -1)
			{
				ScaleDamage(to_entity, weapon_info, specific_hitgroup, autowall_info.current_damage);

				return_info.damage = autowall_info.current_damage;
				return_info.hitgroup = specific_hitgroup;
				return_info.end = autowall_info.enter_trace.endpos;
				return_info.hit_entity = to_entity;
			}
			else
			{
				return_info.damage = autowall_info.current_damage;
				return_info.hitgroup = -1;
				return_info.end = autowall_info.enter_trace.endpos;
				return_info.hit_entity = nullptr;
			}

			break;
		}
		// if hit an entity
		if (autowall_info.enter_trace.hitgroup > 0 && autowall_info.enter_trace.hitgroup <= 7 && autowall_info.enter_trace.m_pEnt)
		{
			// checkles gg
			if ((to_entity && autowall_info.enter_trace.m_pEnt != to_entity) ||
				(autowall_info.enter_trace.m_pEnt->GetTeamNum() == from_entity->GetTeamNum()))
			{
				return_info.damage = -1;
				return return_info;
			}

			if (specific_hitgroup != -1)
				ScaleDamage(autowall_info.enter_trace.m_pEnt, weapon_info, specific_hitgroup, autowall_info.current_damage);
			else
				ScaleDamage(autowall_info.enter_trace.m_pEnt, weapon_info, autowall_info.enter_trace.hitgroup, autowall_info.current_damage);

			// fill the return info
			return_info.damage = autowall_info.current_damage;
			return_info.hitgroup = autowall_info.enter_trace.hitgroup;
			return_info.end = autowall_info.enter_trace.endpos;
			return_info.hit_entity = autowall_info.enter_trace.m_pEnt;

			break;
		}

		// break out of the loop retard
		if (!CanPenetrate(from_entity, autowall_info, weapon_info))
			break;

		return_info.did_penetrate_wall = true;
	}

	return_info.penetration_count = autowall_info.penetration_count;

	return return_info;
}
