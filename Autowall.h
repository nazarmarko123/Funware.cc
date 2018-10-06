#pragma once
struct FireBulletData
{
	FireBulletData(const Vector &eye_pos)
		: src(eye_pos)
	{
	}

	Vector           src;
	trace_t          enter_trace;
	Vector           direction;
	CTraceFilter    filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;
};

class CAutowall {
public:
	//Old Awall
	bool TraceToExit(Vector& end, trace_t* enter_trace, Vector start, Vector dir, trace_t* exit_trace);

	bool HandleBulletPenetration(CSWeaponInfo* pWeaponData, FireBulletData& BulletData);

	bool SimulateFireBullet(CBaseEntity* entity, CBaseEntity* local, CBaseCombatWeapon* weapon, FireBulletData& data);

	bool CanHit(CBaseEntity* entity, const Vector& point, float* damage_given);

	bool CanWallBang(float & dmg);

};

extern CAutowall* g_Autowall;

extern void ScaleDamage(int hitgroup, CBaseEntity* enemy, float weapon_armor_ratio, float& current_damage);




class Autowall
{
public:
	struct Autowall_Return_Info
	{
		int damage;
		int hitgroup;
		int penetration_count;
		bool did_penetrate_wall;
		float thickness;
		Vector end;
		CBaseEntity* hit_entity;
	};
	struct Autowall_Info
	{
		Vector start;
		Vector end;
		Vector current_position;
		Vector direction;

		ITraceFilter* filter;
		trace_t enter_trace;

		float thickness;
		float current_damage;
		int penetration_count;
	};
	void ScaleDamage(CBaseEntity * entity, CSWeaponInfo * weapon_info, int hitgroup, float & current_damage);
	Autowall_Return_Info CalculateDamage(Vector start, Vector end, CBaseEntity* from_entity = nullptr, CBaseEntity* to_entity = nullptr, int specific_hitgroup = -1);

	bool CanPenetrate(CBaseEntity* attacker, Autowall_Info& info, CSWeaponInfo* weapon_data);
	inline bool IsAutowalling() const
	{
		return is_autowalling;
	}
private:
	bool is_autowalling = false;
};

extern Autowall autowall;

