#include "NewEventLog.h"

#include "../Draw.h"




namespace FEATURES
{
	namespace MISC
	{
		InGameLogger in_game_logger;
		void ColorLine::Draw(int x, int y, unsigned int font)
		{
			for (int i = 0; i < texts.size(); i++)
			{
				g_Draw.DrawF(x, y, font, false, false, colors[i], texts[i]);
				x += g_Draw.GetTextSize(font, texts[i]).x;
			}
		}

		void InGameLogger::Do()
		{
			if (log_queue.size() > max_lines_at_once)
				log_queue.erase(log_queue.begin() + max_lines_at_once, log_queue.end());

			for (int i = 0; i < log_queue.size(); i++)
			{
				auto log = log_queue[i];
				float time_delta = fabs(GetCurtime() - log.time);

				int height = ideal_height + (16 * i);

				/// erase dead logs
				if (time_delta > text_time)
				{
					log_queue.erase(log_queue.begin() + i);
					break;
				}
				if (time_delta > text_time - slide_out_speed)
					height = height + (((slide_out_speed - (text_time - time_delta)) / slide_out_speed) * slide_out_distance);

				/// fade out
				if (time_delta > text_time - text_fade_out_time)
					log.color_line.ChangeAlpha(255 - (((time_delta - (text_time - text_fade_out_time)) / text_fade_out_time) * 255.f));
				/// fade in
				if (time_delta < text_fade_in_time)
					log.color_line.ChangeAlpha((time_delta / text_fade_in_time) * 255.f);

				int width = ideal_width;

				/// slide from left
				if (time_delta < text_fade_in_time)
					width = (time_delta / text_fade_in_time) * static_cast<float>(slide_in_distance) + (ideal_width - slide_in_distance);
				/// slider from right
				if (time_delta > text_time - text_fade_out_time)
					width = ideal_width + (((time_delta - (text_time - text_fade_out_time)) / text_fade_out_time) * static_cast<float>(slide_out_distance));

				
				log.color_line.Draw(width, height, g_Draw.in_game_logging_font);
				
			}
		}
	}
}

namespace FEATURES
{
	namespace MISC
	{
		CREATE_EVENT_LISTENER(ItemPurchaseListener, "item_purchase");

		CREATE_EVENT_LISTENER(PlayerHurtListener, "player_hurt");

		CREATE_EVENT_LISTENER(BulletImpactListener, "bullet_impact");

		CREATE_EVENT_LISTENER(WeaponFireListener, "weapon_fire");

		CREATE_EVENT_LISTENER(RoundStartListener, "round_start");


		ItemPurchaseListener item_purchase_listener;
		PlayerHurtListener player_hurt_listener;
		BulletImpactListener bullet_impact_listener;
		WeaponFireListener weapon_fire_listener;
		RoundStartListener round_start_listener;

		void InitializeEventListeners()
		{

			g_pGameEventManager->AddListener(&item_purchase_listener, ("item_purchase"), false);
			g_pGameEventManager->AddListener(&player_hurt_listener, ("player_hurt"), false);
			g_pGameEventManager->AddListener(&bullet_impact_listener, ("bullet_impact"), false);
			g_pGameEventManager->AddListener(&weapon_fire_listener, ("weapon_fire"), false);
			g_pGameEventManager->AddListener(&round_start_listener, ("round_start"), false);

		}

		void RemoveEventListeners()
		{
			g_pGameEventManager->RemoveListener(&item_purchase_listener);
			g_pGameEventManager->RemoveListener(&player_hurt_listener);
			g_pGameEventManager->RemoveListener(&bullet_impact_listener);
			g_pGameEventManager->RemoveListener(&weapon_fire_listener);
			g_pGameEventManager->RemoveListener(&round_start_listener);

		}

		void ItemPurchaseListener::FireGameEvent(IGameEvent* game_event)
		{
			if (!game_event)
				return;

			CBaseEntity* local_player = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
			if (!local_player)
				return;

			CBaseEntity* entity = g_pEntitylist->GetClientEntity(g_pEngine->GetPlayerForUserID(game_event->GetInt("userid")));
			if (!entity)
				return;

			if (entity->GetTeamNum() == local_player->GetTeamNum())
				return;

			player_info_t player_info;
			if (!g_pEngine->GetPlayerInfo(entity->GetIndex(), &player_info))
				return;

			MISC::InGameLogger::Log log;

			log.color_line.PushBack("[Azuhook] ", Color(255, 20, 147,255));
			log.color_line.PushBack(player_info.m_szPlayerName, Color(199, 21, 133,255));
			log.color_line.PushBack(" bought a ", Color(219, 112, 147, 255));
			log.color_line.PushBack(game_event->GetString("weapon"), Color(255, 105, 180, 255));

			MISC::in_game_logger.AddLog(log);
		};

		void xdlolkek(IGameEvent* event)
		{

			if (!g_pEngine->IsInGame())
				return;


			if (!event || strcmp(event->GetName(), XorStr("player_death")) != 0)
				return;

			if (!event->GetInt(XorStr("attacker")) || g_pEngine->GetPlayerForUserID(event->GetInt(XorStr("attacker"))) != g_pEngine->GetLocalPlayer())
				return;

			std::string weapon = event->GetString(XorStr("weapon"));

			std::string wpn = weapon;
			auto local_player = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
			std::string weaponmy = local_player->GetTeamNum() == 2 ? XorStr("knife_t") : XorStr("knife_default_ct");

			if (wpn == XorStr("knife_default_ct") || wpn == XorStr("knife_t") || wpn == XorStr("bayonet") || wpn == XorStr("knife_push") || wpn == XorStr("knife_butterfly") || wpn == XorStr("knife_survival_bowie") || wpn == XorStr("knife_falchion") || wpn == XorStr("knife_tactical") || wpn == XorStr("knife_m9_bayonet") || wpn == XorStr("knife_karambit") || wpn == XorStr("knife_gut") || wpn == XorStr("knife_flip") || wpn == XorStr("knife_gypsy_jackknife") || wpn == XorStr("knife_stiletto") || wpn == XorStr("knife_ursus") || wpn == XorStr("knife_widowmaker"))
			{
				
				switch (*local_player->GetWeapon()->fixskins())
				{
				case 500:
					weaponmy = XorStr("bayonet");
					break;
				case 505:
					weaponmy = XorStr("knife_flip");
					break;
				case 506:
					weaponmy = XorStr("knife_gut");
					break;
				case 507:
					weaponmy = XorStr("knife_karambit");
					break;
				case 508:
					weaponmy = XorStr("knife_m9_bayonet");
					break;
				case 509:
					weaponmy = XorStr("knife_tactical");
					break;
				case 512:
					weaponmy = XorStr("knife_falchion");
					break;
					//case i dont fucking know:
					//	weaponmy = XorStr("knife_survival_bowie");
					//	break;
				case 515:
					weaponmy = XorStr("knife_butterfly");
					break;
				case 516:
					weaponmy = XorStr("knife_push");
					break;
				case 520:
					weaponmy = XorStr("knife_gypsy_jackknife");
					break;
				case 522:
					weaponmy = XorStr("knife_stiletto");
					break;
				case 519:
					weaponmy = XorStr("knife_ursus");
					break;
				case 523:
					weaponmy = XorStr("knife_widowmaker");
					break;
				}

				if (weapon != weaponmy)
					event->SetString(XorStr("weapon"), weaponmy.c_str());
					
			}
		}




		void PlayerHurtListener::FireGameEvent(IGameEvent* game_event)
		{
			if (!game_event)
				return;


			//FEATURES::RAGEBOT::resolver.ProcessSnapShots();
			CBaseEntity* local_player = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
			if (!local_player)
				return;



			xdlolkek(game_event);


			CBaseEntity* attacker = g_pEntitylist->GetClientEntity(g_pEngine->GetPlayerForUserID(game_event->GetInt("attacker")));
			if (!attacker)
				return;

			if (attacker != local_player)
				return;

			player_info_t player_info;
			g_pEngine->GetPlayerInfo(g_pEngine->GetPlayerForUserID(game_event->GetInt("userid")), &player_info);

			MISC::InGameLogger::Log log;
			log.color_line.PushBack("[Azuhook] ", Color(255, 20, 147, 255));
			log.color_line.PushBack("Hit ", Color(219, 112, 147, 255));
			log.color_line.PushBack(player_info.m_szPlayerName, Color(199, 21, 133, 255));
			
			int hitgroup = game_event->GetInt("hitgroup");
			if (hitgroup > 0 && hitgroup <= 7)
			{
				log.color_line.PushBack(" in the ", Color(219, 112, 147, 255));
				log.color_line.PushBack(GetHitgroupName(hitgroup), Color(219, 112, 147, 255));
			}

			log.color_line.PushBack(" for ", Color(219, 112, 147, 255));
			log.color_line.PushBack(std::to_string(game_event->GetInt("dmg_health")), Color(219, 112, 147, 255));
			log.color_line.PushBack(" damage.", Color(219, 112, 147, 255));

			MISC::in_game_logger.AddLog(log);
		};

		void BulletImpactListener::FireGameEvent(IGameEvent* game_event)
		{
			if (!game_event)
				return;



			CBaseEntity* entity = g_pEntitylist->GetClientEntity(g_pEngine->GetPlayerForUserID(game_event->GetInt("userid")));
			if (!entity)
				return;

			Vector end = Vector(game_event->GetFloat("x"), game_event->GetFloat("y"), game_event->GetFloat("z"));
			Vector start = TraceToEnd(end, entity->GetVecOrigin() + entity->GetViewOffset());

		}

		void WeaponFireListener::FireGameEvent(IGameEvent* game_event)
		{
			if (!game_event)
				return;


		}

		void RoundStartListener::FireGameEvent(IGameEvent* game_event)
		{
			if (!game_event)
				return;

			if (!(g_pEngine->IsConnected() && g_pEngine->IsInGame()))
				return;

			CBaseEntity* local_player = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
			if (!local_player)
				return;

			/*
			if (!Clientvariables->Buybot.buybot_enabled)
			return;

			switch (Clientvariables->Buybot.buybot_rifle)
			{
			case 1:
				g_pEngine->ClientCmd("buy scar20;buy g3sg1");
			break;
			case 2:
				g_pEngine->ClientCmd("buy ssg08");
			break;
			case 3:
				g_pEngine->ClientCmd("buy awp");
			break;
			default:
			break;
			}

			switch (Clientvariables->Buybot.buybot_pistol)
			{
			case 1:
				g_pEngine->ClientCmd("buy deagle");
			break;
			case 2:
				g_pEngine->ClientCmd("buy elite");
			break;
			default:
			break;
			}

			switch (Clientvariables->Buybot.buybot_armor)
			{
			case 1:
				g_pEngine->ClientCmd("buy vest");
			break;
			case 2:
				g_pEngine->ClientCmd("buy vesthelm");
			break;
			default:
			break;
			}

			if (Clientvariables->Buybot.buybot_grenade0) /// HG
			{
				g_pEngine->ClientCmd("buy hegrenade");
			}
			if (Clientvariables->Buybot.buybot_grenade1) /// Molotov
			{
				g_pEngine->ClientCmd("buy molotov; buy incgrenade");
			}
			if (Clientvariables->Buybot.buybot_grenade2) /// Smoke
			{
				g_pEngine->ClientCmd("buy smokegrenade");
			}
			if (Clientvariables->Buybot.buybot_grenade3) /// Flash
			{
				g_pEngine->ClientCmd("buy flashbang");
			}
			if (Clientvariables->Buybot.buybot_grenade4) /// Decoy
			{
				g_pEngine->ClientCmd("buy decoy");
			}

			if (Clientvariables->Buybot.buybot_zeus)
				g_pEngine->ClientCmd("buy taser 34");

				*/
		}
	}
}