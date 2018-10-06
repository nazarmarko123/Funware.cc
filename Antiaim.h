#pragma once

class CAntiaim {
public:
	bool choke;
	bool lby_update_in_this_tick;
	float lby_updatetime;
	void Pitch_AA();
	void Run(QAngle org_view);
	void Fakewalk(CUserCmd* userCMD);
	void Friction(Vector& outVel);
	void AntiAimYaw_Real();
	void AntiAimYaw_Fake();
	void gaywalk(CUserCmd* userCMD);
};

extern CAntiaim* g_Antiaim;







namespace FEATURES
{
	namespace RAGEBOT
	{
		class Antiaim
		{
		public:
			void Do(CUserCmd* cmd);
			void UpdateLBY();

			float TimeUntilNextLBYUpdate()
			{
				return m_next_lby_update_time - GetCurtime();
			}

		private:
			float m_next_lby_update_time = 0.f, m_last_move_time = 0.f, m_last_attempted_lby = 0.f;
			bool m_will_lby_update = false;

		private:
			float DoYaw();
			float DoPitch();

			bool DoEdging(float& yaw, float& lby);

			float BreakLBY(float real_yaw, float desired_lby, bool double_flick = false);
		};

		extern Antiaim antiaim;
	}
}
