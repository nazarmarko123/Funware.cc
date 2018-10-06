

#include "../SDK.h"
#include "NewSkinchanger.h"
#include "../global.h"
#include <unordered_map>
#include "sticker.h"
#include "../structs/offsets.h"
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
std::unordered_map<char*, char*> killIcons = {};
#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
HANDLE worldmodel_handle;
CBaseCombatWeapon* worldmodel;
#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))


void newskinchanger()
{
	short cw = SafeWeaponID();
	CBaseEntity* m_local = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
	auto weapons = m_local->m_hMyWeapons();
	for (size_t i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++)
	{
		CBaseEntity* pEntity = g_pEntitylist->GetClientEntityFromHandle(weapons[i]);
		if (pEntity)
		{
			auto pWeapon = (CBaseCombatWeapon*)pEntity;
			if (Clientvariables->NewSkinchanger.enabled && !Clientvariables->RankChanger.semiinventar)
			{
				int Model = Clientvariables->NewSkinchanger.Knife;
				int weapon = *pWeapon->fixskins();
				ApplyStickerHooks(pWeapon);
				switch (weapon)
				{
				case 7: // AK47 
				{
					if (cw = ItemDefinitionIndex::AK47)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.AK47Skin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 180;//fire serpent
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 524;//Fuel Injector
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 639;//Bloodsport
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 302;//vulcan
						break;
					case 5:
						*pWeapon->GetFallbackPaintKit() = 44;//case hardened
						break;
					case 6:
						*pWeapon->GetFallbackPaintKit() = 456;//Hydroponic
						break;
					case 7:
						*pWeapon->GetFallbackPaintKit() = 474;//Aquamarine Revenge
						break;
					case 8:
						*pWeapon->GetFallbackPaintKit() = 490;//Frontside Misty
						break;
					case 9:
						*pWeapon->GetFallbackPaintKit() = 506;//Point Disarray
						break;
					case 10:
						*pWeapon->GetFallbackPaintKit() = 600;//Neon Revolution
						break;
					case 11:
						*pWeapon->GetFallbackPaintKit() = 14;//red laminate
						break;
					case 12:
						*pWeapon->GetFallbackPaintKit() = 282;//redline
						break;
					case 13:
						*pWeapon->GetFallbackPaintKit() = 316;//jaguar
						break;
					case 14:
						*pWeapon->GetFallbackPaintKit() = 340;//jetset
						break;
					case 15:
						*pWeapon->GetFallbackPaintKit() = 380;//wasteland rebel
						break;
					case 16:
						*pWeapon->GetFallbackPaintKit() = 675;//The Empress
						break;
					case 17:
						*pWeapon->GetFallbackPaintKit() = 422;//Elite Build
						break;
					default:
						break;
					}
				}
				break;
				case 16: // M4A4
				{
					if (cw = ItemDefinitionIndex::M4A1_SILENCER)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.M4A4Skin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 255;//Asiimov
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 309;//Howl
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 400;//Dragon King
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 449;//Poseidon
						break;
					case 5:
						*pWeapon->GetFallbackPaintKit() = 471;//Daybreak
						break;
					case 6:
						*pWeapon->GetFallbackPaintKit() = 512;//Royal Paladin
						break;
					case 7:
						*pWeapon->GetFallbackPaintKit() = 533;//BattleStar
						break;
					case 8:
						*pWeapon->GetFallbackPaintKit() = 588;//Desolate Space
						break;
					case 9:
						*pWeapon->GetFallbackPaintKit() = 632;//Buzz Kill
						break;
					case 10:
						*pWeapon->GetFallbackPaintKit() = 155;//Bullet Rain
						break;
					case 11:
						*pWeapon->GetFallbackPaintKit() = 664;//Hell Fire
						break;
					case 12:
						*pWeapon->GetFallbackPaintKit() = 480;//Evil Daimyo
						break;
					case 13:
						*pWeapon->GetFallbackPaintKit() = 384;//Griffin
						break;
					case 14:
						*pWeapon->GetFallbackPaintKit() = 187;//Zirka
						break;
					case 15:
						*pWeapon->GetFallbackPaintKit() = 167;//Radiation Harzard
						break;
					default:
						break;
					}
				}
				break;
				case 2: // dual
				{
					if (cw = ItemDefinitionIndex::ELITE)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.DualSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 276;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 491;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 658;
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 625;
						break;
					case 5:
						*pWeapon->GetFallbackPaintKit() = 447;
						break;
					default:
						break;
					}
				}
				break;
				case 60: // M4A1
				{
					if (cw = ItemDefinitionIndex::M4A1)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.M4A1SSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 644;//Decimator
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 326;//Knight
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 548;//Chantico's Fire
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 497;//Golden Coi
						break;
					case 5:
						*pWeapon->GetFallbackPaintKit() = 430;//Hyper Beast
						break;
					case 6:
						*pWeapon->GetFallbackPaintKit() = 321;//Master Piece
						break;
					case 7:
						*pWeapon->GetFallbackPaintKit() = 445;//Hot Rod
						break;
					case 8:
						*pWeapon->GetFallbackPaintKit() = 587;//Mecha Industries
						break;
					case 9:
						*pWeapon->GetFallbackPaintKit() = 360;//Cyrex
						break;
					case 10:
						*pWeapon->GetFallbackPaintKit() = 440;//Icarus Fell
						break;
					case 11:
						*pWeapon->GetFallbackPaintKit() = 631;//Flashback
						break;
					case 12:
						*pWeapon->GetFallbackPaintKit() = 681;//Flashback
						break;
					case 13:
						*pWeapon->GetFallbackPaintKit() = 430;//Hyper Beast
						break;
					case 14:
						*pWeapon->GetFallbackPaintKit() = 301;//Atomic Alloy
						break;
					case 15:
						*pWeapon->GetFallbackPaintKit() = 257;//Guardian
						break;
					case 16:
						*pWeapon->GetFallbackPaintKit() = 663;//Briefing
						break;
					default:
						break;
					}
				}
				break;
				case 9: // AWP
				{
					if (cw=ItemDefinitionIndex::AWP)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.AWPSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 279;//asiimov
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 344;//dlore
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 640;//Fever Dream
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 446;//medusa
						break;
					case 5:
						*pWeapon->GetFallbackPaintKit() = 475;//hyperbeast
						break;
					case 6:
						*pWeapon->GetFallbackPaintKit() = 174;//boom
						break;
					case 7:
						*pWeapon->GetFallbackPaintKit() = 51;//lightning strike
						break;
					case 8:
						*pWeapon->GetFallbackPaintKit() = 84;//pink ddpat
						break;
					case 9:
						*pWeapon->GetFallbackPaintKit() = 181;//corticera
						break;
					case 10:
						*pWeapon->GetFallbackPaintKit() = 259;//redline
						break;
					case 11:
						*pWeapon->GetFallbackPaintKit() = 395;//manowar
						break;
					case 12:
						*pWeapon->GetFallbackPaintKit() = 212;//graphite
						break;
					case 13:
						*pWeapon->GetFallbackPaintKit() = 227;//electric hive
						break;
					case 14:
						*pWeapon->GetFallbackPaintKit() = 451;//Sun in Leo
						break;
					case 15:
						*pWeapon->GetFallbackPaintKit() = 475;//Hyper Beast
						break;
					case 16:
						*pWeapon->GetFallbackPaintKit() = 251;//Pit viper
						break;
					case 17:
						*pWeapon->GetFallbackPaintKit() = 584;//Phobos
						break;
					case 18:
						*pWeapon->GetFallbackPaintKit() = 525;//Elite Build
						break;
					case 19:
						*pWeapon->GetFallbackPaintKit() = 424;//Worm God
						break;
					case 20:
						*pWeapon->GetFallbackPaintKit() = 662;//Oni Taiji
						break;
					case 21:
						*pWeapon->GetFallbackPaintKit() = 640;//Fever Dream
						break;
					default:
						break;
					}
				}
				break;
				case 61: // USP
				{
					if (cw = ItemDefinitionIndex::USP_SILENCER)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.USPSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 653;//Neo-Noir
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 637;//Cyrex
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 313;//Orion
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 504;//Kill Confirmed
						break;
					case 5:
						*pWeapon->GetFallbackPaintKit() = 183;//Overgrowth
						break;
					case 6:
						*pWeapon->GetFallbackPaintKit() = 339;//Caiman
						break;
					case 7:
						*pWeapon->GetFallbackPaintKit() = 221;//Serum
						break;
					case 8:
						*pWeapon->GetFallbackPaintKit() = 290;//Guardian
						break;
					case 9:
						*pWeapon->GetFallbackPaintKit() = 318;//Road Rash
						break;
					default:
						break;
					}
				}
				break;
				case 4: // Glock
				{
					if (cw = ItemDefinitionIndex::GLOCK)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.GlockSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 38;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 48;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 437;
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 586;
						break;
					case 5:
						*pWeapon->GetFallbackPaintKit() = 353;
						break;
					case 6:
						*pWeapon->GetFallbackPaintKit() = 680;
						break;
					case 7:
						*pWeapon->GetFallbackPaintKit() = 607;
						break;
					case 8:
						*pWeapon->GetFallbackPaintKit() = 532;
						break;
					case 9:
						*pWeapon->GetFallbackPaintKit() = 381;
						break;
					case 10:
						*pWeapon->GetFallbackPaintKit() = 230;
						break;
					case 11:
						*pWeapon->GetFallbackPaintKit() = 159;
						break;
					case 12:
						*pWeapon->GetFallbackPaintKit() = 623;
						break;
					case 13:
						*pWeapon->GetFallbackPaintKit() = 479;
						break;
					case 14:
						*pWeapon->GetFallbackPaintKit() = 680;
						break;
					case 15:
						*pWeapon->GetFallbackPaintKit() = 367;
						break;
					default:
						break;
					}
				}
				break;
				case 1: // Deagle
				{
					if (cw = ItemDefinitionIndex::DEAGLE)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.DeagleSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 37;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 527;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 645;
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 185;
						break;
					case 5:
						*pWeapon->GetFallbackPaintKit() = 37;
						break;
					case 6:
						*pWeapon->GetFallbackPaintKit() = 645;
						break;
					case 7:
						*pWeapon->GetFallbackPaintKit() = 231;
						break;
					case 8:
						*pWeapon->GetFallbackPaintKit() = 603;
						break;
					default:
						break;
					}
				}
				break;
				case 3: // Five Seven
				{
					if (cw = ItemDefinitionIndex::FIVESEVEN)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.FiveSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 427;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 660;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 352;
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 530;
						break;
					case 5:
						*pWeapon->GetFallbackPaintKit() = 510;
						break;
					case 6:
						*pWeapon->GetFallbackPaintKit() = 646;
						break;
					case 7:
						*pWeapon->GetFallbackPaintKit() = 585;
						break;
					default:
						break;
					}
				}
				break;
				case 8: // AUG
				{
					if (cw = ItemDefinitionIndex::AUG)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.AUGSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 9;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 33;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 280;
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 455;
						break;
					default:
						break;
					}
				}
				break;
				case 10: // Famas
				{
					if (cw = ItemDefinitionIndex::FAMAS)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.FAMASSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 429;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 371;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 477;
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 492;
						break;
					default:
						break;
					}
				}
				break;
				case 11: // G3SG1
				{
					if (cw = ItemDefinitionIndex::G3SG1)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.G3sg1Skin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 677;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 511;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 463;
						break;
					default:
						break;
					}
				}
				break;
				case 13: // Galil
				{
					if (cw = ItemDefinitionIndex::GALILAR)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.GalilSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 398;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 647;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 661;
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 428;
						break;
					case 5:
						*pWeapon->GetFallbackPaintKit() = 379;
						break;
					default:
						break;
					}
				}
				break;
				case 14: // M249
				{
					if (cw = ItemDefinitionIndex::M249)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.M249Skin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 496;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 401;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 266;
						break;
					default:
						break;
					}
				}
				break;
				case 17: // Mac 10
				{
					if (cw = ItemDefinitionIndex::MAC10)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.Mac10Skin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 433;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 651;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 310;
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 498;
						break;
					default:
						break;
					}
				}
				break;
				case 19: // P90
				{
					if (cw = ItemDefinitionIndex::P90)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					*pWeapon->GetFallbackPaintKit() = 156;
				}
				break;
				case 24: // UMP-45
				{
					if (cw = ItemDefinitionIndex::UMP45)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.UMP45Skin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 37;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 441;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 448;
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 556;
						break;
					case 5:
						*pWeapon->GetFallbackPaintKit() = 688;
						break;
					default:
						break;
					}
				}
				break;
				case 25: // XM1014
				{
					if (cw = ItemDefinitionIndex::XM1014)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.XmSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 654;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 363;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 689;
						break;
					default:
						break;
					}
				}
				break;
				case 63: // CZ75-Auto
				{
					if (cw = ItemDefinitionIndex::CZ75A)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.Cz75Skin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 543;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 435;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 270;
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 643;
						break;
					default:
						break;
					}
				}
				break;
				case 26: // Bizon
				{
					if (cw = ItemDefinitionIndex::BIZON)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.BizonSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 676;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 542;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 508;
						break;
					default:
						break;
					}
				}
				break;
				case 27: // Mag 7
				{
					if (cw = ItemDefinitionIndex::MAG7)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.MagSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 39;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 431;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 608;
						break;
					default:
						break;
					}
				}
				break;
				case 28: // Negev
				{
					if (cw = ItemDefinitionIndex::NEGEV)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.NegevSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 514;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 483;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 432;
						break;
					default:
						break;
					}
				}
				break;
				case 29: // Sawed Off
				{
					if (cw = ItemDefinitionIndex::SAWEDOFF)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.SawedSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 638;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 256;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 517;
						break;
					default:
						break;
					}
				}
				break;
				case 30: // Tec 9
				{
					if (cw = ItemDefinitionIndex::TEC9)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.tec9Skin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 179;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 248;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 216;
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 272;
						break;
					case 5:
						*pWeapon->GetFallbackPaintKit() = 289;
						break;
					case 6:
						*pWeapon->GetFallbackPaintKit() = 303;
						break;
					case 7:
						*pWeapon->GetFallbackPaintKit() = 374;
						break;
					case 8:
						*pWeapon->GetFallbackPaintKit() = 555;
						break;
					case 9:
						*pWeapon->GetFallbackPaintKit() = 614;
						break;
					default:
						break;
					}
				}
				break;
				case 32: // P2000
				{
					if (cw = ItemDefinitionIndex::HKP2000)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.P2000Skin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 485;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 38;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 184;
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 211;
						break;
					case 5:
						*pWeapon->GetFallbackPaintKit() = 389;
						break;
					case 6:
						*pWeapon->GetFallbackPaintKit() = 442;
						break;
					case 7:
						*pWeapon->GetFallbackPaintKit() = 443;
						break;
					case 8:
						*pWeapon->GetFallbackPaintKit() = 515;
						break;
					case 9:
						*pWeapon->GetFallbackPaintKit() = 550;
						break;
					case 10:
						*pWeapon->GetFallbackPaintKit() = 591;
						break;
					default:
						break;
					}
				}
				break;
				case 33: // MP7
				{
					if (cw = ItemDefinitionIndex::MP7)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.Mp9Skin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 481;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 536;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 500;
						break;
					default:
						break;
					}
				}
				break;
				case 34: // MP9
				{
					if (cw = ItemDefinitionIndex::MP9)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.Mp9Skin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 262;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 482;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 609;
						break;
					default:
						break;
					}
				}
				break;
				case 35: // Nova
				{
					if (cw = ItemDefinitionIndex::NOVA)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.NovaSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 537;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 356;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 286;
						break;
					default:
						break;
					}
				}
				break;
				case 36: // P250
				{
					if (cw = ItemDefinitionIndex::P250)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.P250Skin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 102;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 466;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 467;
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 501;
						break;
					case 5:
						*pWeapon->GetFallbackPaintKit() = 551;
						break;
					case 6:
						*pWeapon->GetFallbackPaintKit() = 678;
						break;
					default:
						break;
					}
				}
				break;
				case 38: // Scar 20
				{
					if (cw = ItemDefinitionIndex::SCAR20)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.SCAR20Skin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 642;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 298;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 406;
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 453;
						break;
					case 5:
						*pWeapon->GetFallbackPaintKit() = 502;
						break;
					case 6:
						*pWeapon->GetFallbackPaintKit() = 518;
						break;
					case 7:
						*pWeapon->GetFallbackPaintKit() = 597;
						break;
					case 8:
						*pWeapon->GetFallbackPaintKit() = 685;
						break;
					default:
						break;
					}
				}
				break;
				case 39: // SG553
				{
					if (cw = ItemDefinitionIndex::SG556)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.Sg553Skin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 519;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 487;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 287;
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 586;
						break;
					default:
						break;
					}
				}
				break;
				case 40: // SSG08
				{
					if (cw = ItemDefinitionIndex::SSG08)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.SSG08Skin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 26;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 60;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 96;
						break;
					case 4:
						*pWeapon->GetFallbackPaintKit() = 99;
						break;
					case 5:
						*pWeapon->GetFallbackPaintKit() = 157;
						break;
					case 6:
						*pWeapon->GetFallbackPaintKit() = 200;
						break;
					case 7:
						*pWeapon->GetFallbackPaintKit() = 222;
						break;
					case 8:
						*pWeapon->GetFallbackPaintKit() = 233;
						break;
					case 9:
						*pWeapon->GetFallbackPaintKit() = 253;
						break;
					case 10:
						*pWeapon->GetFallbackPaintKit() = 304;
						break;
					case 11:
						*pWeapon->GetFallbackPaintKit() = 319;
						break;
					case 12:
						*pWeapon->GetFallbackPaintKit() = 361;
						break;
					case 13:
						*pWeapon->GetFallbackPaintKit() = 503;
						break;
					case 14:
						*pWeapon->GetFallbackPaintKit() = 538;
						break;
					case 15:
						*pWeapon->GetFallbackPaintKit() = 554;
						break;
					case 16:
						*pWeapon->GetFallbackPaintKit() = 624;
						break;
					default:
						break;
					}
				}
				break;
				case 64: // Revolver
				{
					if (cw = ItemDefinitionIndex::REVOLVER)
						pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
					switch (Clientvariables->NewSkinchanger.RevolverSkin)
					{
					case 0:
						*pWeapon->GetFallbackPaintKit() = 0;//none
						break;
					case 1:
						*pWeapon->GetFallbackPaintKit() = 683;
						break;
					case 2:
						*pWeapon->GetFallbackPaintKit() = 522;
						break;
					case 3:
						*pWeapon->GetFallbackPaintKit() = 12;
						break;
					default:
						break;
					}
				}
				break;
				default:
					break;
				}

				ClientClass* pClass = (ClientClass*)pEntity->GetClientClass();

				if ((int)pClass->m_ClassID == (int)93)
				{
					auto pCustomName1 = MakePtr(char*, pWeapon, 0x301C);
					worldmodel_handle = pWeapon->m_hWeaponWorldModel();
					if (worldmodel_handle) worldmodel = (CBaseCombatWeapon*)g_pEntitylist->GetClientEntityFromHandle(worldmodel_handle);
					if (Model == 0) // Bayonet
					{
						int iBayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
						*pWeapon->ModelIndex() = iBayonet; // m_nModelIndex
						*pWeapon->ViewModelIndex() = iBayonet;
						if (worldmodel) *pWeapon->ModelIndex() = iBayonet + 1;
						*pWeapon->fixskins() = 500;
						*pWeapon->GetEntityQuality() = 3;
						if (cw = ItemDefinitionIndex::KNIFE_BAYONET)
							pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
						killIcons.clear();
						killIcons["knife_default_ct"] = "bayonet";
						killIcons["knife_t"] = "bayonet";
						int Skin = Clientvariables->NewSkinchanger.KnifeSkin;
						if (Skin == 0)
						{
							*pWeapon->GetFallbackPaintKit() = 0; // Forest DDPAT
						}
						else if (Skin == 1)
						{
							*pWeapon->GetFallbackPaintKit() = 12; // Crimson Web
						}
						else if (Skin == 2)
						{
							*pWeapon->GetFallbackPaintKit() = 27; // Bone Mask
						}
						else if (Skin == 3)
						{
							*pWeapon->GetFallbackPaintKit() = 38; // Fade
						}
						else if (Skin == 4)
						{
							*pWeapon->GetFallbackPaintKit() = 40; // Night
						}
						else if (Skin == 5)
						{
							*pWeapon->GetFallbackPaintKit() = 42; // Blue Steel
						}
						else if (Skin == 6)
						{
							*pWeapon->GetFallbackPaintKit() = 43; // Stained
						}
						else if (Skin == 7)
						{
							*pWeapon->GetFallbackPaintKit() = 44; // Case Hardened
						}
						else if (Skin == 8)
						{
							*pWeapon->GetFallbackPaintKit() = 59; // Slaughter
						}
						else if (Skin == 9)
						{
							*pWeapon->GetFallbackPaintKit() = 72; // Safari Mesh
						}
						else if (Skin == 10)
						{
							*pWeapon->GetFallbackPaintKit() = 77; // Boreal Forest
						}
						else if (Skin == 11)
						{
							*pWeapon->GetFallbackPaintKit() = 98; // Ultraviolet
						}
						else if (Skin == 12)
						{
							*pWeapon->GetFallbackPaintKit() = 143; // Urban Masked
						}
						else if (Skin == 13)
						{
							*pWeapon->GetFallbackPaintKit() = 175; // Scorched
						}
						else if (Skin == 14)
						{
							*pWeapon->GetFallbackPaintKit() = 323; // Rust Coat
						}
						else if (Skin == 15)
						{
							*pWeapon->GetFallbackPaintKit() = 409; // Tiger Tooth
						}
						else if (Skin == 16)
						{
							*pWeapon->GetFallbackPaintKit() = 410; // Damascus Steel
						}
						else if (Skin == 17)
						{
							*pWeapon->GetFallbackPaintKit() = 411; // Damascus Steel
						}
						else if (Skin == 18)
						{
							*pWeapon->GetFallbackPaintKit() = 413; // Marble Fade
						}
						else if (Skin == 19)
						{
							*pWeapon->GetFallbackPaintKit() = 414; // Rust Coat
						}
						else if (Skin == 20)
						{
							*pWeapon->GetFallbackPaintKit() = 415; // Doppler Ruby
						}
						else if (Skin == 21)
						{
							*pWeapon->GetFallbackPaintKit() = 416; // Doppler Sapphire
						}
						else if (Skin == 22)
						{
							*pWeapon->GetFallbackPaintKit() = 417; // Doppler Blackpearl
						}
						else if (Skin == 23)
						{
							*pWeapon->GetFallbackPaintKit() = 418; // Doppler Phase 1
						}
						else if (Skin == 24)
						{
							*pWeapon->GetFallbackPaintKit() = 419; // Doppler Phase 2
						}
						else if (Skin == 25)
						{
							*pWeapon->GetFallbackPaintKit() = 420; // Doppler Phase 3
						}
						else if (Skin == 26)
						{
							*pWeapon->GetFallbackPaintKit() = 421; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->GetFallbackPaintKit() = 569; // Gamma Doppler Phase1
						}
						else if (Skin == 28)
						{
							*pWeapon->GetFallbackPaintKit() = 570; // Gamma Doppler Phase2
						}
						else if (Skin == 29)
						{
							*pWeapon->GetFallbackPaintKit() = 571; // Gamma Doppler Phase3
						}
						else if (Skin == 30)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Phase4
						}
						else if (Skin == 31)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Emerald
						}
						else if (Skin == 32)
						{
							*pWeapon->GetFallbackPaintKit() = 558; // Lore
						}
						else if (Skin == 33)
						{
							*pWeapon->GetFallbackPaintKit() = 563; // Black Laminate
						}
						else if (Skin == 34)
						{
							*pWeapon->GetFallbackPaintKit() = 573; // Autotronic
						}
						else if (Skin == 35)
						{
							*pWeapon->GetFallbackPaintKit() = 580; // Freehand
						}
					}
					else if (Model == 9) // Bowie Knife
					{
						int iBowie = g_pModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
						*pWeapon->ModelIndex() = iBowie; // m_nModelIndex
						*pWeapon->ViewModelIndex() = iBowie;
						if (worldmodel) *worldmodel->ModelIndex() = iBowie + 1;
						*pWeapon->fixskins() = 514;
						*pWeapon->GetEntityQuality() = 3;
						if (cw = ItemDefinitionIndex::KNIFE_BOWIE)
							pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_survival_bowie";
						killIcons["knife_t"] = "knife_survival_bowie";
						int Skin = Clientvariables->NewSkinchanger.KnifeSkin;
						if (Skin == 0)
						{
							*pWeapon->GetFallbackPaintKit() = 0; // Forest DDPAT
						}
						else if (Skin == 1)
						{
							*pWeapon->GetFallbackPaintKit() = 12; // Crimson Web
						}
						else if (Skin == 2)
						{
							*pWeapon->GetFallbackPaintKit() = 27; // Bone Mask
						}
						else if (Skin == 3)
						{
							*pWeapon->GetFallbackPaintKit() = 38; // Fade
						}
						else if (Skin == 4)
						{
							*pWeapon->GetFallbackPaintKit() = 40; // Night
						}
						else if (Skin == 5)
						{
							*pWeapon->GetFallbackPaintKit() = 42; // Blue Steel
						}
						else if (Skin == 6)
						{
							*pWeapon->GetFallbackPaintKit() = 43; // Stained
						}
						else if (Skin == 7)
						{
							*pWeapon->GetFallbackPaintKit() = 44; // Case Hardened
						}
						else if (Skin == 8)
						{
							*pWeapon->GetFallbackPaintKit() = 59; // Slaughter
						}
						else if (Skin == 9)
						{
							*pWeapon->GetFallbackPaintKit() = 72; // Safari Mesh
						}
						else if (Skin == 10)
						{
							*pWeapon->GetFallbackPaintKit() = 77; // Boreal Forest
						}
						else if (Skin == 11)
						{
							*pWeapon->GetFallbackPaintKit() = 98; // Ultraviolet
						}
						else if (Skin == 12)
						{
							*pWeapon->GetFallbackPaintKit() = 143; // Urban Masked
						}
						else if (Skin == 13)
						{
							*pWeapon->GetFallbackPaintKit() = 175; // Scorched
						}
						else if (Skin == 14)
						{
							*pWeapon->GetFallbackPaintKit() = 323; // Rust Coat
						}
						else if (Skin == 15)
						{
							*pWeapon->GetFallbackPaintKit() = 409; // Tiger Tooth
						}
						else if (Skin == 16)
						{
							*pWeapon->GetFallbackPaintKit() = 410; // Damascus Steel
						}
						else if (Skin == 17)
						{
							*pWeapon->GetFallbackPaintKit() = 411; // Damascus Steel
						}
						else if (Skin == 18)
						{
							*pWeapon->GetFallbackPaintKit() = 413; // Marble Fade
						}
						else if (Skin == 19)
						{
							*pWeapon->GetFallbackPaintKit() = 414; // Rust Coat
						}
						else if (Skin == 20)
						{
							*pWeapon->GetFallbackPaintKit() = 415; // Doppler Ruby
						}
						else if (Skin == 21)
						{
							*pWeapon->GetFallbackPaintKit() = 416; // Doppler Sapphire
						}
						else if (Skin == 22)
						{
							*pWeapon->GetFallbackPaintKit() = 417; // Doppler Blackpearl
						}
						else if (Skin == 23)
						{
							*pWeapon->GetFallbackPaintKit() = 418; // Doppler Phase 1
						}
						else if (Skin == 24)
						{
							*pWeapon->GetFallbackPaintKit() = 419; // Doppler Phase 2
						}
						else if (Skin == 25)
						{
							*pWeapon->GetFallbackPaintKit() = 420; // Doppler Phase 3
						}
						else if (Skin == 26)
						{
							*pWeapon->GetFallbackPaintKit() = 421; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->GetFallbackPaintKit() = 569; // Gamma Doppler Phase1
						}
						else if (Skin == 28)
						{
							*pWeapon->GetFallbackPaintKit() = 570; // Gamma Doppler Phase2
						}
						else if (Skin == 29)
						{
							*pWeapon->GetFallbackPaintKit() = 571; // Gamma Doppler Phase3
						}
						else if (Skin == 30)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Phase4
						}
						else if (Skin == 31)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Emerald
						}
						else if (Skin == 32)
						{
							*pWeapon->GetFallbackPaintKit() = 558; // Lore
						}
						else if (Skin == 33)
						{
							*pWeapon->GetFallbackPaintKit() = 567; // Black Laminate
						}
						else if (Skin == 34)
						{
							*pWeapon->GetFallbackPaintKit() = 574; // Autotronic
						}
						else if (Skin == 35)
						{
							*pWeapon->GetFallbackPaintKit() = 580; // Freehand
						}

					}
					else if (Model == 6) // Butterfly Knife
					{
						int iButterfly = g_pModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
						*pWeapon->ModelIndex() = iButterfly; // m_nModelIndex
						*pWeapon->ViewModelIndex() = iButterfly;
						if (worldmodel) *worldmodel->ModelIndex() = iButterfly + 1;
						*pWeapon->fixskins() = 515;
						*pWeapon->GetEntityQuality() = 3;
						if (cw = ItemDefinitionIndex::KNIFE_BUTTERFLY)
							pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_butterfly";
						killIcons["knife_t"] = "knife_butterfly";
						int Skin = Clientvariables->NewSkinchanger.KnifeSkin;
						if (Skin == 0)
						{
							*pWeapon->GetFallbackPaintKit() = 0; // Forest DDPAT
						}
						else if (Skin == 1)
						{
							*pWeapon->GetFallbackPaintKit() = 12; // Crimson Web
						}
						else if (Skin == 2)
						{
							*pWeapon->GetFallbackPaintKit() = 27; // Bone Mask
						}
						else if (Skin == 3)
						{
							*pWeapon->GetFallbackPaintKit() = 38; // Fade
						}
						else if (Skin == 4)
						{
							*pWeapon->GetFallbackPaintKit() = 40; // Night
						}
						else if (Skin == 5)
						{
							*pWeapon->GetFallbackPaintKit() = 42; // Blue Steel
						}
						else if (Skin == 6)
						{
							*pWeapon->GetFallbackPaintKit() = 43; // Stained
						}
						else if (Skin == 7)
						{
							*pWeapon->GetFallbackPaintKit() = 44; // Case Hardened
						}
						else if (Skin == 8)
						{
							*pWeapon->GetFallbackPaintKit() = 59; // Slaughter
						}
						else if (Skin == 9)
						{
							*pWeapon->GetFallbackPaintKit() = 72; // Safari Mesh
						}
						else if (Skin == 10)
						{
							*pWeapon->GetFallbackPaintKit() = 77; // Boreal Forest
						}
						else if (Skin == 11)
						{
							*pWeapon->GetFallbackPaintKit() = 98; // Ultraviolet
						}
						else if (Skin == 12)
						{
							*pWeapon->GetFallbackPaintKit() = 143; // Urban Masked
						}
						else if (Skin == 13)
						{
							*pWeapon->GetFallbackPaintKit() = 175; // Scorched
						}
						else if (Skin == 14)
						{
							*pWeapon->GetFallbackPaintKit() = 323; // Rust Coat
						}
						else if (Skin == 15)
						{
							*pWeapon->GetFallbackPaintKit() = 409; // Tiger Tooth
						}
						else if (Skin == 16)
						{
							*pWeapon->GetFallbackPaintKit() = 410; // Damascus Steel
						}
						else if (Skin == 17)
						{
							*pWeapon->GetFallbackPaintKit() = 411; // Damascus Steel
						}
						else if (Skin == 18)
						{
							*pWeapon->GetFallbackPaintKit() = 413; // Marble Fade
						}
						else if (Skin == 19)
						{
							*pWeapon->GetFallbackPaintKit() = 414; // Rust Coat
						}
						else if (Skin == 20)
						{
							*pWeapon->GetFallbackPaintKit() = 415; // Doppler Ruby
						}
						else if (Skin == 21)
						{
							*pWeapon->GetFallbackPaintKit() = 416; // Doppler Sapphire
						}
						else if (Skin == 22)
						{
							*pWeapon->GetFallbackPaintKit() = 417; // Doppler Blackpearl
						}
						else if (Skin == 23)
						{
							*pWeapon->GetFallbackPaintKit() = 418; // Doppler Phase 1
						}
						else if (Skin == 24)
						{
							*pWeapon->GetFallbackPaintKit() = 419; // Doppler Phase 2
						}
						else if (Skin == 25)
						{
							*pWeapon->GetFallbackPaintKit() = 420; // Doppler Phase 3
						}
						else if (Skin == 26)
						{
							*pWeapon->GetFallbackPaintKit() = 421; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->GetFallbackPaintKit() = 569; // Gamma Doppler Phase1
						}
						else if (Skin == 28)
						{
							*pWeapon->GetFallbackPaintKit() = 570; // Gamma Doppler Phase2
						}
						else if (Skin == 29)
						{
							*pWeapon->GetFallbackPaintKit() = 571; // Gamma Doppler Phase3
						}
						else if (Skin == 30)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Phase4
						}
						else if (Skin == 31)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Emerald
						}
						else if (Skin == 32)
						{
							*pWeapon->GetFallbackPaintKit() = 558; // Lore
						}
						else if (Skin == 33)
						{
							*pWeapon->GetFallbackPaintKit() = 567; // Black Laminate
						}
						else if (Skin == 34)
						{
							*pWeapon->GetFallbackPaintKit() = 574; // Autotronic
						}
						else if (Skin == 35)
						{
							*pWeapon->GetFallbackPaintKit() = 580; // Freehand
						}
					}
					else if (Model == 7) // Falchion Knife
					{
						int iFalchion = g_pModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
						*pWeapon->ModelIndex() = iFalchion; // m_nModelIndex
						*pWeapon->ViewModelIndex() = iFalchion;
						if (worldmodel) *worldmodel->ModelIndex() = iFalchion + 1;
						*pWeapon->fixskins() = 512;
						*pWeapon->GetEntityQuality() = 3;
						if (cw = ItemDefinitionIndex::KNIFE_FALCHION)
							pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_falchion";
						killIcons["knife_t"] = "knife_falchion";
						int Skin = Clientvariables->NewSkinchanger.KnifeSkin;
						if (Skin == 0)
						{
							*pWeapon->GetFallbackPaintKit() = 0; // Forest DDPAT
						}
						else if (Skin == 1)
						{
							*pWeapon->GetFallbackPaintKit() = 12; // Crimson Web
						}
						else if (Skin == 2)
						{
							*pWeapon->GetFallbackPaintKit() = 27; // Bone Mask
						}
						else if (Skin == 3)
						{
							*pWeapon->GetFallbackPaintKit() = 38; // Fade
						}
						else if (Skin == 4)
						{
							*pWeapon->GetFallbackPaintKit() = 40; // Night
						}
						else if (Skin == 5)
						{
							*pWeapon->GetFallbackPaintKit() = 42; // Blue Steel
						}
						else if (Skin == 6)
						{
							*pWeapon->GetFallbackPaintKit() = 43; // Stained
						}
						else if (Skin == 7)
						{
							*pWeapon->GetFallbackPaintKit() = 44; // Case Hardened
						}
						else if (Skin == 8)
						{
							*pWeapon->GetFallbackPaintKit() = 59; // Slaughter
						}
						else if (Skin == 9)
						{
							*pWeapon->GetFallbackPaintKit() = 72; // Safari Mesh
						}
						else if (Skin == 10)
						{
							*pWeapon->GetFallbackPaintKit() = 77; // Boreal Forest
						}
						else if (Skin == 11)
						{
							*pWeapon->GetFallbackPaintKit() = 98; // Ultraviolet
						}
						else if (Skin == 12)
						{
							*pWeapon->GetFallbackPaintKit() = 143; // Urban Masked
						}
						else if (Skin == 13)
						{
							*pWeapon->GetFallbackPaintKit() = 175; // Scorched
						}
						else if (Skin == 14)
						{
							*pWeapon->GetFallbackPaintKit() = 323; // Rust Coat
						}
						else if (Skin == 15)
						{
							*pWeapon->GetFallbackPaintKit() = 409; // Tiger Tooth
						}
						else if (Skin == 16)
						{
							*pWeapon->GetFallbackPaintKit() = 410; // Damascus Steel
						}
						else if (Skin == 17)
						{
							*pWeapon->GetFallbackPaintKit() = 411; // Damascus Steel
						}
						else if (Skin == 18)
						{
							*pWeapon->GetFallbackPaintKit() = 413; // Marble Fade
						}
						else if (Skin == 19)
						{
							*pWeapon->GetFallbackPaintKit() = 414; // Rust Coat
						}
						else if (Skin == 20)
						{
							*pWeapon->GetFallbackPaintKit() = 415; // Doppler Ruby
						}
						else if (Skin == 21)
						{
							*pWeapon->GetFallbackPaintKit() = 416; // Doppler Sapphire
						}
						else if (Skin == 22)
						{
							*pWeapon->GetFallbackPaintKit() = 417; // Doppler Blackpearl
						}
						else if (Skin == 23)
						{
							*pWeapon->GetFallbackPaintKit() = 418; // Doppler Phase 1
						}
						else if (Skin == 24)
						{
							*pWeapon->GetFallbackPaintKit() = 419; // Doppler Phase 2
						}
						else if (Skin == 25)
						{
							*pWeapon->GetFallbackPaintKit() = 420; // Doppler Phase 3
						}
						else if (Skin == 26)
						{
							*pWeapon->GetFallbackPaintKit() = 421; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->GetFallbackPaintKit() = 569; // Gamma Doppler Phase1
						}
						else if (Skin == 28)
						{
							*pWeapon->GetFallbackPaintKit() = 570; // Gamma Doppler Phase2
						}
						else if (Skin == 29)
						{
							*pWeapon->GetFallbackPaintKit() = 571; // Gamma Doppler Phase3
						}
						else if (Skin == 30)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Phase4
						}
						else if (Skin == 31)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Emerald
						}
						else if (Skin == 32)
						{
							*pWeapon->GetFallbackPaintKit() = 558; // Lore
						}
						else if (Skin == 33)
						{
							*pWeapon->GetFallbackPaintKit() = 567; // Black Laminate
						}
						else if (Skin == 34)
						{
							*pWeapon->GetFallbackPaintKit() = 574; // Autotronic
						}
						else if (Skin == 35)
						{
							*pWeapon->GetFallbackPaintKit() = 580; // Freehand
						}
					}
					else if (Model == 1) // Flip Knife
					{
						int iFlip = g_pModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
						*pWeapon->ModelIndex() = iFlip; // m_nModelIndex
						*pWeapon->ViewModelIndex() = iFlip;
						if (worldmodel) *worldmodel->ModelIndex() = iFlip + 1;
						*pWeapon->fixskins() = 505;
						*pWeapon->GetEntityQuality() = 3;
						if (cw = ItemDefinitionIndex::KNIFE_FLIP)
							pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_flip";
						killIcons["knife_t"] = "knife_flip";
						int Skin = Clientvariables->NewSkinchanger.KnifeSkin;
						if (Skin == 0)
						{
							*pWeapon->GetFallbackPaintKit() = 0; // Forest DDPAT
						}
						else if (Skin == 1)
						{
							*pWeapon->GetFallbackPaintKit() = 12; // Crimson Web
						}
						else if (Skin == 2)
						{
							*pWeapon->GetFallbackPaintKit() = 27; // Bone Mask
						}
						else if (Skin == 3)
						{
							*pWeapon->GetFallbackPaintKit() = 38; // Fade
						}
						else if (Skin == 4)
						{
							*pWeapon->GetFallbackPaintKit() = 40; // Night
						}
						else if (Skin == 5)
						{
							*pWeapon->GetFallbackPaintKit() = 42; // Blue Steel
						}
						else if (Skin == 6)
						{
							*pWeapon->GetFallbackPaintKit() = 43; // Stained
						}
						else if (Skin == 7)
						{
							*pWeapon->GetFallbackPaintKit() = 44; // Case Hardened
						}
						else if (Skin == 8)
						{
							*pWeapon->GetFallbackPaintKit() = 59; // Slaughter
						}
						else if (Skin == 9)
						{
							*pWeapon->GetFallbackPaintKit() = 72; // Safari Mesh
						}
						else if (Skin == 10)
						{
							*pWeapon->GetFallbackPaintKit() = 77; // Boreal Forest
						}
						else if (Skin == 11)
						{
							*pWeapon->GetFallbackPaintKit() = 98; // Ultraviolet
						}
						else if (Skin == 12)
						{
							*pWeapon->GetFallbackPaintKit() = 143; // Urban Masked
						}
						else if (Skin == 13)
						{
							*pWeapon->GetFallbackPaintKit() = 175; // Scorched
						}
						else if (Skin == 14)
						{
							*pWeapon->GetFallbackPaintKit() = 323; // Rust Coat
						}
						else if (Skin == 15)
						{
							*pWeapon->GetFallbackPaintKit() = 409; // Tiger Tooth
						}
						else if (Skin == 16)
						{
							*pWeapon->GetFallbackPaintKit() = 410; // Damascus Steel
						}
						else if (Skin == 17)
						{
							*pWeapon->GetFallbackPaintKit() = 411; // Damascus Steel
						}
						else if (Skin == 18)
						{
							*pWeapon->GetFallbackPaintKit() = 413; // Marble Fade
						}
						else if (Skin == 19)
						{
							*pWeapon->GetFallbackPaintKit() = 414; // Rust Coat
						}
						else if (Skin == 20)
						{
							*pWeapon->GetFallbackPaintKit() = 415; // Doppler Ruby
						}
						else if (Skin == 21)
						{
							*pWeapon->GetFallbackPaintKit() = 416; // Doppler Sapphire
						}
						else if (Skin == 22)
						{
							*pWeapon->GetFallbackPaintKit() = 417; // Doppler Blackpearl
						}
						else if (Skin == 23)
						{
							*pWeapon->GetFallbackPaintKit() = 418; // Doppler Phase 1
						}
						else if (Skin == 24)
						{
							*pWeapon->GetFallbackPaintKit() = 419; // Doppler Phase 2
						}
						else if (Skin == 25)
						{
							*pWeapon->GetFallbackPaintKit() = 420; // Doppler Phase 3
						}
						else if (Skin == 26)
						{
							*pWeapon->GetFallbackPaintKit() = 421; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->GetFallbackPaintKit() = 569; // Gamma Doppler Phase1
						}
						else if (Skin == 28)
						{
							*pWeapon->GetFallbackPaintKit() = 570; // Gamma Doppler Phase2
						}
						else if (Skin == 29)
						{
							*pWeapon->GetFallbackPaintKit() = 571; // Gamma Doppler Phase3
						}
						else if (Skin == 30)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Phase4
						}
						else if (Skin == 31)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Emerald
						}
						else if (Skin == 32)
						{
							*pWeapon->GetFallbackPaintKit() = 559; // Lore
						}
						else if (Skin == 33)
						{
							*pWeapon->GetFallbackPaintKit() = 564; // Black Laminate
						}
						else if (Skin == 34)
						{
							*pWeapon->GetFallbackPaintKit() = 574; // Autotronic
						}
						else if (Skin == 35)
						{
							*pWeapon->GetFallbackPaintKit() = 580; // Freehand
						}

					}
					else if (Model == 2) // Gut Knife
					{
						int iGut = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
						*pWeapon->ModelIndex() = iGut; // m_nModelIndex
						*pWeapon->ViewModelIndex() = iGut;
						if (worldmodel) *worldmodel->ModelIndex() = iGut + 1;
						*pWeapon->fixskins() = 506;
						*pWeapon->GetEntityQuality() = 3;
						if (cw = ItemDefinitionIndex::KNIFE_GUT)
							pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_gut";
						killIcons["knife_t"] = "knife_gut";
						int Skin = Clientvariables->NewSkinchanger.KnifeSkin;
						if (Skin == 0)
						{
							*pWeapon->GetFallbackPaintKit() = 0; // Forest DDPAT
						}
						else if (Skin == 1)
						{
							*pWeapon->GetFallbackPaintKit() = 12; // Crimson Web
						}
						else if (Skin == 2)
						{
							*pWeapon->GetFallbackPaintKit() = 27; // Bone Mask
						}
						else if (Skin == 3)
						{
							*pWeapon->GetFallbackPaintKit() = 38; // Fade
						}
						else if (Skin == 4)
						{
							*pWeapon->GetFallbackPaintKit() = 40; // Night
						}
						else if (Skin == 5)
						{
							*pWeapon->GetFallbackPaintKit() = 42; // Blue Steel
						}
						else if (Skin == 6)
						{
							*pWeapon->GetFallbackPaintKit() = 43; // Stained
						}
						else if (Skin == 7)
						{
							*pWeapon->GetFallbackPaintKit() = 44; // Case Hardened
						}
						else if (Skin == 8)
						{
							*pWeapon->GetFallbackPaintKit() = 59; // Slaughter
						}
						else if (Skin == 9)
						{
							*pWeapon->GetFallbackPaintKit() = 72; // Safari Mesh
						}
						else if (Skin == 10)
						{
							*pWeapon->GetFallbackPaintKit() = 77; // Boreal Forest
						}
						else if (Skin == 11)
						{
							*pWeapon->GetFallbackPaintKit() = 98; // Ultraviolet
						}
						else if (Skin == 12)
						{
							*pWeapon->GetFallbackPaintKit() = 143; // Urban Masked
						}
						else if (Skin == 13)
						{
							*pWeapon->GetFallbackPaintKit() = 175; // Scorched
						}
						else if (Skin == 14)
						{
							*pWeapon->GetFallbackPaintKit() = 323; // Rust Coat
						}
						else if (Skin == 15)
						{
							*pWeapon->GetFallbackPaintKit() = 409; // Tiger Tooth
						}
						else if (Skin == 16)
						{
							*pWeapon->GetFallbackPaintKit() = 410; // Damascus Steel
						}
						else if (Skin == 17)
						{
							*pWeapon->GetFallbackPaintKit() = 411; // Damascus Steel
						}
						else if (Skin == 18)
						{
							*pWeapon->GetFallbackPaintKit() = 413; // Marble Fade
						}
						else if (Skin == 19)
						{
							*pWeapon->GetFallbackPaintKit() = 414; // Rust Coat
						}
						else if (Skin == 20)
						{
							*pWeapon->GetFallbackPaintKit() = 415; // Doppler Ruby
						}
						else if (Skin == 21)
						{
							*pWeapon->GetFallbackPaintKit() = 416; // Doppler Sapphire
						}
						else if (Skin == 22)
						{
							*pWeapon->GetFallbackPaintKit() = 417; // Doppler Blackpearl
						}
						else if (Skin == 23)
						{
							*pWeapon->GetFallbackPaintKit() = 418; // Doppler Phase 1
						}
						else if (Skin == 24)
						{
							*pWeapon->GetFallbackPaintKit() = 419; // Doppler Phase 2
						}
						else if (Skin == 25)
						{
							*pWeapon->GetFallbackPaintKit() = 420; // Doppler Phase 3
						}
						else if (Skin == 26)
						{
							*pWeapon->GetFallbackPaintKit() = 421; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->GetFallbackPaintKit() = 569; // Gamma Doppler Phase1
						}
						else if (Skin == 28)
						{
							*pWeapon->GetFallbackPaintKit() = 570; // Gamma Doppler Phase2
						}
						else if (Skin == 29)
						{
							*pWeapon->GetFallbackPaintKit() = 571; // Gamma Doppler Phase3
						}
						else if (Skin == 30)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Phase4
						}
						else if (Skin == 31)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Emerald
						}
						else if (Skin == 32)
						{
							*pWeapon->GetFallbackPaintKit() = 560; // Lore
						}
						else if (Skin == 33)
						{
							*pWeapon->GetFallbackPaintKit() = 565; // Black Laminate
						}
						else if (Skin == 34)
						{
							*pWeapon->GetFallbackPaintKit() = 575; // Autotronic
						}
						else if (Skin == 35)
						{
							*pWeapon->GetFallbackPaintKit() = 580; // Freehand
						}
					}
					else if (Model == 5) // Huntsman Knife
					{
						int iHuntsman = g_pModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
						*pWeapon->ModelIndex() = iHuntsman; // m_nModelIndex
						*pWeapon->ViewModelIndex() = iHuntsman;
						if (worldmodel) *worldmodel->ModelIndex() = iHuntsman + 1;
						*pWeapon->fixskins() = 509;
						*pWeapon->GetEntityQuality() = 3;
						if (cw = 509)
							pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_tactical";
						killIcons["knife_t"] = "knife_tactical";
						int Skin = Clientvariables->NewSkinchanger.KnifeSkin;
						if (Skin == 0)
						{
							*pWeapon->GetFallbackPaintKit() = 0; // Forest DDPAT
						}
						else if (Skin == 1)
						{
							*pWeapon->GetFallbackPaintKit() = 12; // Crimson Web
						}
						else if (Skin == 2)
						{
							*pWeapon->GetFallbackPaintKit() = 27; // Bone Mask
						}
						else if (Skin == 3)
						{
							*pWeapon->GetFallbackPaintKit() = 38; // Fade
						}
						else if (Skin == 4)
						{
							*pWeapon->GetFallbackPaintKit() = 40; // Night
						}
						else if (Skin == 5)
						{
							*pWeapon->GetFallbackPaintKit() = 42; // Blue Steel
						}
						else if (Skin == 6)
						{
							*pWeapon->GetFallbackPaintKit() = 43; // Stained
						}
						else if (Skin == 7)
						{
							*pWeapon->GetFallbackPaintKit() = 44; // Case Hardened
						}
						else if (Skin == 8)
						{
							*pWeapon->GetFallbackPaintKit() = 59; // Slaughter
						}
						else if (Skin == 9)
						{
							*pWeapon->GetFallbackPaintKit() = 72; // Safari Mesh
						}
						else if (Skin == 10)
						{
							*pWeapon->GetFallbackPaintKit() = 77; // Boreal Forest
						}
						else if (Skin == 11)
						{
							*pWeapon->GetFallbackPaintKit() = 98; // Ultraviolet
						}
						else if (Skin == 12)
						{
							*pWeapon->GetFallbackPaintKit() = 143; // Urban Masked
						}
						else if (Skin == 13)
						{
							*pWeapon->GetFallbackPaintKit() = 175; // Scorched
						}
						else if (Skin == 14)
						{
							*pWeapon->GetFallbackPaintKit() = 323; // Rust Coat
						}
						else if (Skin == 15)
						{
							*pWeapon->GetFallbackPaintKit() = 409; // Tiger Tooth
						}
						else if (Skin == 16)
						{
							*pWeapon->GetFallbackPaintKit() = 410; // Damascus Steel
						}
						else if (Skin == 17)
						{
							*pWeapon->GetFallbackPaintKit() = 411; // Damascus Steel
						}
						else if (Skin == 18)
						{
							*pWeapon->GetFallbackPaintKit() = 413; // Marble Fade
						}
						else if (Skin == 19)
						{
							*pWeapon->GetFallbackPaintKit() = 414; // Rust Coat
						}
						else if (Skin == 20)
						{
							*pWeapon->GetFallbackPaintKit() = 415; // Doppler Ruby
						}
						else if (Skin == 21)
						{
							*pWeapon->GetFallbackPaintKit() = 416; // Doppler Sapphire
						}
						else if (Skin == 22)
						{
							*pWeapon->GetFallbackPaintKit() = 417; // Doppler Blackpearl
						}
						else if (Skin == 23)
						{
							*pWeapon->GetFallbackPaintKit() = 418; // Doppler Phase 1
						}
						else if (Skin == 24)
						{
							*pWeapon->GetFallbackPaintKit() = 419; // Doppler Phase 2
						}
						else if (Skin == 25)
						{
							*pWeapon->GetFallbackPaintKit() = 420; // Doppler Phase 3
						}
						else if (Skin == 26)
						{
							*pWeapon->GetFallbackPaintKit() = 421; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->GetFallbackPaintKit() = 569; // Gamma Doppler Phase1
						}
						else if (Skin == 28)
						{
							*pWeapon->GetFallbackPaintKit() = 570; // Gamma Doppler Phase2
						}
						else if (Skin == 29)
						{
							*pWeapon->GetFallbackPaintKit() = 571; // Gamma Doppler Phase3
						}
						else if (Skin == 30)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Phase4
						}
						else if (Skin == 31)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Emerald
						}
						else if (Skin == 32)
						{
							*pWeapon->GetFallbackPaintKit() = 559; // Lore
						}
						else if (Skin == 33)
						{
							*pWeapon->GetFallbackPaintKit() = 567; // Black Laminate
						}
						else if (Skin == 34)
						{
							*pWeapon->GetFallbackPaintKit() = 574; // Autotronic
						}
						else if (Skin == 35)
						{
							*pWeapon->GetFallbackPaintKit() = 580; // Freehand
						}
					}
					else if (Model == 3) // Karambit
					{
						int iKarambit = g_pModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
						*pWeapon->ModelIndex() = iKarambit; // m_nModelIndex
						*pWeapon->ViewModelIndex() = iKarambit;
						if (worldmodel) *worldmodel->ModelIndex() = iKarambit + 1;
						*pWeapon->fixskins() = 507;
						*pWeapon->GetEntityQuality() = 3;
						if (cw = ItemDefinitionIndex::KNIFE_KARAMBIT)
							pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_karambit";
						killIcons["knife_t"] = "knife_karambit";
						int Skin = Clientvariables->NewSkinchanger.KnifeSkin;
						if (Skin == 0)
						{
							*pWeapon->GetFallbackPaintKit() = 0; // Forest DDPAT
						}
						else if (Skin == 1)
						{
							*pWeapon->GetFallbackPaintKit() = 12; // Crimson Web
						}
						else if (Skin == 2)
						{
							*pWeapon->GetFallbackPaintKit() = 27; // Bone Mask
						}
						else if (Skin == 3)
						{
							*pWeapon->GetFallbackPaintKit() = 38; // Fade
						}
						else if (Skin == 4)
						{
							*pWeapon->GetFallbackPaintKit() = 40; // Night
						}
						else if (Skin == 5)
						{
							*pWeapon->GetFallbackPaintKit() = 42; // Blue Steel
						}
						else if (Skin == 6)
						{
							*pWeapon->GetFallbackPaintKit() = 43; // Stained
						}
						else if (Skin == 7)
						{
							*pWeapon->GetFallbackPaintKit() = 44; // Case Hardened
						}
						else if (Skin == 8)
						{
							*pWeapon->GetFallbackPaintKit() = 59; // Slaughter
						}
						else if (Skin == 9)
						{
							*pWeapon->GetFallbackPaintKit() = 72; // Safari Mesh
						}
						else if (Skin == 10)
						{
							*pWeapon->GetFallbackPaintKit() = 77; // Boreal Forest
						}
						else if (Skin == 11)
						{
							*pWeapon->GetFallbackPaintKit() = 98; // Ultraviolet
						}
						else if (Skin == 12)
						{
							*pWeapon->GetFallbackPaintKit() = 143; // Urban Masked
						}
						else if (Skin == 13)
						{
							*pWeapon->GetFallbackPaintKit() = 175; // Scorched
						}
						else if (Skin == 14)
						{
							*pWeapon->GetFallbackPaintKit() = 323; // Rust Coat
						}
						else if (Skin == 15)
						{
							*pWeapon->GetFallbackPaintKit() = 409; // Tiger Tooth
						}
						else if (Skin == 16)
						{
							*pWeapon->GetFallbackPaintKit() = 410; // Damascus Steel
						}
						else if (Skin == 17)
						{
							*pWeapon->GetFallbackPaintKit() = 411; // Damascus Steel
						}
						else if (Skin == 18)
						{
							*pWeapon->GetFallbackPaintKit() = 413; // Marble Fade
						}
						else if (Skin == 19)
						{
							*pWeapon->GetFallbackPaintKit() = 414; // Rust Coat
						}
						else if (Skin == 20)
						{
							*pWeapon->GetFallbackPaintKit() = 415; // Doppler Ruby
						}
						else if (Skin == 21)
						{
							*pWeapon->GetFallbackPaintKit() = 416; // Doppler Sapphire
						}
						else if (Skin == 22)
						{
							*pWeapon->GetFallbackPaintKit() = 417; // Doppler Blackpearl
						}
						else if (Skin == 23)
						{
							*pWeapon->GetFallbackPaintKit() = 418; // Doppler Phase 1
						}
						else if (Skin == 24)
						{
							*pWeapon->GetFallbackPaintKit() = 419; // Doppler Phase 2
						}
						else if (Skin == 25)
						{
							*pWeapon->GetFallbackPaintKit() = 420; // Doppler Phase 3
						}
						else if (Skin == 26)
						{
							*pWeapon->GetFallbackPaintKit() = 421; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->GetFallbackPaintKit() = 570; // Doppler Phase 4
						}
						else if (Skin == 28)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->GetFallbackPaintKit() = 569; // Gamma Doppler Phase1
						}
						else if (Skin == 28)
						{
							*pWeapon->GetFallbackPaintKit() = 570; // Gamma Doppler Phase2
						}
						else if (Skin == 29)
						{
							*pWeapon->GetFallbackPaintKit() = 571; // Gamma Doppler Phase3
						}
						else if (Skin == 30)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Phase4
						}
						else if (Skin == 31)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Emerald
						}
						else if (Skin == 32)
						{
							*pWeapon->GetFallbackPaintKit() = 561; // Lore
						}
						else if (Skin == 33)
						{
							*pWeapon->GetFallbackPaintKit() = 566; // Black Laminate
						}
						else if (Skin == 34)
						{
							*pWeapon->GetFallbackPaintKit() = 576; // Autotronic
						}
						else if (Skin == 35)
						{
							*pWeapon->GetFallbackPaintKit() = 582; // Freehand
						}

					}
					else if (Model == 4) // M9 Bayonet
					{
						int iM9Bayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
						*pWeapon->ModelIndex() = iM9Bayonet; // m_nModelIndex
						*pWeapon->ViewModelIndex() = iM9Bayonet;
						if (worldmodel) *worldmodel->ModelIndex() = iM9Bayonet + 1;
						*pWeapon->fixskins() = 508;
						*pWeapon->GetEntityQuality() = 3;
						if (cw = ItemDefinitionIndex::KNIFE_M9_BAYONET)
							pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_m9_bayonet";
						killIcons["knife_t"] = "knife_m9_bayonet";
						int Skin = Clientvariables->NewSkinchanger.KnifeSkin;
						if (Skin == 0)
						{
							*pWeapon->GetFallbackPaintKit() = 0; // Forest DDPAT
						}
						else if (Skin == 1)
						{
							*pWeapon->GetFallbackPaintKit() = 12; // Crimson Web
						}
						else if (Skin == 2)
						{
							*pWeapon->GetFallbackPaintKit() = 27; // Bone Mask
						}
						else if (Skin == 3)
						{
							*pWeapon->GetFallbackPaintKit() = 38; // Fade
						}
						else if (Skin == 4)
						{
							*pWeapon->GetFallbackPaintKit() = 40; // Night
						}
						else if (Skin == 5)
						{
							*pWeapon->GetFallbackPaintKit() = 42; // Blue Steel
						}
						else if (Skin == 6)
						{
							*pWeapon->GetFallbackPaintKit() = 43; // Stained
						}
						else if (Skin == 7)
						{
							*pWeapon->GetFallbackPaintKit() = 44; // Case Hardened
						}
						else if (Skin == 8)
						{
							*pWeapon->GetFallbackPaintKit() = 59; // Slaughter
						}
						else if (Skin == 9)
						{
							*pWeapon->GetFallbackPaintKit() = 72; // Safari Mesh
						}
						else if (Skin == 10)
						{
							*pWeapon->GetFallbackPaintKit() = 77; // Boreal Forest
						}
						else if (Skin == 11)
						{
							*pWeapon->GetFallbackPaintKit() = 98; // Ultraviolet
						}
						else if (Skin == 12)
						{
							*pWeapon->GetFallbackPaintKit() = 143; // Urban Masked
						}
						else if (Skin == 13)
						{
							*pWeapon->GetFallbackPaintKit() = 175; // Scorched
						}
						else if (Skin == 14)
						{
							*pWeapon->GetFallbackPaintKit() = 323; // Rust Coat
						}
						else if (Skin == 15)
						{
							*pWeapon->GetFallbackPaintKit() = 409; // Tiger Tooth
						}
						else if (Skin == 16)
						{
							*pWeapon->GetFallbackPaintKit() = 410; // Damascus Steel
						}
						else if (Skin == 17)
						{
							*pWeapon->GetFallbackPaintKit() = 411; // Damascus Steel
						}
						else if (Skin == 18)
						{
							*pWeapon->GetFallbackPaintKit() = 413; // Marble Fade
						}
						else if (Skin == 19)
						{
							*pWeapon->GetFallbackPaintKit() = 414; // Rust Coat
						}
						else if (Skin == 20)
						{
							*pWeapon->GetFallbackPaintKit() = 415; // Doppler Ruby
						}
						else if (Skin == 21)
						{
							*pWeapon->GetFallbackPaintKit() = 416; // Doppler Sapphire
						}
						else if (Skin == 22)
						{
							*pWeapon->GetFallbackPaintKit() = 417; // Doppler Blackpearl
						}
						else if (Skin == 23)
						{
							*pWeapon->GetFallbackPaintKit() = 418; // Doppler Phase 1
						}
						else if (Skin == 24)
						{
							*pWeapon->GetFallbackPaintKit() = 419; // Doppler Phase 2
						}
						else if (Skin == 25)
						{
							*pWeapon->GetFallbackPaintKit() = 420; // Doppler Phase 3
						}
						else if (Skin == 26)
						{
							*pWeapon->GetFallbackPaintKit() = 421; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->GetFallbackPaintKit() = 570; // Doppler Phase 4
						}
						else if (Skin == 28)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->GetFallbackPaintKit() = 569; // Gamma Doppler Phase1
						}
						else if (Skin == 28)
						{
							*pWeapon->GetFallbackPaintKit() = 570; // Gamma Doppler Phase2
						}
						else if (Skin == 29)
						{
							*pWeapon->GetFallbackPaintKit() = 571; // Gamma Doppler Phase3
						}
						else if (Skin == 30)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Phase4
						}
						else if (Skin == 31)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Emerald
						}
						else if (Skin == 32)
						{
							*pWeapon->GetFallbackPaintKit() = 562; // Lore
						}
						else if (Skin == 33)
						{
							*pWeapon->GetFallbackPaintKit() = 567; // Black Laminate
						}
						else if (Skin == 34)
						{
							*pWeapon->GetFallbackPaintKit() = 577; // Autotronic
						}
						else if (Skin == 35)
						{
							*pWeapon->GetFallbackPaintKit() = 581; // Freehand
						}

					}
					else if (Model == 8)
					{
						int iDagger = g_pModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
						*pWeapon->ModelIndex() = iDagger; // m_nModelIndex
						*pWeapon->ViewModelIndex() = iDagger;
						if (worldmodel) *worldmodel->ModelIndex() = iDagger + 1;
						*pWeapon->fixskins() = 516;
						*pWeapon->GetEntityQuality() = 3;
						if (cw = ItemDefinitionIndex::KNIFE_PUSH)
							pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_push";
						killIcons["knife_t"] = "knife_push";
						int Skin = Clientvariables->NewSkinchanger.KnifeSkin;
						if (Skin == 0)
						{
							*pWeapon->GetFallbackPaintKit() = 5; // Forest DDPAT
						}
						else if (Skin == 1)
						{
							*pWeapon->GetFallbackPaintKit() = 12; // Crimson Web
						}
						else if (Skin == 2)
						{
							*pWeapon->GetFallbackPaintKit() = 27; // Bone Mask
						}
						else if (Skin == 3)
						{
							*pWeapon->GetFallbackPaintKit() = 38; // Fade
						}
						else if (Skin == 4)
						{
							*pWeapon->GetFallbackPaintKit() = 40; // Night
						}
						else if (Skin == 5)
						{
							*pWeapon->GetFallbackPaintKit() = 42; // Blue Steel
						}
						else if (Skin == 6)
						{
							*pWeapon->GetFallbackPaintKit() = 43; // Stained
						}
						else if (Skin == 7)
						{
							*pWeapon->GetFallbackPaintKit() = 44; // Case Hardened
						}
						else if (Skin == 8)
						{
							*pWeapon->GetFallbackPaintKit() = 59; // Slaughter
						}
						else if (Skin == 9)
						{
							*pWeapon->GetFallbackPaintKit() = 72; // Safari Mesh
						}
						else if (Skin == 10)
						{
							*pWeapon->GetFallbackPaintKit() = 77; // Boreal Forest
						}
						else if (Skin == 11)
						{
							*pWeapon->GetFallbackPaintKit() = 98; // Ultraviolet
						}
						else if (Skin == 12)
						{
							*pWeapon->GetFallbackPaintKit() = 143; // Urban Masked
						}
						else if (Skin == 13)
						{
							*pWeapon->GetFallbackPaintKit() = 175; // Scorched
						}
						else if (Skin == 14)
						{
							*pWeapon->GetFallbackPaintKit() = 323; // Rust Coat
						}
						else if (Skin == 15)
						{
							*pWeapon->GetFallbackPaintKit() = 409; // Tiger Tooth
						}
						else if (Skin == 16)
						{
							*pWeapon->GetFallbackPaintKit() = 410; // Damascus Steel
						}
						else if (Skin == 17)
						{
							*pWeapon->GetFallbackPaintKit() = 411; // Damascus Steel
						}
						else if (Skin == 18)
						{
							*pWeapon->GetFallbackPaintKit() = 413; // Marble Fade
						}
						else if (Skin == 19)
						{
							*pWeapon->GetFallbackPaintKit() = 414; // Rust Coat
						}
						else if (Skin == 20)
						{
							*pWeapon->GetFallbackPaintKit() = 415; // Doppler Ruby
						}
						else if (Skin == 21)
						{
							*pWeapon->GetFallbackPaintKit() = 416; // Doppler Sapphire
						}
						else if (Skin == 22)
						{
							*pWeapon->GetFallbackPaintKit() = 417; // Doppler Blackpearl
						}
						else if (Skin == 23)
						{
							*pWeapon->GetFallbackPaintKit() = 418; // Doppler Phase 1
						}
						else if (Skin == 24)
						{
							*pWeapon->GetFallbackPaintKit() = 419; // Doppler Phase 2
						}
						else if (Skin == 25)
						{
							*pWeapon->GetFallbackPaintKit() = 420; // Doppler Phase 3
						}
						else if (Skin == 26)
						{
							*pWeapon->GetFallbackPaintKit() = 421; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->GetFallbackPaintKit() = 569; // Gamma Doppler Phase1
						}
						else if (Skin == 28)
						{
							*pWeapon->GetFallbackPaintKit() = 570; // Gamma Doppler Phase2
						}
						else if (Skin == 29)
						{
							*pWeapon->GetFallbackPaintKit() = 571; // Gamma Doppler Phase3
						}
						else if (Skin == 30)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Phase4
						}
						else if (Skin == 31)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Emerald
						}
						else if (Skin == 32)
						{
							*pWeapon->GetFallbackPaintKit() = 561; // Lore
						}
						else if (Skin == 33)
						{
							*pWeapon->GetFallbackPaintKit() = 567; // Black Laminate
						}
						else if (Skin == 34)
						{
							*pWeapon->GetFallbackPaintKit() = 574; // Autotronic
						}
						else if (Skin == 35)
						{
							*pWeapon->GetFallbackPaintKit() = 580; // Freehand
						}
					}
					else if (Model == 10)
					{
						int Navaja = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
						*pWeapon->ModelIndex() = Navaja; // m_nModelIndex
						*pWeapon->ViewModelIndex() = Navaja;
						if (worldmodel) *worldmodel->ModelIndex() = Navaja + 1;
						*pWeapon->fixskins() = 520;
						*pWeapon->GetEntityQuality() = 3;
						if (cw = 520)
							pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_gypsy_jackknife";
						killIcons["knife_t"] = "knife_gypsy_jackknife";
						int Skin = Clientvariables->NewSkinchanger.KnifeSkin;
						if (Skin == 0)
						{
							*pWeapon->GetFallbackPaintKit() = 5; // Forest DDPAT
						}
						else if (Skin == 1)
						{
							*pWeapon->GetFallbackPaintKit() = 12; // Crimson Web
						}
						else if (Skin == 2)
						{
							*pWeapon->GetFallbackPaintKit() = 27; // Bone Mask
						}
						else if (Skin == 3)
						{
							*pWeapon->GetFallbackPaintKit() = 38; // Fade
						}
						else if (Skin == 4)
						{
							*pWeapon->GetFallbackPaintKit() = 40; // Night
						}
						else if (Skin == 5)
						{
							*pWeapon->GetFallbackPaintKit() = 42; // Blue Steel
						}
						else if (Skin == 6)
						{
							*pWeapon->GetFallbackPaintKit() = 43; // Stained
						}
						else if (Skin == 7)
						{
							*pWeapon->GetFallbackPaintKit() = 44; // Case Hardened
						}
						else if (Skin == 8)
						{
							*pWeapon->GetFallbackPaintKit() = 59; // Slaughter
						}
						else if (Skin == 9)
						{
							*pWeapon->GetFallbackPaintKit() = 72; // Safari Mesh
						}
						else if (Skin == 10)
						{
							*pWeapon->GetFallbackPaintKit() = 77; // Boreal Forest
						}
						else if (Skin == 11)
						{
							*pWeapon->GetFallbackPaintKit() = 98; // Ultraviolet
						}
						else if (Skin == 12)
						{
							*pWeapon->GetFallbackPaintKit() = 143; // Urban Masked
						}
						else if (Skin == 13)
						{
							*pWeapon->GetFallbackPaintKit() = 175; // Scorched
						}
						else if (Skin == 14)
						{
							*pWeapon->GetFallbackPaintKit() = 323; // Rust Coat
						}
						else if (Skin == 15)
						{
							*pWeapon->GetFallbackPaintKit() = 409; // Tiger Tooth
						}
						else if (Skin == 16)
						{
							*pWeapon->GetFallbackPaintKit() = 410; // Damascus Steel
						}
						else if (Skin == 17)
						{
							*pWeapon->GetFallbackPaintKit() = 411; // Damascus Steel
						}
						else if (Skin == 18)
						{
							*pWeapon->GetFallbackPaintKit() = 413; // Marble Fade
						}
						else if (Skin == 19)
						{
							*pWeapon->GetFallbackPaintKit() = 414; // Rust Coat
						}
						else if (Skin == 20)
						{
							*pWeapon->GetFallbackPaintKit() = 415; // Doppler Ruby
						}
						else if (Skin == 21)
						{
							*pWeapon->GetFallbackPaintKit() = 416; // Doppler Sapphire
						}
						else if (Skin == 22)
						{
							*pWeapon->GetFallbackPaintKit() = 417; // Doppler Blackpearl
						}
						else if (Skin == 23)
						{
							*pWeapon->GetFallbackPaintKit() = 418; // Doppler Phase 1
						}
						else if (Skin == 24)
						{
							*pWeapon->GetFallbackPaintKit() = 419; // Doppler Phase 2
						}
						else if (Skin == 25)
						{
							*pWeapon->GetFallbackPaintKit() = 420; // Doppler Phase 3
						}
						else if (Skin == 26)
						{
							*pWeapon->GetFallbackPaintKit() = 421; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->GetFallbackPaintKit() = 569; // Gamma Doppler Phase1
						}
						else if (Skin == 28)
						{
							*pWeapon->GetFallbackPaintKit() = 570; // Gamma Doppler Phase2
						}
						else if (Skin == 29)
						{
							*pWeapon->GetFallbackPaintKit() = 571; // Gamma Doppler Phase3
						}
						else if (Skin == 30)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Phase4
						}
						else if (Skin == 31)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Emerald
						}
						else if (Skin == 32)
						{
							*pWeapon->GetFallbackPaintKit() = 561; // Lore
						}
						else if (Skin == 33)
						{
							*pWeapon->GetFallbackPaintKit() = 567; // Black Laminate
						}
						else if (Skin == 34)
						{
							*pWeapon->GetFallbackPaintKit() = 574; // Autotronic
						}
						else if (Skin == 35)
						{
							*pWeapon->GetFallbackPaintKit() = 580; // Freehand
						}
					}
					else if (Model == 11) // Shadow Daggers
					{
						int Stiletto = g_pModelInfo->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
						*pWeapon->ModelIndex() = Stiletto; // m_nModelIndex
						*pWeapon->ViewModelIndex() = Stiletto;
						if (worldmodel) *worldmodel->ModelIndex() = Stiletto + 1;
						*pWeapon->fixskins() = 522;
						*pWeapon->GetEntityQuality() = 3;
						if (cw = 522)
							pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_stiletto";
						killIcons["knife_t"] = "knife_stiletto";
						int Skin = Clientvariables->NewSkinchanger.KnifeSkin;
						if (Skin == 0)
						{
							*pWeapon->GetFallbackPaintKit() = 5; // Forest DDPAT
						}
						else if (Skin == 1)
						{
							*pWeapon->GetFallbackPaintKit() = 12; // Crimson Web
						}
						else if (Skin == 2)
						{
							*pWeapon->GetFallbackPaintKit() = 27; // Bone Mask
						}
						else if (Skin == 3)
						{
							*pWeapon->GetFallbackPaintKit() = 38; // Fade
						}
						else if (Skin == 4)
						{
							*pWeapon->GetFallbackPaintKit() = 40; // Night
						}
						else if (Skin == 5)
						{
							*pWeapon->GetFallbackPaintKit() = 42; // Blue Steel
						}
						else if (Skin == 6)
						{
							*pWeapon->GetFallbackPaintKit() = 43; // Stained
						}
						else if (Skin == 7)
						{
							*pWeapon->GetFallbackPaintKit() = 44; // Case Hardened
						}
						else if (Skin == 8)
						{
							*pWeapon->GetFallbackPaintKit() = 59; // Slaughter
						}
						else if (Skin == 9)
						{
							*pWeapon->GetFallbackPaintKit() = 72; // Safari Mesh
						}
						else if (Skin == 10)
						{
							*pWeapon->GetFallbackPaintKit() = 77; // Boreal Forest
						}
						else if (Skin == 11)
						{
							*pWeapon->GetFallbackPaintKit() = 98; // Ultraviolet
						}
						else if (Skin == 12)
						{
							*pWeapon->GetFallbackPaintKit() = 143; // Urban Masked
						}
						else if (Skin == 13)
						{
							*pWeapon->GetFallbackPaintKit() = 175; // Scorched
						}
						else if (Skin == 14)
						{
							*pWeapon->GetFallbackPaintKit() = 323; // Rust Coat
						}
						else if (Skin == 15)
						{
							*pWeapon->GetFallbackPaintKit() = 409; // Tiger Tooth
						}
						else if (Skin == 16)
						{
							*pWeapon->GetFallbackPaintKit() = 410; // Damascus Steel
						}
						else if (Skin == 17)
						{
							*pWeapon->GetFallbackPaintKit() = 411; // Damascus Steel
						}
						else if (Skin == 18)
						{
							*pWeapon->GetFallbackPaintKit() = 413; // Marble Fade
						}
						else if (Skin == 19)
						{
							*pWeapon->GetFallbackPaintKit() = 414; // Rust Coat
						}
						else if (Skin == 20)
						{
							*pWeapon->GetFallbackPaintKit() = 415; // Doppler Ruby
						}
						else if (Skin == 21)
						{
							*pWeapon->GetFallbackPaintKit() = 416; // Doppler Sapphire
						}
						else if (Skin == 22)
						{
							*pWeapon->GetFallbackPaintKit() = 417; // Doppler Blackpearl
						}
						else if (Skin == 23)
						{
							*pWeapon->GetFallbackPaintKit() = 418; // Doppler Phase 1
						}
						else if (Skin == 24)
						{
							*pWeapon->GetFallbackPaintKit() = 419; // Doppler Phase 2
						}
						else if (Skin == 25)
						{
							*pWeapon->GetFallbackPaintKit() = 420; // Doppler Phase 3
						}
						else if (Skin == 26)
						{
							*pWeapon->GetFallbackPaintKit() = 421; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->GetFallbackPaintKit() = 569; // Gamma Doppler Phase1
						}
						else if (Skin == 28)
						{
							*pWeapon->GetFallbackPaintKit() = 570; // Gamma Doppler Phase2
						}
						else if (Skin == 29)
						{
							*pWeapon->GetFallbackPaintKit() = 571; // Gamma Doppler Phase3
						}
						else if (Skin == 30)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Phase4
						}
						else if (Skin == 31)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Emerald
						}
						else if (Skin == 32)
						{
							*pWeapon->GetFallbackPaintKit() = 561; // Lore
						}
						else if (Skin == 33)
						{
							*pWeapon->GetFallbackPaintKit() = 567; // Black Laminate
						}
						else if (Skin == 34)
						{
							*pWeapon->GetFallbackPaintKit() = 574; // Autotronic
						}
						else if (Skin == 35)
						{
							*pWeapon->GetFallbackPaintKit() = 580; // Freehand
						}
					}
					else if (Model == 12)
					{
						int Ursus = g_pModelInfo->GetModelIndex("models/weapons/v_knife_ursus.mdl");
						*pWeapon->ModelIndex() = Ursus; // m_nModelIndex
						*pWeapon->ViewModelIndex() = Ursus;
						if (worldmodel) *worldmodel->ModelIndex() = Ursus + 1;
						*pWeapon->fixskins() = 519;
						*pWeapon->GetEntityQuality() = 3;
						if (cw = 519)
							pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_push";
						killIcons["knife_t"] = "knife_push";
						int Skin = Clientvariables->NewSkinchanger.KnifeSkin;
						if (Skin == 0)
						{
							*pWeapon->GetFallbackPaintKit() = 5; // Forest DDPAT
						}
						else if (Skin == 1)
						{
							*pWeapon->GetFallbackPaintKit() = 12; // Crimson Web
						}
						else if (Skin == 2)
						{
							*pWeapon->GetFallbackPaintKit() = 27; // Bone Mask
						}
						else if (Skin == 3)
						{
							*pWeapon->GetFallbackPaintKit() = 38; // Fade
						}
						else if (Skin == 4)
						{
							*pWeapon->GetFallbackPaintKit() = 40; // Night
						}
						else if (Skin == 5)
						{
							*pWeapon->GetFallbackPaintKit() = 42; // Blue Steel
						}
						else if (Skin == 6)
						{
							*pWeapon->GetFallbackPaintKit() = 43; // Stained
						}
						else if (Skin == 7)
						{
							*pWeapon->GetFallbackPaintKit() = 44; // Case Hardened
						}
						else if (Skin == 8)
						{
							*pWeapon->GetFallbackPaintKit() = 59; // Slaughter
						}
						else if (Skin == 9)
						{
							*pWeapon->GetFallbackPaintKit() = 72; // Safari Mesh
						}
						else if (Skin == 10)
						{
							*pWeapon->GetFallbackPaintKit() = 77; // Boreal Forest
						}
						else if (Skin == 11)
						{
							*pWeapon->GetFallbackPaintKit() = 98; // Ultraviolet
						}
						else if (Skin == 12)
						{
							*pWeapon->GetFallbackPaintKit() = 143; // Urban Masked
						}
						else if (Skin == 13)
						{
							*pWeapon->GetFallbackPaintKit() = 175; // Scorched
						}
						else if (Skin == 14)
						{
							*pWeapon->GetFallbackPaintKit() = 323; // Rust Coat
						}
						else if (Skin == 15)
						{
							*pWeapon->GetFallbackPaintKit() = 409; // Tiger Tooth
						}
						else if (Skin == 16)
						{
							*pWeapon->GetFallbackPaintKit() = 410; // Damascus Steel
						}
						else if (Skin == 17)
						{
							*pWeapon->GetFallbackPaintKit() = 411; // Damascus Steel
						}
						else if (Skin == 18)
						{
							*pWeapon->GetFallbackPaintKit() = 413; // Marble Fade
						}
						else if (Skin == 19)
						{
							*pWeapon->GetFallbackPaintKit() = 414; // Rust Coat
						}
						else if (Skin == 20)
						{
							*pWeapon->GetFallbackPaintKit() = 415; // Doppler Ruby
						}
						else if (Skin == 21)
						{
							*pWeapon->GetFallbackPaintKit() = 416; // Doppler Sapphire
						}
						else if (Skin == 22)
						{
							*pWeapon->GetFallbackPaintKit() = 417; // Doppler Blackpearl
						}
						else if (Skin == 23)
						{
							*pWeapon->GetFallbackPaintKit() = 418; // Doppler Phase 1
						}
						else if (Skin == 24)
						{
							*pWeapon->GetFallbackPaintKit() = 419; // Doppler Phase 2
						}
						else if (Skin == 25)
						{
							*pWeapon->GetFallbackPaintKit() = 420; // Doppler Phase 3
						}
						else if (Skin == 26)
						{
							*pWeapon->GetFallbackPaintKit() = 421; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->GetFallbackPaintKit() = 569; // Gamma Doppler Phase1
						}
						else if (Skin == 28)
						{
							*pWeapon->GetFallbackPaintKit() = 570; // Gamma Doppler Phase2
						}
						else if (Skin == 29)
						{
							*pWeapon->GetFallbackPaintKit() = 571; // Gamma Doppler Phase3
						}
						else if (Skin == 30)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Phase4
						}
						else if (Skin == 31)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Emerald
						}
						else if (Skin == 32)
						{
							*pWeapon->GetFallbackPaintKit() = 561; // Lore
						}
						else if (Skin == 33)
						{
							*pWeapon->GetFallbackPaintKit() = 567; // Black Laminate
						}
						else if (Skin == 34)
						{
							*pWeapon->GetFallbackPaintKit() = 574; // Autotronic
						}
						else if (Skin == 35)
						{
							*pWeapon->GetFallbackPaintKit() = 580; // Freehand
						}
					}
					else if (Model == 13)
					{
						int Talon = g_pModelInfo->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");
						*pWeapon->ModelIndex() = Talon; // m_nModelIndex
						*pWeapon->ViewModelIndex() = Talon;
						if (worldmodel) *worldmodel->ModelIndex() = Talon + 1;
						*pWeapon->fixskins() = 523;
						*pWeapon->GetEntityQuality() = 3;
						if (cw = 523)
							pWeapon->SeName(Clientvariables->NewSkinchanger.weapons[cw].ChangerName);
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_push";
						killIcons["knife_t"] = "knife_push";
						int Skin = Clientvariables->NewSkinchanger.KnifeSkin;
						if (Skin == 0)
						{
							*pWeapon->GetFallbackPaintKit() = 5; // Forest DDPAT
						}
						else if (Skin == 1)
						{
							*pWeapon->GetFallbackPaintKit() = 12; // Crimson Web
						}
						else if (Skin == 2)
						{
							*pWeapon->GetFallbackPaintKit() = 27; // Bone Mask
						}
						else if (Skin == 3)
						{
							*pWeapon->GetFallbackPaintKit() = 38; // Fade
						}
						else if (Skin == 4)
						{
							*pWeapon->GetFallbackPaintKit() = 40; // Night
						}
						else if (Skin == 5)
						{
							*pWeapon->GetFallbackPaintKit() = 42; // Blue Steel
						}
						else if (Skin == 6)
						{
							*pWeapon->GetFallbackPaintKit() = 43; // Stained
						}
						else if (Skin == 7)
						{
							*pWeapon->GetFallbackPaintKit() = 44; // Case Hardened
						}
						else if (Skin == 8)
						{
							*pWeapon->GetFallbackPaintKit() = 59; // Slaughter
						}
						else if (Skin == 9)
						{
							*pWeapon->GetFallbackPaintKit() = 72; // Safari Mesh
						}
						else if (Skin == 10)
						{
							*pWeapon->GetFallbackPaintKit() = 77; // Boreal Forest
						}
						else if (Skin == 11)
						{
							*pWeapon->GetFallbackPaintKit() = 98; // Ultraviolet
						}
						else if (Skin == 12)
						{
							*pWeapon->GetFallbackPaintKit() = 143; // Urban Masked
						}
						else if (Skin == 13)
						{
							*pWeapon->GetFallbackPaintKit() = 175; // Scorched
						}
						else if (Skin == 14)
						{
							*pWeapon->GetFallbackPaintKit() = 323; // Rust Coat
						}
						else if (Skin == 15)
						{
							*pWeapon->GetFallbackPaintKit() = 409; // Tiger Tooth
						}
						else if (Skin == 16)
						{
							*pWeapon->GetFallbackPaintKit() = 410; // Damascus Steel
						}
						else if (Skin == 17)
						{
							*pWeapon->GetFallbackPaintKit() = 411; // Damascus Steel
						}
						else if (Skin == 18)
						{
							*pWeapon->GetFallbackPaintKit() = 413; // Marble Fade
						}
						else if (Skin == 19)
						{
							*pWeapon->GetFallbackPaintKit() = 414; // Rust Coat
						}
						else if (Skin == 20)
						{
							*pWeapon->GetFallbackPaintKit() = 415; // Doppler Ruby
						}
						else if (Skin == 21)
						{
							*pWeapon->GetFallbackPaintKit() = 416; // Doppler Sapphire
						}
						else if (Skin == 22)
						{
							*pWeapon->GetFallbackPaintKit() = 417; // Doppler Blackpearl
						}
						else if (Skin == 23)
						{
							*pWeapon->GetFallbackPaintKit() = 418; // Doppler Phase 1
						}
						else if (Skin == 24)
						{
							*pWeapon->GetFallbackPaintKit() = 419; // Doppler Phase 2
						}
						else if (Skin == 25)
						{
							*pWeapon->GetFallbackPaintKit() = 420; // Doppler Phase 3
						}
						else if (Skin == 26)
						{
							*pWeapon->GetFallbackPaintKit() = 421; // Doppler Phase 4
						}
						else if (Skin == 27)
						{
							*pWeapon->GetFallbackPaintKit() = 569; // Gamma Doppler Phase1
						}
						else if (Skin == 28)
						{
							*pWeapon->GetFallbackPaintKit() = 570; // Gamma Doppler Phase2
						}
						else if (Skin == 29)
						{
							*pWeapon->GetFallbackPaintKit() = 571; // Gamma Doppler Phase3
						}
						else if (Skin == 30)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Phase4
						}
						else if (Skin == 31)
						{
							*pWeapon->GetFallbackPaintKit() = 568; // Gamma Doppler Emerald
						}
						else if (Skin == 32)
						{
							*pWeapon->GetFallbackPaintKit() = 561; // Lore
						}
						else if (Skin == 33)
						{
							*pWeapon->GetFallbackPaintKit() = 567; // Black Laminate
						}
						else if (Skin == 34)
						{
							*pWeapon->GetFallbackPaintKit() = 574; // Autotronic
						}
						else if (Skin == 35)
						{
							*pWeapon->GetFallbackPaintKit() = 580; // Freehand
						}
					}
				}
				*pWeapon->OwnerXuidLow() = 0;
				*pWeapon->OwnerXuidHigh() = 0;
				*pWeapon->FallbackWear() = 0.001f;
				*pWeapon->ItemIDHigh() = 1;
			}
			if (Clientvariables->RankChanger.semiinventar)
			{
				int Model = Clientvariables->NewSkinchanger.Knife;
				int weapon = *pWeapon->fixskins();
				switch (weapon)
				{
				case 7:
				{
					*pWeapon->GetFallbackPaintKit() = 656;
					break;
				}
				break;
				case 16: // M4A4
				{
					*pWeapon->GetFallbackPaintKit() = 632;
					break;
				}
				break;
				case 2: // dual
				{
					*pWeapon->GetFallbackPaintKit() = 544;
					break;
				}
				break;
				case 60: // M4A1
				{
					*pWeapon->GetFallbackPaintKit() = 383;
					break;
				}
				break;
				case 9: // AWP
				{
					*pWeapon->GetFallbackPaintKit() = 344;
					break;
				}
				break;
				case 61: // USP
				{
					*pWeapon->GetFallbackPaintKit() = 653;
					break;
				}
				break;
				case 4: // Glock
				{
					*pWeapon->GetFallbackPaintKit() = 586;
					break;
				}
				break;
				case 1: // Deagle
				{
					*pWeapon->GetFallbackPaintKit() = 527;
					break;
				}
				break;
				case 3: // Five Seven
				{
					*pWeapon->GetFallbackPaintKit() = 387;
					break;
				}
				break;
				case 8: // AUG
				{
					*pWeapon->GetFallbackPaintKit() = 305;
					break;
				}
				break;
				case 10: // Famas
				{
					*pWeapon->GetFallbackPaintKit() = 260;
					break;
				}
				break;
				case 11: // G3SG1
				{
					*pWeapon->GetFallbackPaintKit() = 628;
					break;
				}
				break;
				case 13: // Galil
				{
					*pWeapon->GetFallbackPaintKit() = 629;
					break;
				}
				break;
				case 14: // M249
				{
					*pWeapon->GetFallbackPaintKit() = 401;
					break;
				}
				break;
				case 17: // Mac 10
				{
					*pWeapon->GetFallbackPaintKit() = 433;
					break;
				}
				break;
				case 19: // P90
				{
					*pWeapon->GetFallbackPaintKit() = 283;
					break;
				}
				break;
				case 24: // UMP-45
				{
					*pWeapon->GetFallbackPaintKit() = 688;
					break;
				}
				break;
				case 25: // XM1014
				{
					*pWeapon->GetFallbackPaintKit() = 616;
					break;
				}
				break;
				case 63: // CZ75-Auto
				{
					*pWeapon->GetFallbackPaintKit() = 270;
					break;
				}
				break;
				case 26: // Bizon
				{
					*pWeapon->GetFallbackPaintKit() = 306;
					break;
				}
				break;
				case 27: // Mag 7
				{
					*pWeapon->GetFallbackPaintKit() = 198;
					break;
				}
				break;
				case 28: // Negev
				{
					*pWeapon->GetFallbackPaintKit() = 483;
					break;
				}
				break;
				case 29: // Sawed Off
				{
					*pWeapon->GetFallbackPaintKit() = 434;
					break;
				}
				break;
				case 30: // Tec 9
				{
					*pWeapon->GetFallbackPaintKit() = 652;
					break;
				}
				break;
				case 32: // P2000
				{
					*pWeapon->GetFallbackPaintKit() = 338;
					break;
				}
				break;
				case 33: // MP7
				{
					*pWeapon->GetFallbackPaintKit() = 481;
					break;
				}
				break;
				case 34: // MP9
				{
					*pWeapon->GetFallbackPaintKit() = 262;
					break;
				}
				break;
				case 35: // Nova
				{
					*pWeapon->GetFallbackPaintKit() = 450;
					break;
				}
				break;
				case 36: // P250
				{
					*pWeapon->GetFallbackPaintKit() = 404;
					break;
				}
				break;
				case 38: // Scar 20
				{
					*pWeapon->GetFallbackPaintKit() = 232;
					break;
				}
				break;
				case 39: // SG553
				{
					*pWeapon->GetFallbackPaintKit() = 311;
					break;
				}
				break;
				case 40: // SSG08
				{
					*pWeapon->GetFallbackPaintKit() = 538;
					break;
				}
				break;
				case 64: // Revolver
				{
					*pWeapon->GetFallbackPaintKit() = 538;
					break;
				}
				break;
				default:
					break;
				}

				ClientClass* pClass = (ClientClass*)pEntity->GetClientClass();
				if ((int)pClass->m_ClassID == (int)93)
				{
					auto pCustomName1 = MakePtr(char*, pWeapon, 0x301C);
					worldmodel_handle = pWeapon->m_hWeaponWorldModel();
					if (worldmodel_handle) worldmodel = (CBaseCombatWeapon*)g_pEntitylist->GetClientEntityFromHandle(worldmodel_handle);
					{
						int iM9Bayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
						*pWeapon->ModelIndex() = iM9Bayonet; // m_nModelIndex
						*pWeapon->ViewModelIndex() = iM9Bayonet;
						if (worldmodel) *worldmodel->ModelIndex() = iM9Bayonet + 1;
						*pWeapon->fixskins() = 508;
						*pWeapon->GetEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "knife_m9_bayonet";
						killIcons["knife_t"] = "knife_m9_bayonet";
						*pWeapon->GetFallbackPaintKit() = 622;
					}

				}
				*pWeapon->OwnerXuidLow() = 0;
				*pWeapon->OwnerXuidHigh() = 0;
				*pWeapon->FallbackWear() = 0.001f;
				*pWeapon->ItemIDHigh() = 1;
			}
		}
	}
}