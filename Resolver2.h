#pragma once
#include <deque>

class PerfectResolver
{

public:
	struct ResolverData
	{
		//ints
		int right_damage = 0, left_damage = 0;
		int stored_missed;
		int missed;
		int shotaimangles;
		//ints

		//floats
		float lastlbystand;
		float right_fraction = 0.f, left_fraction = 0.f;
		float last_moving_lby;
		float stored_lby;
		float next_lby_update;
		float lby1, nextlbyup;
		float lastmovinglby;
		float flick;
		//floats

		//bools
		bool is_moving;
		bool is_jumping;
		bool is_crouching;
		bool using_fake_angles;
		bool bfakeangle, bfakewalk, playerhurtcalled, weaponfirecalled;
		bool lbywasupdated;
		//bools

		//vectors
		AnimationLayer anim_layers[15];
		//vectors


	} pResolverData[64];


	bool AntiFreestanding(CBaseEntity * entity, float & yaw);

	void DoCM();

	bool IsYawSideways(CBaseEntity * entity, float yaw);

	void DoResolver();

};
extern PerfectResolver* set;