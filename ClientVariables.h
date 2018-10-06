#pragma once

class ClientVariables {
public:
	struct Ragebot {
		bool EnableAimbot = false;
		int AimbotSelection = 0;
		bool AutomaticFire = false;
		bool AutomaticScope = false;
		bool SilentAimbot = false;
		bool AutoRevolver = false;
		bool EnginePred = false;
		int EnginePred1 = 0;
		bool NoRecoil = false;
		bool NoSpread = false;
		bool PositionAdjustment = false;
		int Preferbodyaim = 0;
		int Hitbox = 0;
		int Hitscan = 0;
		bool AutowallHitscan = false;
		bool Autowall = false;
		int Multipoint = 0.f;
		bool enablescale = false;
		float Headscale = 0.0f;
		bool PreferBody = false;
		bool PreferBodyEnable = false;
		float Bodyscale = 0.0f;
		int Fov = 0.f;
		int Mindamage = 1.f;
		bool enablemindmg = false;
		bool Hitchance = false;
		int Minhitchance = 0.f;
		bool hitchanceenable = false;
		bool AutomaticResolver = false;
		bool AutomaticResolver2 = false;
		int ResolverStepAngle = 25;
		int fakewalk_speed = 0;
		bool FriendlyFire = false;
		bool Quickstop = false;


		bool HeadH = false;
		bool NeckH = false;
		bool BodyH = false;
		bool ArmsH = false;
		bool LegsH = false;
		bool FootsH = false;

		bool perfectresolver;
	} Ragebot;

	struct Weaponxdfix
	{
		

		int AK47hit;
		int AK47dmg;

		int Galilhit;
		int Galildmg;


		int M4A1Shit;
		int M4A1Sdmg;

		int M4A4hit;
		int M4A4dmg;

		
		int AUGhit;
		int AUGdmg;


		int FAMAShit;
		int AWPhit;
		int SSG08hit;
		int SCAR20hit;
		int P90hit;
		int Mp7hit;
		int Novahit;
		int UMP45hit;
		int Glockhit;
		int Sawedhit;
		int USPhit;
		int Maghit;
		int Xmhit;
		int Deaglehit;
		int Dualhit;
		int Fivehit;
		int Revolverhit;
		int Mac10hit;
		int tec9hit;
		int Cz75hit;
		int Negevhit;
		int M249hit;
		int Mp9hit;
		int P2000hit;
		int Bizonhit;
		int Sg553hit;
		int P250hit;
		int G3sg1hit;



		int FAMASdmg;
		int AWPdmg;
		int SSG08dmg;
		int SCAR20dmg;
		int P90dmg;
		int Mp7dmg;
		int Novadmg;
		int UMP45dmg;
		int Glockdmg;
		int Saweddmg;
		int USPdmg;
		int Magdmg;
		int Xmdmg;
		int Deagledmg;
		int Dualdmg;
		int Fivedmg;
		int Revolverdmg;
		int Mac10dmg;
		int tec9dmg;
		int Cz75dmg;
		int Negevdmg;
		int M249dmg;
		int Mp9dmg;
		int P2000dmg;
		int Bizondmg;
		int Sg553dmg;
		int P250dmg;
		int G3sg1dmg;


	}Weaponxdfix;


	struct RankChanger
	{
		bool enabled;
		bool semiinventar;
		int cmd_friendly;
		int cmd_teaching;
		int cmd_leader;
		int rank_id;
		int wins;
		int player_level;
	}RankChanger;


#define STICKERS_COUNT 5
	class Weapon_tTT
	{
	public:
		int SkinsWeapon;
		int SkinsKnife;
		int VremennyiWeapon;
		int VremennyiKnife;
		int Stikers1;
		int Stikers2;
		int Stikers3;
		int Stikers4;
		float ChangerWear = 0;
		int ChangerStatTrak = 0;
		char ChangerName[32] = "";
		bool ChangerEnabled;
	};
	class Sticker_t
	{
	public:
		int   iID = 0;
		float flWear = 0.f;
		float flScale = 1.f;
		int   iRotation = 0;
	};
	struct NewSkinchanger
	{
		Weapon_tTT weapons[520];


		struct wpnz
		{
			bool  EnabledStickers;
			bool  EnabledSkins;
			int   ChangerSkin;
			char  ChangerName[128] = "";
			int   ChangerStatTrak;
			int   ChangerSeed;
			float ChangerWear;
			Sticker_t Stickers[STICKERS_COUNT];

		}Weapons[519];





		bool glovesenabled;
		float glovewear;
		int gloves;
		int skingloves;

		bool enabled;
		int Knife;
		int KnifeSkin;

		



		char* customname;
		int AK47Skin;
		int GalilSkin;
		int M4A1SSkin;
		int M4A4Skin;
		int AUGSkin;
		int FAMASSkin;
		int AWPSkin;
		int SSG08Skin;
		int SCAR20Skin;
		int P90Skin;
		int Mp7Skin;
		int NovaSkin;
		int UMP45Skin;
		int GlockSkin;
		int SawedSkin;
		int USPSkin;
		int MagSkin;
		int XmSkin;
		int DeagleSkin;
		int DualSkin;
		int FiveSkin;
		int RevolverSkin;
		int Mac10Skin;
		int tec9Skin;
		int Cz75Skin;
		int NegevSkin;
		int M249Skin;
		int Mp9Skin;
		int P2000Skin;
		int BizonSkin;
		int Sg553Skin;
		int P250Skin;
		int G3sg1Skin;


	} NewSkinchanger;


	struct Antiaim {
		bool AntiaimEnable = false;
		bool Manual = false;
		int Pitch = 0;

		int Yaw = 0;
		int Air = 0;
		int YawAdd = 0;
		int AirAdd = 0;
		int YawRunning = 0;
		int YawRunningAdd = 0;
		int FakeYaw = 0;
		int FakeYawAdd = 0;
		int jitter_range = 0;
		int AntiResolver = 0;

		int Fakewalk = 0;

		int Freestanding = 0;
		int FreestandingDelta = 0;


		int AtPlayer = 0;
		bool Disableaa = false;
		int Jitterrange = 0.f;
		bool Randomizejitter = false;
		int AntiaimMode = 0;
	} Antiaim;



	struct NewAntiAim {


		bool antiaim_enabled;

		bool antiaim_standing_at_target_enabled = false;
		int antiaim_standing_pitch = 0;
		/// Real
		int antiaim_standing_real_yaw = 0;
		float antiaim_standing_real_offset = 0.f;
		float antiaim_standing_real_lby_delta = 0.f;
		float antiaim_standing_real_jrange = 0.f;
		float antiaim_standing_real_spin_speed = 0.f;
		/// Fake
		int antiaim_standing_fake_yaw = 0;
		float antiaim_standing_fake_offset = 0.f;
		float antiaim_standing_fake_jrange = 0.f;
		float antiaim_standing_fake_spin_speed = 0.f;

		/* Moving */
		bool antiaim_moving_at_target_enabled = false;
		int antiaim_moving_pitch = 0;
		/// Real
		int antiaim_moving_real_yaw = 0;
		float antiaim_moving_real_offset = 0.f;
		float antiaim_moving_real_jrange = 0.f;
		float antiaim_moving_real_spin_speed = 0.f;
		/// Fake
		int antiaim_moving_fake_yaw = 0;
		float antiaim_moving_fake_offset = 0.f;
		float antiaim_moving_fake_jrange = 0.f;
		float antiaim_moving_fake_spin_speed = 0.f;

		/* Jumping */
		bool antiaim_jumping_at_target_enabled = false;
		int antiaim_jumping_pitch = 0;
		/// Real
		int antiaim_jumping_real_yaw = 0;
		float antiaim_jumping_real_offset = 0.f;
		float antiaim_jumping_real_jrange = 0.f;
		float antiaim_jumping_real_spin_speed = 0.f;
		/// Fake
		int antiaim_jumping_fake_yaw = 0;
		float antiaim_jumping_fake_offset = 0.f;
		float antiaim_jumping_fake_jrange = 0.f;
		float antiaim_jumping_fake_spin_speed = 0.f;

		/* Edging */
		int antiaim_edging_type = 0;
		float antiaim_edging_lby_delta = 0.f;
		float antiaim_edging_jrange = 0.f;
		bool antiaim_edging_standing_enabled = false;
		bool antiaim_edging_moving_enabled = false;
		bool antiaim_edging_jumping_enabled = false;
		
	} NewAntiAim;



	struct Visuals {

		bool PreserveKillfeed;
		bool fakeheadshot;
		bool eventlog;
		bool SpectatorList;

		float Opacity = 0.0f;
		float Opacity_2 = 0.0f;
		bool EspEnable = false;
		bool EnemyOnly = false;
		bool BoundingBox = false;
		bool Bones = false;
		bool aimware_hitmarker = true;
		int aimware_hitmarker_color = 0;
		float aimwarehitmarkercolor[3];
		bool Health = false;
		int HealthType;
		bool Armor = false;
		bool Flags = false;
		bool Bonesbacktrack = false;
		bool Fake = false;
		bool Dlight = false;
		bool Name = false;
		bool Weapon = false;
		bool Icon = false;
		bool Ammo = false;
		bool AllItems = false;
		bool Rank = false;
		bool Wins = false;
		bool Glow = false;
		bool pulseglow;
		bool LineofSight = false;
		bool SnapLines = false;
		bool GrenadePrediction = false;
		int Crosshair = 0;
		bool SpreadCrosshair = false;
		bool RecoilCrosshair = false;
		bool FartherESP = false;
		int hitmarker_size = 1.f;

		//Cbase/filers
		int DroppedWeapons = 0;
		bool Hostage = false;
		bool ThrownNades = false;
		bool LocalPlayer = false;
		bool LocalChams = false;
		bool LocalGlow = false;
		bool BulletTracers = false;
		bool Bullet_impacts = false;
		bool DrawBeamCircle = false;
		bool Teammates = false;
		bool Bomb = false;
		bool Spectators = false;
		bool DamageIndicators = false;
		float lvmodel_col[3] = { 0.f, 1.f, 0.f };
		//Effects/world
		bool OutOfPOVArrows = false;
		bool skycolor;
		bool fakechams;
		float lineFakeAngle;
		bool worldcolor;
		bool rifk_arrows;
		bool nightmode = false;
		int Skybox = 0;
		int FlashbangAlpha = 0;
		bool Nosmoke = false;
		bool Noscope = false;
		bool RemoveParticles = false;
		bool Novisrevoil = false;
		int Hitmarker;
		bool ChamsEnable = true;
		bool removeparticles2 = false;
		int ChamsStyle = 0;
		//	int FakeAngleGhost = 0;
		bool FakeAngleGhost = false;
		bool WeaponMaterials = false;
		bool Chamsenemyonly = false;
		bool metallicchams = false;
		bool LocalChams1 = false;
		bool ChamsPlayer = false;
		bool ChamsPlayerWall = false;
		bool ChamsHands = false;
		bool ChamsHandsWireframe = false;
		bool WeaponWireframe = false;
		bool WeaponChams = false;
		int stylexd;
		bool TransparensWallsEnable = false;
		bool WeaponEnableChams = false;
		bool HandEnableChams = false;
		bool test = false;
		bool test1 = false;
		bool test2 = false;
		bool test3 = false;

	} Visuals;

	struct Misc
	{
		int Rightside = 0;
		int Leftside = 0;
		int Backside = 0;
		bool AntiUT = true;
		bool AntiUTxd = false;
		int PlayerFOV = 0.f;
		int PlayerViewmodel = 0.f;
		int TPangles = 0;
		int TPKey = 0;
		int MenuKey = 0x2d;
		int WalkbotSet = 0x2d;
		int WalkbotDelete = 0x2d;
		int WalkbotStart = 0x2d;
		bool AutoJump = false;
		bool AutoStrafe = false;
		bool AutoAccept = false;
		bool Prespeed = false;
		int Retrack = 0.f;
		int PrespeedKey = 0;
		bool FakelagEnable = false;
		bool FakelagOnground = false;
		int FakelagMode = 0;
		int FakelagAmount = 0.f;
		bool FakelagShoot = false;
		int ConfigSelection = 0;
		bool Walkbot = false;
		bool WalkbotBunnyhop = false;
		//int WalkbotSetPoint = 0;
		//int WalkbotDeletePoint = 0;
		//int WalkbotStart = 0;
		bool FakewalkEnable = false;
		int FakewalkKey = 0;
		bool geywalk = false;
		bool enablemanual = false;
		bool buybot = false;
		int buybot2;
		bool buybot3 = false;
		bool clantag = false;
		int fakewalk_speed = 0;
		bool lagfix;
	} Misc;


	struct CPlayerlist {
		bool bEnabled;
		int iPlayer;
		char* szPlayers[64] = {
			" ", " ", " ", " ", " ", " ", " ", " ", " ",
			" ", " ", " ", " ", " ", " ", " ", " ", " ",
			" ", " ", " ", " ", " ", " ", " ", " ", " ",
			" ", " ", " ", " ", " ", " ", " ", " ", " ",
			" ", " ", " ", " ", " ", " ", " ", " ", " ",
			" ", " ", " ", " ", " ", " ", " ", " ", " ",
			" ", " ", " ", " ", " ", " ", " ", " ", " ",
			" "
		};
	} Playerlist;

	struct NigColors {
		float MenuColor[3] = { 1.f, 1.f, 1.f };
		float BoundingBox[3] = { 0.f, 1.f, 0.f };

		float PlayerChams[3] = { 0.f, 1.f, 0.f };


		float PlayerChamsWall[3] = { 1.f, 0.f, 0.f };
		float LocalChams[3] = { 0.f, 1.f, 0.f };
		float Skeletons[3] = { 1.f, 1.f, 1.f };
		float SelfChams[3] = { 0.f, 0.f, 1.f };
		float Bulletracer[3] = { 1.f, 0.f, 0.f };
		float bullet_impacts[3] = { 1.f, 0.f, 0.f };
		float aw_hitmarker[3] = { 1.f, 0.f, 0.f };
		float ArmorColor[3] = { 0.f, 0.f, 1.f };

		float WireframeHand[3] = { 1.f, 1.f, 1.f };
		float ChamsHand[3] = { 1.f, 1.f, 1.f };
		float ChamsWeapon[3] = { 1.f, 1.f, 1.f };
		float WireframeWeapon[3] = { 1.f, 1.f, 1.f };
		float CrystalColor[3] = { 1.f, 1.f, 1.f };

		float Glow[3] = { 1.f, 0.f, 0.f };
		float LocalGlow[3] = { 1.f, 0.f, 0.f };
		float DroppedWeapon[3] = { 1.f, 1.f, 1.f };
		float Bomb[3] = { 1.f, 1.f, 1.f };
		float PlantedBomb[3] = { 1.f, 1.f, 1.f };
		float Hostage[3] = { 1.f, 1.f, 1.f };
		float GrenadePrediction[3] = { 0.f, 1.f, 0.f };
		float FakeAngleGhost[3] = { 1.f, 1.f, 1.f };
		float SpreadCrosshair[3] = { 0.30f,1.45f,1.50f };
		float Snaplines[3] = { 1.f, 1.f, 1.f };
		float bullet_impacts_2[3] = { 1.f, 1.f, 1.f };
		float Hitmarker[3] = { 1.f, 1.f, 1.f };
		float DamageIndicator[3] = { 1.f, 0.f, 0.f };
		float WeaponsCAT[3] = { 1.f, 1.f, 1.f };
		float skycolorxd[3] = { 1.f, 1.f, 1.f };
		float worldcolorxd[3] = { 1.f, 1.f, 1.f }; 

	} Colors;




	int config_sel;
	int aachange;



	bool Save(std::string file_name);
	bool Load(std::string file_name);
	void CreateConfig(std::string name);
	std::vector<std::string> GetConfigs();

};

extern ClientVariables* Clientvariables;
