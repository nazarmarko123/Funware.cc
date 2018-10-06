#include "sdk.h"
#include "hooks.h"
#include "Menu.h"
#include "global.h"
#include "Hitmarker.h"
#include "ESP.h"
#include "ImGUI\imconfig.h"
#include "ImGUI\imgui.h"
#include "ImGUI\imgui_internal.h"
#include "ImGUI\stb_rect_pack.h"
#include "ImGUI\stb_textedit.h"
#include "ImGUI\stb_truetype.h"
#include "ImGUI\DX9\imgui_impl_dx9.h"
#include "Items.h"
#include "Config.h"
#include "GameUtils.h"
#include "icons/LegitBar.h"
#include "icons/MiscBar.h"
#include "icons/RageBar.h"
#include "icons/TitleBar.h"
#include "icons/VisualsBar.h"
#include "icons/SaveBar.h"
#include "item.h"
#include "Skinchanger/NewSkinchanger.h"
#include "RankChanger/Protobuffs.h"
#include "EventLog/NewEventLog.h"
IDirect3DTexture9 *TittleBar = nullptr;
IDirect3DTexture9 *LegitTabImg = nullptr;
IDirect3DTexture9 *RageTabImg = nullptr;
IDirect3DTexture9 *VisualsTabImg = nullptr;
IDirect3DTexture9 *MiscTabImg = nullptr;
IDirect3DTexture9 *SaveTabImg = nullptr;

int windowWidth = 800;
int windowHeight = 425;

int windowWidth2 = 650;
int windowHeight2 = 485;

int curWidth = windowWidth;
int curHeight = windowHeight;

namespace ImGui
{

	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	IMGUI_API bool ComboBoxArray(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}
}

void DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow)
{
	ImDrawList* windowDrawList = ImGui::GetWindowDrawList();

	Color colColor(255, 255, 255, 255);

	flRainbow += flSpeed;
	if (flRainbow > 1.f) flRainbow = 0.f;

	for (int i = 0; i < width; i++)
	{
		float hue = (1.f / (float)width) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1.f;

		Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		windowDrawList->AddRectFilled(ImVec2(x + i, y), ImVec2(width, height), colRainbow.GetU32());
	}
}
struct hud_weapons_t {
	std::int32_t* get_weapon_count() {
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};
template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(FindPatternIDA("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 85 C0 0F 84 ? ? ? ? 8D 58") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(FindPatternIDA("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39"));
	return (T*)find_hud_element(pThis, name);
}
void KnifeApplyCallbk()
{
	
	static auto clear_hud_weapon_icon_fn =
		reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(
			FindPatternIDA("client_panorama.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C"));

	auto element = FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");

	auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0x9c);
	if (hud_weapons == nullptr)
		return;

	if (!*hud_weapons->get_weapon_count())
		return;

	for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
		i = clear_hud_weapon_icon_fn(hud_weapons, i);

	/*
	typedef void(*ForceUpdate) (void);
	ForceUpdate FullUpdate = (ForceUpdate)FindSignaturenew("engine.dll", "FullUpdate", "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
	FullUpdate();
	*/
	typedef void(*CL_FullUpdate_t) (void);
	static CL_FullUpdate_t cl_fullupdate = (CL_FullUpdate_t)(offys.IDA_FullUpdate);
	cl_fullupdate();
	
}

PresentFn oPresent;

ImFont* Impact;
ImFont* Default;
ImFont* Tabs;
ImFont* Tabs2;

tReset oResetScene;

void GUI_Init(IDirect3DDevice9* pDevice) // Setup for imgui
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplDX9_Init(G::Window, pDevice);

	ImGuiStyle& style = ImGui::GetStyle();

	Impact = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Impact.ttf", 32.0f);
	Default = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\micross.ttf", 14.0f);
	Tabs = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Skeet-tabs.ttf", 60.0f);
	Tabs2 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\SkeetFont.ttf", 60.0f);

	ImGuiStyle& styled = ImGui::GetStyle();

	styled.WindowPadding = ImVec2(8, 8);
	styled.WindowMinSize = ImVec2(32, 32);
	styled.WindowRounding = 0.0f;
	styled.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	styled.ChildWindowRounding = 0.0f;
	styled.FramePadding = ImVec2(4, 1);
	styled.FrameRounding = 0.0f;
	styled.ItemSpacing = ImVec2(15, 4);
	styled.ItemInnerSpacing = ImVec2(4, 4);
	styled.TouchExtraPadding = ImVec2(0, 0);
	styled.IndentSpacing = 21.0f;
	styled.ColumnsMinSpacing = 3.0f;
	styled.ScrollbarSize = 12.0f;
	styled.ScrollbarRounding = 0.0f;
	styled.GrabMinSize = 0.1f;
	styled.GrabRounding = 16.0f;
	styled.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	styled.DisplayWindowPadding = ImVec2(22, 22);
	styled.DisplaySafeAreaPadding = ImVec2(4, 4);
	styled.AntiAliasedLines = true;
	styled.AntiAliasedShapes = true;
	styled.CurveTessellationTol = 1.25f;

	G::Init = true;
}

void color()
{
	ImGuiStyle& styled = ImGui::GetStyle();

	ImVec4 Color_Title_Text = ImColor(165, 75, 100);

	int MenuRed, MenuGreen, MenuBlue;

	MenuRed = 255;
	MenuGreen = 255;
	MenuBlue = 255;

	ImColor mainColor = ImColor(MenuRed, MenuGreen, MenuBlue);//ImColor(MenuRed, MenuGreen, MenuBlue, 255);
	ImColor bodyColor = ImColor(24, 24, 24, 239);
	ImColor fontColor = ImColor(150, 150, 150, 255);


	ImVec4 mainColorHovered = ImVec4(mainColor.Value.x + 0.1f, mainColor.Value.y + 0.1f, mainColor.Value.z + 0.1f, mainColor.Value.w);
	ImVec4 mainColorActive = ImVec4(mainColor.Value.x + 0.2f, mainColor.Value.y + 0.2f, mainColor.Value.z + 0.2f, mainColor.Value.w);
	ImVec4 menubarColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w - 0.8f);
	ImVec4 frameBgColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w + .1f);
	ImVec4 tooltipBgColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w + .05f);

	styled.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
	styled.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
	styled.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.f);
	styled.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
	styled.Colors[ImGuiCol_Border] = ImColor(15, 30, 15, 255);//ImVec4(0.15f, 0.15f, 0.15f, 1.f);
	styled.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	styled.Colors[ImGuiCol_FrameBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.f);
	styled.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.22f, 1.f);
	styled.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.f);
	styled.Colors[ImGuiCol_TitleBg] = ImColor(38, 40, 42, 255);
	styled.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(MenuRed, MenuGreen, MenuBlue, 255);
	styled.Colors[ImGuiCol_TitleBgActive] = ImColor(38, 40, 42, 255);
	styled.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
	styled.Colors[ImGuiCol_ScrollbarBg] = ImVec4(frameBgColor.x + .1f, frameBgColor.y + .1f, frameBgColor.z + .1f, frameBgColor.w);
	styled.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.f);
	styled.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.f);
	styled.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.f);
	styled.Colors[ImGuiCol_ComboBg] = frameBgColor;
	styled.Colors[ImGuiCol_CheckMark] = ImColor(68, 140, 255);
	styled.Colors[ImGuiCol_SliderGrab] = ImColor(68, 140, 255);
	styled.Colors[ImGuiCol_SliderGrabActive] = ImColor(68, 140, 255);
	styled.Colors[ImGuiCol_Button] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
	styled.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
	styled.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
	styled.Colors[ImGuiCol_Header] = ImColor(68, 140, 255);
	styled.Colors[ImGuiCol_HeaderHovered] = ImColor(68, 140, 255);
	styled.Colors[ImGuiCol_HeaderActive] = ImColor(68, 140, 255);
	styled.Colors[ImGuiCol_CloseButton] = ImColor(255, 255, 255, 255);
	styled.Colors[ImGuiCol_CloseButtonHovered] = ImColor(255, 255, 255, 255);
	styled.Colors[ImGuiCol_CloseButtonActive] = ImColor(255, 255, 255, 255);
	styled.Colors[ImGuiCol_Column] = ImVec4(.30f, .30f, .30f, 1.f);
	styled.Colors[ImGuiCol_ColumnHovered] = mainColorHovered;
	styled.Colors[ImGuiCol_ColumnActive] = mainColorActive;
}

void TextColor(bool active)
{
	auto& style = ImGui::GetStyle();

	if (active)
		style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);
	else
		style.Colors[ImGuiCol_Text] = ImVec4(0.39f, 0.39f, 0.39f, 1.f);
}


void buttonColor(bool bActive)
{
	ImGuiStyle& style = ImGui::GetStyle();

	if (bActive)
	{
		style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_WindowBg];
	}
	else
	{
		style.Colors[ImGuiCol_Button] = ImColor(10, 10, 10);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(10, 10, 10);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(10, 10, 10);
	}
}

void lineColor(bool bActive)
{
	ImGuiStyle& style = ImGui::GetStyle();

	if (bActive)
	{
		style.Colors[ImGuiCol_Button] = ImColor(120, 120, 120);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(120, 120, 120);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(120, 120, 120);
	}
	else
	{
		style.Colors[ImGuiCol_Button] = ImColor(10, 10, 10);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(10, 10, 10);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(10, 10, 10);
	}
}

void lineColor1(bool bActive)
{
	ImGuiStyle& style = ImGui::GetStyle();

	if (bActive)
	{
		style.Colors[ImGuiCol_Button] = ImColor(120, 120, 120);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(120, 120, 120);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(120, 120, 120);
	}
	else
	{
		style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_WindowBg];
	}
}


void Border(bool active)
{
	auto& style = ImGui::GetStyle();

	if (active)
		style.Colors[ImGuiCol_Border] = ImColor(80, 80, 80, 255);
	else
		style.Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);
}

void BtnNormal()
{
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImVec4(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_Text] = ImVec4(0.78431372549f, 0.78431372549f, 0.78431372549f, 1.f);

}

void BtnActive()
{
	int MenuRed, MenuGreen, MenuBlue;

	MenuRed = Clientvariables->Colors.MenuColor[0] * 255;
	MenuGreen = Clientvariables->Colors.MenuColor[1] * 255;
	MenuBlue = Clientvariables->Colors.MenuColor[2] * 255;

	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImColor(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_Text] = ImColor(MenuRed, MenuGreen, MenuBlue, 255);
}

void ragebot()
{
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_ChildWindowBg] = ImColor(18, 18, 18, 100);
	ImGui::BeginChild(XorStr("RagebotChild1"), ImVec2(ImGui::GetWindowWidth() / 3 - 10, 0), true, G::ShowMenu ? ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar : ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
	{
		ImGui::Checkbox("Enable", &Clientvariables->Ragebot.EnableAimbot);
		ImGui::Text("Selection");
		ImGui::Combo("##selection", &Clientvariables->Ragebot.AimbotSelection, SelectionMode, ARRAYSIZE(SelectionMode));
		ImGui::Checkbox("Automatic fire", &Clientvariables->Ragebot.AutomaticFire);
		ImGui::Checkbox("AutoScope", &Clientvariables->Ragebot.AutomaticScope);
		ImGui::Checkbox("AutoWall", &Clientvariables->Ragebot.Autowall);
		ImGui::Checkbox("Remove spread", &Clientvariables->Ragebot.NoSpread);
		ImGui::Checkbox("Remove recoil", &Clientvariables->Ragebot.NoRecoil);
		ImGui::Checkbox("Silent aim", &Clientvariables->Ragebot.SilentAimbot);
		
		short cw = SafeWeaponID();
		ImGui::Text("Weapon ->"); ImGui::SameLine(); ImGui::Text(GetWeaponNameById(cw));
		{
			ImGui::Text("Minimum hit chance");
			{
				if (cw == ItemDefinitionIndex::AK47)
					ImGui::SliderInt("##xdxd", &Clientvariables->Weaponxdfix.AK47hit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::AUG)
					ImGui::SliderInt("##xdxd1", &Clientvariables->Weaponxdfix.AUGhit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::AWP)
					ImGui::SliderInt("##xdxd2", &Clientvariables->Weaponxdfix.AWPhit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::BIZON)
					ImGui::SliderInt("##xdxd3", &Clientvariables->Weaponxdfix.Bizonhit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::CZ75A)
					ImGui::SliderInt("##xdxd4", &Clientvariables->Weaponxdfix.Cz75hit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::DEAGLE)
					ImGui::SliderInt("##xdxd5", &Clientvariables->Weaponxdfix.Deaglehit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::ELITE)
					ImGui::SliderInt("##xdxd6", &Clientvariables->Weaponxdfix.Dualhit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::FAMAS)
					ImGui::SliderInt("##xdxder", &Clientvariables->Weaponxdfix.FAMAShit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::FIVESEVEN)
					ImGui::SliderInt("##xdxd23", &Clientvariables->Weaponxdfix.Fivehit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::G3SG1)
					ImGui::SliderInt("##xdxd2ef", &Clientvariables->Weaponxdfix.G3sg1hit, 10, 100, "%.0f%%");


				if (cw == ItemDefinitionIndex::GALILAR)
					ImGui::SliderInt("##xdxd34twef", &Clientvariables->Weaponxdfix.Galilhit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::GLOCK)
					ImGui::SliderInt("##xdxd24wfeds", &Clientvariables->Weaponxdfix.Glockhit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::HKP2000)
					ImGui::SliderInt("##xdxdt4fwervgvs", &Clientvariables->Weaponxdfix.P2000hit, 10, 100, "%.0f%%");


				if (cw == ItemDefinitionIndex::M249)
					ImGui::SliderInt("##xdxdthergds", &Clientvariables->Weaponxdfix.M249hit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::M4A1)
					ImGui::SliderInt("##xdxdlikj", &Clientvariables->Weaponxdfix.M4A4hit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::M4A1_SILENCER)
					ImGui::SliderInt("##xdxd2w", &Clientvariables->Weaponxdfix.M4A1Shit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::MAC10)
					ImGui::SliderInt("##xdxdj6e5htrbv", &Clientvariables->Weaponxdfix.Mac10hit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::MAG7)
					ImGui::SliderInt("##xdxdukyjhgnb", &Clientvariables->Weaponxdfix.Maghit, 10, 100, "%.0f%%");


				if (cw == ItemDefinitionIndex::MP7)
					ImGui::SliderInt("##xdxdefbv", &Clientvariables->Weaponxdfix.Mp7hit, 10, 100, "%.0f%%");
				if (cw == ItemDefinitionIndex::MP9)
					ImGui::SliderInt("##xdxdtnbvcxvbn", &Clientvariables->Weaponxdfix.Mp9hit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::NEGEV)
					ImGui::SliderInt("##xdxdzqfqw", &Clientvariables->Weaponxdfix.Negevhit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::NOVA)
					ImGui::SliderInt("##xdxdynbrtvsf", &Clientvariables->Weaponxdfix.Novahit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::XM1014)
					ImGui::SliderInt("##xdxdmbvg", &Clientvariables->Weaponxdfix.Xmhit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::USP_SILENCER)
					ImGui::SliderInt("##xdxddfgsf", &Clientvariables->Weaponxdfix.USPhit, 10, 100, "%.0f%%");


				if (cw == ItemDefinitionIndex::UMP45)
					ImGui::SliderInt("##xdxdergdfsvwefg", &Clientvariables->Weaponxdfix.UMP45hit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::TEC9)
					ImGui::SliderInt("##xdxdegvergw", &Clientvariables->Weaponxdfix.tec9hit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::SSG08)
					ImGui::SliderInt("##xdxdwgsdfbdfhwg", &Clientvariables->Weaponxdfix.SSG08hit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::SG556)
					ImGui::SliderInt("##xdxderhdrfvssdv", &Clientvariables->Weaponxdfix.Sg553hit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::SCAR20)
					ImGui::SliderInt("##xdxdehbvsdgrwsg", &Clientvariables->Weaponxdfix.SCAR20hit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::SAWEDOFF)
					ImGui::SliderInt("##xdxdaehsvsverhg", &Clientvariables->Weaponxdfix.Sawedhit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::REVOLVER)
					ImGui::SliderInt("##xdxdewrghwrvwv", &Clientvariables->Weaponxdfix.Revolverhit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::P90)
					ImGui::SliderInt("##xdxdwgsbsffbs", &Clientvariables->Weaponxdfix.P90hit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::P250)
					ImGui::SliderInt("##xdxddfgsdvwegw", &Clientvariables->Weaponxdfix.P250hit, 10, 100, "%.0f%%");
			}
			
			ImGui::Text("Minimum damage");
			{
				if (cw == ItemDefinitionIndex::AK47)
					ImGui::SliderInt("##xdxdwgsdfaehe", &Clientvariables->Weaponxdfix.AK47dmg, 1, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::AUG)
					ImGui::SliderInt("##xdxdasdcqefqef", &Clientvariables->Weaponxdfix.AUGdmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::AWP)
					ImGui::SliderInt("##xdxdfsdgvehwr", &Clientvariables->Weaponxdfix.AWPdmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::BIZON)
					ImGui::SliderInt("##xdxddfhsrtheghe", &Clientvariables->Weaponxdfix.Bizondmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::CZ75A)
					ImGui::SliderInt("##xdxdfdhfdhbadfb", &Clientvariables->Weaponxdfix.Cz75dmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::DEAGLE)
					ImGui::SliderInt("##xdxddfgaegfefg", &Clientvariables->Weaponxdfix.Deagledmg, 10, 110, "%.0f%%");

				if (cw == ItemDefinitionIndex::ELITE)
					ImGui::SliderInt("##xdxdafhgsegfdfs", &Clientvariables->Weaponxdfix.Dualdmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::FAMAS)
					ImGui::SliderInt("##xdxdgfnbdghmhj", &Clientvariables->Weaponxdfix.FAMASdmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::FIVESEVEN)
					ImGui::SliderInt("##xdxdassasad", &Clientvariables->Weaponxdfix.Fivedmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::G3SG1)
					ImGui::SliderInt("##xdxdgkjfyukyu", &Clientvariables->Weaponxdfix.G3sg1dmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::GALILAR)
					ImGui::SliderInt("##ak47hitvaergaergaerg", &Clientvariables->Weaponxdfix.Galildmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::GLOCK)
					ImGui::SliderInt("##ak47hitagersg", &Clientvariables->Weaponxdfix.Glockdmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::HKP2000)
					ImGui::SliderInt("##gsdfhfgsreh", &Clientvariables->Weaponxdfix.P2000dmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::M249)
					ImGui::SliderInt("##ak47hit", &Clientvariables->Weaponxdfix.M249dmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::M4A1)
					ImGui::SliderInt("##egrserge", &Clientvariables->Weaponxdfix.M4A4dmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::M4A1_SILENCER)
					ImGui::SliderInt("##xngcxnvx", &Clientvariables->Weaponxdfix.M4A1Sdmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::MAC10)
					ImGui::SliderInt("##zxvsdfvasdfv", &Clientvariables->Weaponxdfix.Mac10dmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::MAG7)
					ImGui::SliderInt("##bghnhgmnghd", &Clientvariables->Weaponxdfix.Magdmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::MP7)
					ImGui::SliderInt("##zvzdfbvd", &Clientvariables->Weaponxdfix.Mp7dmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::MP9)
					ImGui::SliderInt("##dsfgergfdhsb", &Clientvariables->Weaponxdfix.Mp9dmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::NEGEV)
					ImGui::SliderInt("##ak4sdgsadga7hit", &Clientvariables->Weaponxdfix.Negevdmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::NOVA)
					ImGui::SliderInt("##ak47hit", &Clientvariables->Weaponxdfix.Novadmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::XM1014)
					ImGui::SliderInt("##aksdfds47hit", &Clientvariables->Weaponxdfix.Xmdmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::USP_SILENCER)
					ImGui::SliderInt("##ak47dfhgdfhghit", &Clientvariables->Weaponxdfix.USPdmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::TEC9)
					ImGui::SliderInt("##aksfdsdfasgddx47hit", &Clientvariables->Weaponxdfix.tec9dmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::SSG08)
					ImGui::SliderInt("##ak47zxcasfhit", &Clientvariables->Weaponxdfix.SSG08dmg, 10, 110, "%.0f%%");

				if (cw == ItemDefinitionIndex::SG556)
					ImGui::SliderInt("##ak47hgwgwit", &Clientvariables->Weaponxdfix.Sg553dmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::SCAR20)
					ImGui::SliderInt("##ak47asdfqewffhit", &Clientvariables->Weaponxdfix.SCAR20dmg, 10, 110, "%.0f%%");

				if (cw == ItemDefinitionIndex::SAWEDOFF)
					ImGui::SliderInt("##ak4asfasdfdsg7hit", &Clientvariables->Weaponxdfix.Sawedhit, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::REVOLVER)
					ImGui::SliderInt("##ak47htykmgnfbhit", &Clientvariables->Weaponxdfix.Revolverdmg, 10, 110, "%.0f%%");

				if (cw == ItemDefinitionIndex::P90)
					ImGui::SliderInt("##ak4zxcvxcbc7hit", &Clientvariables->Weaponxdfix.P90dmg, 10, 100, "%.0f%%");

				if (cw == ItemDefinitionIndex::P250)
					ImGui::SliderInt("##ak4cvncvm7hit", &Clientvariables->Weaponxdfix.P250dmg, 10, 100, "%.0f%%");
			}


			ImGui::Checkbox("Automatic resolver", &Clientvariables->Ragebot.AutomaticResolver);
			ImGui::Checkbox("Perfect resolver", &Clientvariables->Ragebot.perfectresolver);
			ImGui::Checkbox("Engine prediction", &Clientvariables->Ragebot.EnginePred);

		}
		//old shit but work still xd

		/*
		ImGui::Text("Minimum hit chance");
		ImGui::SliderInt("##hitchance", &Clientvariables->Ragebot.Minhitchance, 0, 100, "%.0f%%");

		ImGui::Text("Minimum damage");
		ImGui::SliderInt("##mindamage", &Clientvariables->Ragebot.Mindamage, 1, 101, "%.0f%%");
		*/



		

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(XorStr("RagebotChild2"), ImVec2(ImGui::GetWindowWidth() / 3 - 10, 0), true, G::ShowMenu ? ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar : ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
	{
		//old antiaim xd
		/*
		ImGui::Checkbox("Enable", &Clientvariables->Antiaim.AntiaimEnable);
		ImGui::Text("Pitch");
		ImGui::Combo("##pitch", &Clientvariables->Antiaim.Pitch, AntiaimbotPitch, ARRAYSIZE(AntiaimbotPitch));

		ImGui::Text("Anti-aimbot");
		ImGui::Combo("##freestanding", &Clientvariables->Antiaim.Freestanding, Freestanding, ARRAYSIZE(Freestanding));

		ImGui::Spacing();
		if (Clientvariables->Antiaim.Freestanding == 1)
		{
			ImGui::Text("Yaw");
			ImGui::Combo("##yaw", &Clientvariables->Antiaim.Yaw, AntiaimbotYaw, ARRAYSIZE(AntiaimbotYaw));
			ImGui::SliderInt("##yawadd", &Clientvariables->Antiaim.YawAdd, -180, 180);



			ImGui::Text("Move");
			ImGui::Combo("##yawrunning", &Clientvariables->Antiaim.YawRunning, AntiaimbotYawRun, ARRAYSIZE(AntiaimbotYawRun));
			ImGui::SliderInt("##yawrunadd", &Clientvariables->Antiaim.YawRunningAdd, -180, 180);

			ImGui::Text("Fake");
			ImGui::Combo("##fakeyaw", &Clientvariables->Antiaim.FakeYaw, AntiaimbotYawFake, ARRAYSIZE(AntiaimbotYawFake));
			ImGui::SliderInt("##yawfakeadd", &Clientvariables->Antiaim.FakeYawAdd, -180, 180);



			ImGui::Text("LBY");
			ImGui::SliderInt("##freestand", &Clientvariables->Antiaim.FreestandingDelta, -180, 180);

			ImGui::Text("At Target");
			ImGui::Combo("##dynamic", &Clientvariables->Antiaim.AtPlayer, AtTarget, ARRAYSIZE(AtTarget));
		}
		if (Clientvariables->Antiaim.Freestanding == 2)
		{
			ImGui::Text("Right key");
			ImGui::Hotkey("##rightkey", &Clientvariables->Misc.Rightside, ImVec2(150, 20));

			ImGui::Text("Left key");
			ImGui::Hotkey("##leftkey", &Clientvariables->Misc.Leftside, ImVec2(150, 20));

			ImGui::Text("Back key");
			ImGui::Hotkey("##backkey", &Clientvariables->Misc.Backside, ImVec2(150, 20));

			ImGui::Text("Fake");
			ImGui::Combo("##fakeyaw", &Clientvariables->Antiaim.FakeYaw, AntiaimbotYawFake, ARRAYSIZE(AntiaimbotYawFake));
			ImGui::SliderInt("##yawfakeadd", &Clientvariables->Antiaim.FakeYawAdd, -180, 180);


			ImGui::Text("LBY");
			ImGui::SliderInt("##freestand", &Clientvariables->Antiaim.FreestandingDelta, -180, 180);
		}
		if (Clientvariables->Antiaim.Freestanding == 3)
		{
			ImGui::Text("Right key");
			ImGui::Hotkey("##rightkey", &Clientvariables->Misc.Rightside, ImVec2(150, 20));

			ImGui::Text("Left key");
			ImGui::Hotkey("##leftkey", &Clientvariables->Misc.Leftside, ImVec2(150, 20));

			ImGui::Text("Back key");
			ImGui::Hotkey("##backkey", &Clientvariables->Misc.Backside, ImVec2(150, 20));

			ImGui::Text("Fake");
			ImGui::Combo("##fakeyaw", &Clientvariables->Antiaim.FakeYaw, AntiaimbotYawFake, ARRAYSIZE(AntiaimbotYawFake));
			ImGui::SliderInt("##yawfakeadd", &Clientvariables->Antiaim.FakeYawAdd, -180, 180);

			ImGui::Text("LBY");
			ImGui::SliderInt("##freestand", &Clientvariables->Antiaim.FreestandingDelta, -180, 180);
		}
		*/

		
		ImGui::Checkbox("Anti-aim enabled", &Clientvariables->NewAntiAim.antiaim_enabled);
		if (Clientvariables->NewAntiAim.antiaim_enabled)
		{
			ImGui::Combo("##xdez", &Clientvariables->aachange, aat, ARRAYSIZE(aat));

			if (Clientvariables->aachange == 0)
			{
				ImGui::Checkbox("At target##standing at target", &Clientvariables->NewAntiAim.antiaim_standing_at_target_enabled);

				ImGui::Combo("Pitch##standing pitch", &Clientvariables->NewAntiAim.antiaim_standing_pitch, antiaim_pitch_types, ARRAYSIZE(antiaim_pitch_types));

				ImGui::Text("Real");
				ImGui::Combo("Yaw##standing real yaw", &Clientvariables->NewAntiAim.antiaim_standing_real_yaw, antiaim_real_types, ARRAYSIZE(antiaim_real_types));
				/* REAL */

				ImGui::newSliderFloat("Offset##standing real offset", &Clientvariables->NewAntiAim.antiaim_standing_real_offset, -179.f, 179.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
				ImGui::newSliderFloat("LBY delta##standing lby delta", &Clientvariables->NewAntiAim.antiaim_standing_real_lby_delta, -179.f, 179.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
				ImGui::newSliderFloat("Jitter range##standing real jitter range", &Clientvariables->NewAntiAim.antiaim_standing_real_jrange, 0, 360.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));



				/// spin
				if (Clientvariables->NewAntiAim.antiaim_standing_real_yaw == 4)
					ImGui::newSliderFloat("Spin speed##standing real spin speed", &Clientvariables->NewAntiAim.antiaim_standing_real_spin_speed, -20.0f, 20.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
				else
					Clientvariables->NewAntiAim.antiaim_standing_real_spin_speed = 0.f;

				/* FAKE */
				ImGui::Text("Fake");

				ImGui::Combo("Yaw##standing fake yaw", &Clientvariables->NewAntiAim.antiaim_standing_fake_yaw, antiaim_fake_types, ARRAYSIZE(antiaim_fake_types));

				ImGui::newSliderFloat("Offset##standing fake offset", &Clientvariables->NewAntiAim.antiaim_standing_fake_offset, -179.f, 179.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
				ImGui::newSliderFloat("Jitter range##standing fake jitter range", &Clientvariables->NewAntiAim.antiaim_standing_fake_jrange, 0, 360.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));


				if (Clientvariables->NewAntiAim.antiaim_standing_fake_yaw == 4)
					ImGui::newSliderFloat("Spin speed##standing real spin speed", &Clientvariables->NewAntiAim.antiaim_standing_fake_spin_speed, -20.0f, 20.0f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
				else
					Clientvariables->NewAntiAim.antiaim_standing_fake_spin_speed = 0.f;
			}
			if (Clientvariables->aachange == 1)
			{

				ImGui::Checkbox("At target##moving at target", &Clientvariables->NewAntiAim.antiaim_moving_at_target_enabled);

				ImGui::Combo("Pitch##moving pitch", &Clientvariables->NewAntiAim.antiaim_moving_pitch, antiaim_pitch_types, ARRAYSIZE(antiaim_pitch_types));

				ImGui::Text("Real");

				ImGui::Combo("Yaw##moving real yaw", &Clientvariables->NewAntiAim.antiaim_moving_real_yaw, antiaim_real_types, ARRAYSIZE(antiaim_real_types));

				ImGui::newSliderFloat("Offset##moving real offset", &Clientvariables->NewAntiAim.antiaim_moving_real_offset, -179.f, 179.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
				ImGui::newSliderFloat("Jitter range##moving real jitter range", &Clientvariables->NewAntiAim.antiaim_moving_real_jrange, 0, 360.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
				/// spin
				if (Clientvariables->NewAntiAim.antiaim_moving_real_yaw == 4)
					ImGui::newSliderFloat("Spin speed##moving real spin speed", &Clientvariables->NewAntiAim.antiaim_moving_real_spin_speed, -20.f, 20.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
				else
					Clientvariables->NewAntiAim.antiaim_moving_real_spin_speed = 0.f;

				/* FAKE */
				ImGui::Text("Fake");

				ImGui::Combo("Yaw##moving fake yaw", &Clientvariables->NewAntiAim.antiaim_moving_fake_yaw, antiaim_fake_types, ARRAYSIZE(antiaim_fake_types));

				ImGui::newSliderFloat("Offset##moving fake offset", &Clientvariables->NewAntiAim.antiaim_moving_fake_offset, -179.f, 179.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
				ImGui::newSliderFloat("Jitter range##moving fake jitter range", &Clientvariables->NewAntiAim.antiaim_moving_fake_jrange, 0, 360.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));


			

				/// spin
				if (Clientvariables->NewAntiAim.antiaim_moving_fake_yaw == 4)
					ImGui::newSliderFloat("Spin speed##moving real spin speed", &Clientvariables->NewAntiAim.antiaim_moving_fake_spin_speed, -20.f, 20.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
				else
					Clientvariables->NewAntiAim.antiaim_moving_fake_spin_speed = 0.f;

			}
			if (Clientvariables->aachange == 2)
			{

				ImGui::Checkbox("At target##jumping at target", &Clientvariables->NewAntiAim.antiaim_jumping_at_target_enabled);


				ImGui::Combo("Pitch##jumping pitch", &Clientvariables->NewAntiAim.antiaim_jumping_pitch, antiaim_pitch_types, ARRAYSIZE(antiaim_pitch_types));



				/* REAL */
				ImGui::Text("Real");

				ImGui::Combo("Yaw##jumping real yaw", &Clientvariables->NewAntiAim.antiaim_jumping_real_yaw, antiaim_real_types, ARRAYSIZE(antiaim_real_types));


				ImGui::newSliderFloat("Offset##jumping real offset", &Clientvariables->NewAntiAim.antiaim_jumping_real_offset, -179.f, 179.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
				ImGui::newSliderFloat("Jitter range##jumping real jitter range", &Clientvariables->NewAntiAim.antiaim_jumping_real_jrange, 0, 360.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));

				/// spin
				if (Clientvariables->NewAntiAim.antiaim_jumping_real_yaw == 4)
					ImGui::newSliderFloat("Spin speed##jumping real spin speed", &Clientvariables->NewAntiAim.antiaim_jumping_real_spin_speed, -20.f, 20.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
				else
					Clientvariables->NewAntiAim.antiaim_jumping_real_spin_speed = 0.f;

				/* FAKE */
				ImGui::Text("Fake");

				ImGui::Combo("Yaw##jumping fake yaw", &Clientvariables->NewAntiAim.antiaim_jumping_fake_yaw, antiaim_fake_types, ARRAYSIZE(antiaim_fake_types));


				ImGui::newSliderFloat("Offset##jumping fake offset", &Clientvariables->NewAntiAim.antiaim_jumping_fake_offset, -179.f, 179.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
				ImGui::newSliderFloat("Jitter range##jumping fake jitter range", &Clientvariables->NewAntiAim.antiaim_jumping_fake_jrange, 0, 360.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));

				/// spin
				if (Clientvariables->NewAntiAim.antiaim_jumping_fake_yaw == 4)
					ImGui::newSliderFloat("Jitter range##jumping fake jitter range", &Clientvariables->NewAntiAim.antiaim_jumping_fake_spin_speed, -20.f, 20.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
				else
					Clientvariables->NewAntiAim.antiaim_jumping_fake_spin_speed = 0.f;
			}
			if (Clientvariables->aachange == 3)
			{
				ImGui::Combo("Edging type", &Clientvariables->NewAntiAim.antiaim_edging_type, antiaim_edging_types, ARRAYSIZE(antiaim_edging_types));
				ImGui::newSliderFloat("LBY delta", &Clientvariables->NewAntiAim.antiaim_edging_lby_delta, -179.f, 179.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
				ImGui::newSliderFloat("Jitter range", &Clientvariables->NewAntiAim.antiaim_edging_jrange, 0, 360.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
				ImGui::Checkbox("Standing", &Clientvariables->NewAntiAim.antiaim_edging_standing_enabled);
				ImGui::Checkbox("Moving", &Clientvariables->NewAntiAim.antiaim_edging_moving_enabled);
				ImGui::Checkbox("In-air", &Clientvariables->NewAntiAim.antiaim_edging_jumping_enabled);

			}
		}
		
	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(XorStr("RagebotChild3"), ImVec2(ImGui::GetWindowWidth() / 3 - 10, 0), true, G::ShowMenu ? ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar : ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
	{

		ImGui::Checkbox("Delay shot", &Clientvariables->Ragebot.PositionAdjustment);
		if (Clientvariables->Ragebot.PositionAdjustment)
		{
			ImGui::SliderInt("##delay", &Clientvariables->Ragebot.EnginePred1, 0, 10, "%.fms%");
		}
		ImGui::Checkbox("Multipoint", &Clientvariables->Ragebot.enablescale);
		ImGui::Combo("##hitbox", &Clientvariables->Ragebot.Hitbox, HitboxMode, ARRAYSIZE(HitboxMode));
		ImGui::newSliderFloat("Head scale", &Clientvariables->Ragebot.Headscale, 0.f, 1.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
		ImGui::newSliderFloat("Body scale", &Clientvariables->Ragebot.Bodyscale, 0.f, 1.f, "%.2f%%", 1.0f, ImVec2(0, 0), ImVec4(0.09f, 0.26f, 0.55f, 0.90f));
		ImGui::Checkbox("Hitscan penetration", &Clientvariables->Ragebot.AutowallHitscan);
		ImGui::Text("Hitscan");
		ImGui::Combo("##multibox", &Clientvariables->Ragebot.Hitscan, HitscanMode, ARRAYSIZE(HitscanMode));
		if (Clientvariables->Ragebot.Hitscan == 4)
		{
			ImGui::Text("Bones to hitscan");
			ImGui::Separator();
			ImGui::Selectable("Head", &Clientvariables->Ragebot.HeadH);
			ImGui::Selectable("Neck", &Clientvariables->Ragebot.NeckH);
			ImGui::Selectable("Body", &Clientvariables->Ragebot.BodyH);
			ImGui::Selectable("Arms", &Clientvariables->Ragebot.ArmsH);
			ImGui::Selectable("Legs", &Clientvariables->Ragebot.LegsH);
			ImGui::Selectable("Foot", &Clientvariables->Ragebot.FootsH);
		}

	}ImGui::EndChild();
	

}

void visuals()
{
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_ChildWindowBg] = ImColor(18, 18, 18, 100);
	ImGui::BeginChild(XorStr("RagebotChild1"), ImVec2(ImGui::GetWindowWidth() / 3 - 10, 0), true, G::ShowMenu ? ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar : ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
	{

		static bool Main = true;
		static bool Colors = false;

		static int Page = 0;

		ImGuiStyle& style = ImGui::GetStyle();


		style.ItemSpacing = ImVec2(1, 1);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.f));

		const char* meme = "page : error";
		switch (Page)
		{
		case 0: meme = "page : 1";  break;
		case 1: meme = "page : 2";  break;
		case 2: meme = "page : 3";  break;
		case 3: meme = "page : 4";  break;
		case 4: meme = "page : 5";  break;
		default: break;
		}

		ImGui::Text(meme); ImGui::SameLine(); ImGui::Text(XorStr("                  ")); ImGui::SameLine();
		if (ImGui::Button(XorStr("-"), ImVec2(22, 22)))
		{
			if (Page != 0)
				Page--;
		};
		ImGui::SameLine();
		if (ImGui::Button(XorStr("+"), ImVec2(22, 22)))
		{
			if (Page != 3)
				Page++;
		};

		ImGui::Text(XorStr("        "));

		ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor();
		style.ItemSpacing = ImVec2(8, 4);


		switch (Page)
		{
		case 0:
		{
			TextColor(true);
			ImGui::Checkbox("Enable", &Clientvariables->Visuals.EspEnable);
			ImGui::Checkbox("Enemy only", &Clientvariables->Visuals.EnemyOnly);
			// box
			ImGui::Checkbox("Box", &Clientvariables->Visuals.BoundingBox);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##box", Clientvariables->Colors.BoundingBox, 1 << 7);

			// health bar
			ImGui::Checkbox("Health bar", &Clientvariables->Visuals.Health);
			ImGui::Combo("Health type", &Clientvariables->Visuals.HealthType, HealthType, ARRAYSIZE(HealthType));


			//snaplines
			ImGui::Checkbox("Line of sight", &Clientvariables->Visuals.SnapLines);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##snapline", Clientvariables->Colors.Snaplines, 1 << 7);

			// skeletons
			ImGui::Checkbox("Skeleton", &Clientvariables->Visuals.Bones);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##bones", Clientvariables->Colors.Skeletons, 1 << 7);
		}break;
		case 1:
		{

			// Glow
			ImGui::Checkbox("Glow", &Clientvariables->Visuals.Glow);
			if (Clientvariables->Visuals.Glow) {
				ImGui::SliderFloat("##glowforce", &Clientvariables->Visuals.Opacity, 0.f, 1.f, "%.2f%%");
				ImGui::SameLine();
				ImGui::MyColorEdit3("##glow", Clientvariables->Colors.Glow, 1 << 7);
				ImGui::Checkbox("Pulse", &Clientvariables->Visuals.pulseglow);
			}

			//  hitmarker
			ImGui::Text("Hitmarker");
			ImGui::Combo("##hitmarkerwav", &Clientvariables->Visuals.Hitmarker, Hitmarkerwavs, ARRAYSIZE(Hitmarkerwavs));
			ImGui::Checkbox("hitmarker", &Clientvariables->Visuals.aimware_hitmarker); 
			ImGui::SameLine();
			ImGui::MyColorEdit3("##hitmarkeraimwarecolor", Clientvariables->Visuals.aimwarehitmarkercolor, 1 << 7);
			ImGui::SliderInt("Hitmarkersize", &Clientvariables->Visuals.hitmarker_size, 1, 20);
			//ImGui::SliderInt("##FOVV", &Clientvariables->Misc.PlayerViewmodel, 1, 50);
		}break;
		case 2:
		{
			ImGui::Checkbox("Player", &Clientvariables->Visuals.ChamsPlayer);
			ImGui::Checkbox("Metallic", &Clientvariables->Visuals.metallicchams);
			ImGui::Checkbox("Local Chams", &Clientvariables->Visuals.LocalChams1);
			
			ImGui::Combo("Chams type xd", &Clientvariables->Visuals.ChamsStyle, stylexd, ARRAYSIZE(stylexd));
			
			ImGui::Checkbox("Enable", &Clientvariables->Visuals.ChamsEnable);
			ImGui::Checkbox("Enable", &Clientvariables->Visuals.ChamsEnable);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##asdddddddddddddddd##chams", Clientvariables->Colors.PlayerChams, 1 << 7);
			ImGui::Checkbox("Player (behind wall)", &Clientvariables->Visuals.ChamsPlayerWall);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##aggggggggggggggggggggggggg##chamswall", Clientvariables->Colors.PlayerChamsWall, 1 << 7);
			ImGui::MyColorEdit3("When scoped", Clientvariables->Visuals.lvmodel_col, 1 << 7);
		


			// grenade trajectory
			ImGui::Checkbox("Grenade trajectory", &Clientvariables->Visuals.GrenadePrediction);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##nadetrajectory", Clientvariables->Colors.GrenadePrediction, 1 << 7);

			// bullet beams
			ImGui::Checkbox("Bullet tracers", &Clientvariables->Visuals.BulletTracers);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##tracers", Clientvariables->Colors.Bulletracer, 1 << 7);

			// bullet impacts
			ImGui::Checkbox("Bullet impacts", &Clientvariables->Visuals.Bullet_impacts);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##impacts", Clientvariables->Colors.bullet_impacts, 1 << 7);
		}break;
		case 3:
		{
			// local esp
			ImGui::Checkbox("Self ESP", &Clientvariables->Visuals.LocalPlayer);

			// self chams
			ImGui::Checkbox("Self Chams", &Clientvariables->Visuals.LocalChams);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##localchams", Clientvariables->Colors.LocalChams, 1 << 7);

			// self glow
			

			
			ImGui::Checkbox("Fake chams", &Clientvariables->Visuals.FakeAngleGhost);
			ImGui::MyColorEdit3("##agtjygvdczf", Clientvariables->Colors.FakeAngleGhost, 1 << 7);




		}break;

		default:break;
		}


	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(XorStr("RagebotChild2"), ImVec2(ImGui::GetWindowWidth() / 3 - 10, 0), true, G::ShowMenu ? ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar : ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
	{

		ImGui::Checkbox("Remove smoke grenades", &Clientvariables->Visuals.Nosmoke);
		ImGui::Checkbox("Remove fog", &Clientvariables->Visuals.RemoveParticles);
		ImGui::Checkbox("Remove visual recoil", &Clientvariables->Visuals.Novisrevoil);
		ImGui::Checkbox("Remove scope", &Clientvariables->Visuals.Noscope);
		ImGui::Checkbox("Nightmode", &Clientvariables->Visuals.nightmode);

		ImGui::Text("Skybox");
		ImGui::Combo("##Skybox", &Clientvariables->Visuals.Skybox, Skyboxmode, ARRAYSIZE(Skyboxmode));

		ImGui::Text("Thirdperson");
		ImGui::Hotkey("##thirdperson", &Clientvariables->Misc.TPKey, ImVec2(164, 20));
		ImGui::Combo("##angles", &Clientvariables->Misc.TPangles, ThirdpersonAngles, ARRAYSIZE(ThirdpersonAngles));



		ImGui::Combo("Chams style xd", &Clientvariables->Visuals.stylexd, stylexd, ARRAYSIZE(stylexd));

		ImGui::Checkbox("Weapon Chams", &Clientvariables->Visuals.WeaponChams);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##poiuytre", Clientvariables->Colors.ChamsWeapon, 1 << 7);

		ImGui::Checkbox("Wireframe weapon Chams", &Clientvariables->Visuals.WeaponWireframe);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##kjhgfd", Clientvariables->Colors.WireframeWeapon, 1 << 7);


		ImGui::Checkbox("Hand Chams", &Clientvariables->Visuals.ChamsHands);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##zxcvbnm", Clientvariables->Colors.ChamsHand, 1 << 7);

		ImGui::Checkbox("Wireframe hand Chams", &Clientvariables->Visuals.ChamsHandsWireframe);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##mnbvcxcv", Clientvariables->Colors.WireframeHand, 1 << 7);



		

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(XorStr("RagebotChild3"), ImVec2(ImGui::GetWindowWidth() / 3 - 10, 0), true, G::ShowMenu ? ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar : ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
	{
		ImGui::Checkbox("Kill Feed", &Clientvariables->Visuals.PreserveKillfeed);
		ImGui::Checkbox("Event Log", &Clientvariables->Visuals.eventlog);
		ImGui::Checkbox("Spectators", &Clientvariables->Visuals.SpectatorList);
		ImGui::Checkbox("Color sky", &Clientvariables->Visuals.skycolor);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##aergfvwargvrtghberdg", Clientvariables->Colors.skycolorxd, 1 << 7);
		ImGui::Checkbox("Color world", &Clientvariables->Visuals.worldcolor);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##qebafvaefvgawvawerfv", Clientvariables->Colors.worldcolorxd, 1 << 7);
	}ImGui::EndChild();

	/*
	auto& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_ChildWindowBg] = ImColor(18, 18, 18, 180);
	ImGui::Columns(2, "yes", false);

	ImGui::Text("Effects");
	ImGui::BeginChild("##effects", ImVec2(272, 459), true);  // 272, 289 + 170
	{
		TextColor(true);
		ImGui::Checkbox("Remove smoke grenades", &Clientvariables->Visuals.Nosmoke);
		ImGui::Checkbox("Remove fog", &Clientvariables->Visuals.RemoveParticles);
		ImGui::Checkbox("Remove visual recoil", &Clientvariables->Visuals.Novisrevoil);
		ImGui::Checkbox("Remove scope", &Clientvariables->Visuals.Noscope);
		ImGui::Checkbox("Nightmode", &Clientvariables->Visuals.nightmode);

		ImGui::Text("Skybox");
		ImGui::Combo("##Skybox", &Clientvariables->Visuals.Skybox, Skyboxmode, ARRAYSIZE(Skyboxmode));

		ImGui::Text("Thirdperson");
		ImGui::Hotkey("##thirdperson", &Clientvariables->Misc.TPKey, ImVec2(164, 20));
		ImGui::Combo("##angles", &Clientvariables->Misc.TPangles, ThirdpersonAngles, ARRAYSIZE(ThirdpersonAngles));
	}
	ImGui::EndChild();

	ImGui::NextColumn();

	ImGui::Text("Player ESP");
	ImGui::BeginChild("##player", ImVec2(272, 459), true);
	{

		


	


		//  player shit
	
	



	}

	ImGui::EndChild();
	*/
}

void misc() {

	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_ChildWindowBg] = ImColor(18, 18, 18, 100);

	ImGui::BeginChild(XorStr("RagebotChild1"), ImVec2(ImGui::GetWindowWidth() / 3 - 10, 0), true, G::ShowMenu ? ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar : ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
	{
		ImGui::Checkbox("LagFix test", &Clientvariables->Misc.lagfix);
		ImGui::Checkbox("Anti-untrusted", &Clientvariables->Misc.AntiUT);
		ImGui::Checkbox("Antiuntrusted", &Clientvariables->Misc.AntiUTxd);
		ImGui::Checkbox("Enable buybot", &Clientvariables->Misc.buybot);
		if (Clientvariables->Misc.buybot) {
			ImGui::Text("Buy bot");
			ImGui::Combo("##buybot", &Clientvariables->Misc.buybot2, BuyBot, ARRAYSIZE(BuyBot));
			ImGui::Checkbox("Helmet and grenades", &Clientvariables->Misc.buybot3);
		}
		ImGui::Checkbox("Bunnyhop", &Clientvariables->Misc.AutoJump);
		ImGui::Checkbox("Air strafe", &Clientvariables->Misc.AutoStrafe);
		if (Clientvariables->Misc.AutoStrafe) {
			ImGui::Text("Circle-Strafe");
			ImGui::Hotkey("##speedkey", &Clientvariables->Misc.PrespeedKey, ImVec2(150, 20));
			ImGui::SliderInt("##retrack", &Clientvariables->Misc.Retrack, 1, 8);
		}

		ImGui::Checkbox("Fakewalk", &Clientvariables->Misc.FakewalkEnable);
		if (Clientvariables->Misc.FakewalkEnable) {
			ImGui::Hotkey("##walkkey", &Clientvariables->Misc.FakewalkKey, ImVec2(150, 20));
			ImGui::SliderInt("fakewalkspeed", &Clientvariables->Misc.fakewalk_speed, 0, 14);
		}

		ImGui::Checkbox("Fakelag Enable", &Clientvariables->Misc.FakelagEnable);
		ImGui::Checkbox("Fakelag on ground", &Clientvariables->Misc.FakelagOnground);
		ImGui::Combo("Fakelag Mode", &Clientvariables->Misc.FakelagMode, FakelagMode, ARRAYSIZE(FakelagMode));
		ImGui::SliderInt("Choke", &Clientvariables->Misc.FakelagAmount, 1, 14);
		
	}ImGui::EndChild();
}
static char ConfigName[64] = { 0 };
void safetab()
{
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_ChildWindowBg] = ImColor(18, 18, 18, 100);



	ImGui::BeginChild(XorStr("safexd3"), ImVec2(0, 0), true, G::ShowMenu ? ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar : ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
	{
		{
			ImGui::InputText("##CFG", ConfigName, 64);
			static int sel;
			std::string config;
			std::vector<std::string> configs = Clientvariables->GetConfigs();
			if (configs.size() > 0) {
				ImGui::ComboBoxArray("Configs", &sel, configs);
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::PushItemWidth(220.f);
				config = configs[Clientvariables->config_sel];
			}
			Clientvariables->config_sel = sel;

			if (configs.size() > 0) {
				if (ImGui::Button("Load", ImVec2(100, 20)))
				{

					Clientvariables->Load(config);
					FEATURES::MISC::in_game_logger.AddLog(FEATURES::MISC::InGameLogger::Log(("[Azuhook] Configuration loaded."), Color(255, 0, 0, 255)));
				}
			}
			ImGui::SameLine();

			if (configs.size() >= 1) {
				if (ImGui::Button("Save", ImVec2(100, 20)))
				{
					Clientvariables->Save(config);
					FEATURES::MISC::in_game_logger.AddLog(FEATURES::MISC::InGameLogger::Log(("[Azuhook] Configuration saved."), Color(255, 0, 0, 255)));
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Create", ImVec2(100, 20)))
			{
				std::string ConfigFileName = ConfigName;
				if (ConfigFileName.size() < 1)
				{
					ConfigFileName = "settings";
				}
				Clientvariables->CreateConfig(ConfigFileName);
				FEATURES::MISC::in_game_logger.AddLog(FEATURES::MISC::InGameLogger::Log(("[Azuhook] Configuration created."), Color(255, 0, 0, 255)));
			}
		}
	}ImGui::EndChild();



	
}

void skinchanger()
{
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_ChildWindowBg] = ImColor(18, 18, 18, 100);
	ImGui::BeginChild(XorStr("Skinsxd1"), ImVec2(ImGui::GetWindowWidth() / 3 - 10, 0), true, G::ShowMenu ? ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar : ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
	{
		ImGui::Checkbox("skinchanger", &Clientvariables->NewSkinchanger.enabled);
		ImGui::Checkbox("glovechanger", &Clientvariables->NewSkinchanger.glovesenabled);
		ImGui::Checkbox("rankchanger", &Clientvariables->RankChanger.enabled);
		ImGui::Checkbox("inventarchanger", &Clientvariables->RankChanger.semiinventar);
		if (ImGui::Button(("Force update")))
		{
			KnifeApplyCallbk();

		}

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(XorStr("Skinsxd2"), ImVec2(ImGui::GetWindowWidth() / 3 - 10, 0), true, G::ShowMenu ? ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar : ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
	{
	
		static bool Main = true;
		static bool Colors = false;

		static int Page = 0;

		ImGuiStyle& style = ImGui::GetStyle();


		style.ItemSpacing = ImVec2(1, 1);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.f));

		const char* meme = "page : error";
		switch (Page)
		{
		case 0: meme = "page : 1";  break;
		case 1: meme = "page : 2";  break;
		case 2: meme = "page : 3";  break;
		case 3: meme = "page : 4";  break;
		case 4: meme = "page : 5";  break;
		default: break;
		}

		ImGui::Text(meme); ImGui::SameLine(); ImGui::Text(XorStr("                  ")); ImGui::SameLine();
		if (ImGui::Button(XorStr("-"), ImVec2(22, 22)))
		{
			if (Page != 0)
				Page--;
		};
		ImGui::SameLine();
		if (ImGui::Button(XorStr("+"), ImVec2(22, 22)))
		{
			if (Page != 3)
				Page++;
		};

		ImGui::Text(XorStr("        "));

		ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor();
		style.ItemSpacing = ImVec2(8, 4);
		switch (Page)
		{
		case 0:
		{
			
			ImGui::PushItemWidth(150.0f);
			ImGui::Combo(("Knife Model"), &Clientvariables->NewSkinchanger.Knife, KnifeModel, ARRAYSIZE(KnifeModel));
			ImGui::PushItemWidth(150.0f);
			ImGui::Combo(("Knife Skin"), &Clientvariables->NewSkinchanger.KnifeSkin, knifeskins, ARRAYSIZE(knifeskins));
			ImGui::PushItemWidth(150.0f);
			ImGui::Combo(("glove model"), &Clientvariables->NewSkinchanger.gloves, GloveModel, ARRAYSIZE(GloveModel));
			ImGui::PushItemWidth(150.0f);
			ImGui::Combo(("glove skin"), &Clientvariables->NewSkinchanger.skingloves, xdshit, ARRAYSIZE(xdshit));
		}break;
		case 1:
		{
			ImGui::Combo(("AK-47"), &Clientvariables->NewSkinchanger.AK47Skin, ak47, ARRAYSIZE(ak47));
			ImGui::Combo(("M4A1-S"), &Clientvariables->NewSkinchanger.M4A1SSkin, m4a1s, ARRAYSIZE(m4a1s));
			ImGui::Combo(("M4A4"), &Clientvariables->NewSkinchanger.M4A4Skin, m4a4, ARRAYSIZE(m4a4));
			ImGui::Combo(("Galil AR"), &Clientvariables->NewSkinchanger.GalilSkin, galil, ARRAYSIZE(galil));
			ImGui::Combo(("AUG"), &Clientvariables->NewSkinchanger.AUGSkin, aug, ARRAYSIZE(aug));
			ImGui::Combo(("FAMAS"), &Clientvariables->NewSkinchanger.FAMASSkin, famas, ARRAYSIZE(famas));
			ImGui::Combo(("Sg553"), &Clientvariables->NewSkinchanger.Sg553Skin, sg553, ARRAYSIZE(sg553));
			ImGui::Combo(("UMP45"), &Clientvariables->NewSkinchanger.UMP45Skin, ump45, ARRAYSIZE(ump45));
			ImGui::Combo(("MAC-10"), &Clientvariables->NewSkinchanger.Mac10Skin, mac10, ARRAYSIZE(mac10));
			ImGui::Combo(("PP-Bizon"), &Clientvariables->NewSkinchanger.BizonSkin, bizon, ARRAYSIZE(bizon));
			ImGui::Combo(("TEC-9"), &Clientvariables->NewSkinchanger.tec9Skin, tec9, ARRAYSIZE(tec9));
			ImGui::Combo(("P2000"), &Clientvariables->NewSkinchanger.P2000Skin, p2000, ARRAYSIZE(p2000));
			ImGui::Combo(("P250"), &Clientvariables->NewSkinchanger.P250Skin, p250, ARRAYSIZE(p250));
			ImGui::Combo(("Dual-Barettas"), &Clientvariables->NewSkinchanger.DualSkin, dual, ARRAYSIZE(dual));
			ImGui::Combo(("Cz75-Auto"), &Clientvariables->NewSkinchanger.Cz75Skin, cz75, ARRAYSIZE(cz75));
			ImGui::Combo(("Nova"), &Clientvariables->NewSkinchanger.NovaSkin, nova, ARRAYSIZE(nova));
			
			
		}break;
		case 2:
		{
			ImGui::Combo(("XM1014"), &Clientvariables->NewSkinchanger.XmSkin, xm, ARRAYSIZE(xm));
			ImGui::Combo(("AWP"), &Clientvariables->NewSkinchanger.AWPSkin, awp, ARRAYSIZE(awp));
			ImGui::Combo(("SSG08"), &Clientvariables->NewSkinchanger.SSG08Skin, ssg08, ARRAYSIZE(ssg08));
			ImGui::Combo(("SCAR20"), &Clientvariables->NewSkinchanger.SCAR20Skin, scar20, ARRAYSIZE(scar20));
			ImGui::Combo(("G3SG1"), &Clientvariables->NewSkinchanger.G3sg1Skin, g3sg1, ARRAYSIZE(g3sg1));
			ImGui::Combo(("MP9"), &Clientvariables->NewSkinchanger.Mp9Skin, mp9, ARRAYSIZE(mp9));
			ImGui::Combo(("Glock-18"), &Clientvariables->NewSkinchanger.GlockSkin, glock, ARRAYSIZE(glock));
			ImGui::Combo(("USP-S"), &Clientvariables->NewSkinchanger.USPSkin, usp, ARRAYSIZE(usp));
			ImGui::Combo(("Deagle"), &Clientvariables->NewSkinchanger.DeagleSkin, deagle, ARRAYSIZE(deagle));
			ImGui::Combo(("Five-Seven"), &Clientvariables->NewSkinchanger.FiveSkin, five, ARRAYSIZE(five));
			ImGui::Combo(("Revolver"), &Clientvariables->NewSkinchanger.RevolverSkin, revolver, ARRAYSIZE(revolver));
			ImGui::Combo(("Negev"), &Clientvariables->NewSkinchanger.NegevSkin, negev, ARRAYSIZE(negev));
			ImGui::Combo(("M249"), &Clientvariables->NewSkinchanger.M249Skin, m249, ARRAYSIZE(m249));
			ImGui::Combo(("Sawed-Off"), &Clientvariables->NewSkinchanger.SawedSkin, sawed, ARRAYSIZE(sawed));
			ImGui::Combo(("Mag-7"), &Clientvariables->NewSkinchanger.MagSkin, mag, ARRAYSIZE(mag));
		}break;
		case 3:
		{
			short cw = SafeWeaponID();
			ImGui::InputText(XorStr("yourshit"), Clientvariables->NewSkinchanger.weapons[cw].ChangerName, 128);
			if (cw == 59 || cw == 500 || cw == 42 || cw == 507 || cw == 506 || cw == 508 || cw == 509 || cw == 515 || cw == 516 || cw == 505 || cw == 512 || cw == 523 || cw == 519 || cw == 521 || cw == 520 || cw == 522)
			{

			}
			else
			{
				ImGui::InputInt(("Stikers1"), &Clientvariables->NewSkinchanger.weapons[cw].Stikers1);
				ImGui::InputInt(("Stikers2"), &Clientvariables->NewSkinchanger.weapons[cw].Stikers2);
				ImGui::InputInt(("Stikers3"), &Clientvariables->NewSkinchanger.weapons[cw].Stikers3);
				ImGui::InputInt(("Stikers4"), &Clientvariables->NewSkinchanger.weapons[cw].Stikers4);
			}
			

		}break;

		default:break;
		}

	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild(XorStr("Skinsxd3"), ImVec2(ImGui::GetWindowWidth() / 3 - 10, 0), true, G::ShowMenu ? ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar : ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
	{
		ImGui::Combo(("rank"), &Clientvariables->RankChanger.rank_id, xdranks, ARRAYSIZE(xdranks));
		ImGui::SliderInt("level", &Clientvariables->RankChanger.player_level, 0, 40);
		ImGui::InputInt("wins", &Clientvariables->RankChanger.wins);
		ImGui::InputInt("friendly", &Clientvariables->RankChanger.cmd_friendly);
		ImGui::InputInt("teaching", &Clientvariables->RankChanger.cmd_teaching);
		ImGui::InputInt("leader", &Clientvariables->RankChanger.cmd_leader);
		if (ImGui::Button(("apply")))
		{
			ProtoFeatures.SendClientHello();
			ProtoFeatures.SendMatchmakingClient2GCHello();
		}
	}ImGui::EndChild();

}


std::string SanitizeName(char *name)
{
	name[127] = '\0';

	std::string tmp(name);

	for (int i = 0; i < (int)tmp.length(); i++)
	{
		if ((
			tmp[i] >= 'a' && tmp[i] <= 'z' ||
			tmp[i] >= 'A' && tmp[i] <= 'Z' ||
			tmp[i] >= '0' && tmp[i] <= '9' ||
			tmp[i] == ' ' || tmp[i] == '.' || tmp[i] == '/' || tmp[i] == ':' ||
			tmp[i] == ',' || tmp[i] == '_' || tmp[i] == '#' || tmp[i] == '$' ||
			tmp[i] == '<' || tmp[i] == '>' || tmp[i] == '-' || tmp[i] == '+' ||
			tmp[i] == '*' || tmp[i] == '%' || tmp[i] == '@' || tmp[i] == '(' ||
			tmp[i] == ')' || tmp[i] == '{' || tmp[i] == '}' || tmp[i] == '[' || tmp[i] == ']' ||
			tmp[i] == '!' || tmp[i] == '&' || tmp[i] == '~' || tmp[i] == '^'
			) == false)
		{
			tmp[i] = '_';
		}
	}

	if (tmp.length() > (20))
	{
		tmp.erase(20, (tmp.length() - 20));
		tmp.append("...");
	}
	return tmp;
}

HRESULT __stdcall Hooks::D3D9_EndScene(IDirect3DDevice9* pDevice)
{
	HRESULT result = d3d9VMT->GetOriginalMethod<EndSceneFn>(42)(pDevice);

	if (!G::Init)
	{
		GUI_Init(pDevice);
	}
	else
	{
		ImGui_ImplDX9_NewFrame();
		ImGuiStyle& style = ImGui::GetStyle();

		


		if (Clientvariables->Visuals.SpectatorList)
		{
			style.ChildWindowRounding = 4.0f;
			style.WindowRounding = 4.0f;

			if (ImGui::Begin(XorStr("Spectators"), &Clientvariables->Visuals.SpectatorList, ImVec2(300, 100), 0.5f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize))
			{
				//ImGui::Text(XorStr("Spectator list"));
				int max = 65;

				if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
				{
					for (int i = 1; i < max; i++)
					{
						CBaseEntity *entity = g_pEntitylist->GetClientEntity(i);

						if (NULL == entity) continue;
						if (entity->IsDormant()) continue;
						ClientClass* pClass = (ClientClass*)entity->GetClientClass();

						if (pClass->m_ClassID != 35) continue;

						CBaseHandle sh = entity->GetObserverTarget();

						if (sh == 0xFFFFFFFF) continue;

						CBaseEntity *spectating = g_pEntitylist->GetClientEntityFromHandle(sh);

						if (spectating != NULL && spectating == G::LocalPlayer)
						{
							player_info_t info;
							if (g_pEngine->GetPlayerInfo(entity->GetIndex(), &info))
							{
								std::string tmp = SanitizeName(info.m_szPlayerName);

								ImGui::Text(u8"%s", tmp.c_str());
							}
						}
					}
				}



				ImGui::End();
			}
			style.ChildWindowRounding = 0.0f;
			style.WindowRounding = 0.0f;
		}


		if (G::Opened)
			G::ShowMenu = true;
		else 
			G::ShowMenu = false;
		
	

		/*
		if (G::ShowMenu)
		{
			if (TittleBar == nullptr)D3DXCreateTextureFromFileInMemoryEx(pDevice
				, &NameArry, sizeof(NameArry),
				1000, 1000, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &TittleBar);

		}

		*/
		static bool notFull = true;
		static bool notFull1 = true;
		static bool notFull2 = true;
		static int XMenu = 1;
		static int YMenu = 80;
		if (!G::ShowMenu)
		{
			notFull = true;
			notFull1 = true;
			notFull2 = true;
			XMenu = 1;
			YMenu = 95;
			if (!Clientvariables->Visuals.SpectatorList)
				g_pCvar->FindVar("cl_showfps")->SetValue(false);
			else
				g_pCvar->FindVar("cl_showfps")->SetValue(true);
		}
		if (G::ShowMenu)
		{
			int pX, pY;
			g_pCvar->FindVar("cl_showfps")->SetValue(true);
			g_pInputSystem->GetCursorPosition(&pX, &pY);
			ImGuiIO& io = ImGui::GetIO();
			io.MousePos.x = (float)(pX);
			io.MousePos.y = (float)(pY);
			//VMProtectBeginMutation("MENU");
			//	char nameChar[128];
			//	sprintf(nameChar, "Project Z");

			int windowWidth3 = 750;
			int windowHeight3 = 530;
			if (windowWidth3 <= XMenu) {
				notFull1 = false;
			}
			if (windowHeight3 <= YMenu) {
				notFull2 = false;
			}
			ImGui::SetNextWindowSize(ImVec2(XMenu, YMenu));
			if (notFull1)
			{
				if (true)
					XMenu += 32;
				else
					XMenu = windowWidth3;
			}
			if (notFull2 && !notFull1)
			{
				if (true)
					YMenu += 20;
				else
					YMenu = windowHeight3;
			}
			color();
			TextColor(true);
			Border(true);
			style.Colors[ImGuiCol_WindowBg] = ImColor(18, 18, 18, 180);
			if (ImGui::Begin(("Shonax"), &G::ShowMenu, ImVec2(XMenu, YMenu), 0.70f, (notFull1 || notFull2 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar))
			{
				ImGui::PushFont(Default);
				static bool skinTab = false;
				static bool visualsTab = false;
				static bool changer = false;
				static bool rageTab = false;
				static bool configTab = false;
				static bool miscTab = false;
				static bool testTab = false;
				static bool safeTab = false;


				//if (notFull) return;	ImVec2 lPos = ImGui::GetCursorPos();
				ImGuiContext* io = ImGui::GetCurrentContext();
				


				ImDrawList* windowDrawList = ImGui::GetWindowDrawList();
				




				if (!LegitTabImg)
					D3DXCreateTextureFromFileInMemoryEx(pDevice, &NameLegitArray, sizeof(NameLegitArray), 1000, 1000, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &LegitTabImg);

				if (RageTabImg == nullptr)D3DXCreateTextureFromFileInMemoryEx(pDevice
					, &NameRageArray, sizeof(NameRageArray),
					1000, 1000, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &RageTabImg);

				if (VisualsTabImg == nullptr)D3DXCreateTextureFromFileInMemoryEx(pDevice
					, &NameVisualsArray, sizeof(NameVisualsArray),
					1000, 1000, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &VisualsTabImg);


				if (MiscTabImg == nullptr)D3DXCreateTextureFromFileInMemoryEx(pDevice
					, &NameMiscArray, sizeof(NameMiscArray),
					1000, 1000, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &MiscTabImg);


				if (SaveTabImg == nullptr)D3DXCreateTextureFromFileInMemoryEx(pDevice
					, &NameSavesArray, sizeof(NameSavesArray),
					1000, 1000, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &SaveTabImg);

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.05f, 1.0f, 0.05f, 0.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.05f, 1.0f, 0.05f, 0.f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.05f, 1.0f, 0.05f, 0.f));
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.05f, 1.0f, 0.05f, 0.f));
				ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.05f, 1.0f, 0.05f, 0.f));


				//ImVec2 curWindowPos = ImGui::GetWindowPos();
			//	windowDrawList->AddImage(TittleBar, ImVec2(curWindowPos.x + 750 / 2.5f + 20, curWindowPos.y), ImVec2(curWindowPos.x + 750 / 2.5f + 125, curWindowPos.y + 55));

				//ImGui::Text(XorStr("        "));
				//ImGui::Text(XorStr("        "));
				//ImGui::Text(XorStr("        "));
				//ImGui::Text(XorStr("        "));

				//ImGui::Text(XorStr("         "));
			//	ImGui::SameLine();

				
				if (ImGui::ImageButton(RageTabImg, ImVec2(105, 105))) {
					rageTab = true;
					skinTab = false;
					visualsTab = false;
					changer = false;
					configTab = false;
					miscTab = false;
					safeTab = false;
				}
				ImGui::SameLine();
				ImGui::Text(XorStr("       "));
				ImGui::SameLine();
				if (ImGui::ImageButton(VisualsTabImg, ImVec2(105, 105))) {
					rageTab = false;
					skinTab = false;
					visualsTab = true;
					changer = false;
					configTab = false;
					miscTab = false;
					safeTab = false;
				}
				ImGui::SameLine();
				ImGui::Text(XorStr("       "));
				ImGui::SameLine();
				if (ImGui::ImageButton(MiscTabImg, ImVec2(105, 105))) {
					rageTab = false;
					skinTab = false;
					visualsTab = false;
					miscTab = true;
					safeTab = false;
				}
				ImGui::SameLine();
				ImGui::Text(XorStr("       "));
				ImGui::SameLine();
				if (ImGui::ImageButton(LegitTabImg, ImVec2(105, 105))) {
					rageTab = false;
					skinTab = true;
					visualsTab = false;
					changer = false;
					configTab = false;
					miscTab = false;
					safeTab = false;
				}

				ImGui::SameLine();
				ImGui::Text(XorStr("       "));
				ImGui::SameLine();
				if (ImGui::ImageButton(SaveTabImg, ImVec2(105, 105))) {
					rageTab = false;
					skinTab = false;
					visualsTab = false;
					changer = false;
					configTab = false;
					miscTab = false;
					safeTab = true;
				}

				


				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();


				
				if (rageTab)
					ragebot();
				if (visualsTab)
					visuals();
				if (miscTab)
					misc();
				if (skinTab)
					skinchanger();
				if (safeTab)
					safetab();
					

			}
			ImGui::End();
			ImGui::PopFont();


		}
		/*
		auto& style = ImGui::GetStyle();

		if (G::Opened && style.Alpha < 1.f)
		{
			G::ShowMenu = true;
			if (style.Alpha > 1.f)
				style.Alpha = 1.f;
			else if (style.Alpha != 1.f)
				style.Alpha += 0.03f;
		}
		else if (style.Alpha > 0.f)
		{
			if (style.Alpha < 0.f)
				style.Alpha = 0.f;
			else if (style.Alpha != 0.f)
				style.Alpha -= 0.03f;
			if (style.Alpha == 0.f)
				G::ShowMenu = false;
		}

		static int tab;

		if (G::ShowMenu)
		{
			ImVec2 mainWindowPos; // wir brauchen die main window pos um nen vernünftigen border zu drawen

			ImGui::PushFont(Default);
			color();
			ImGui::SetNextWindowSize(ImVec2(649, 539));
			ImGui::Begin("Gamephetamine", &G::ShowMenu, ImVec2(712, 699), 0.98f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar);
			{
				mainWindowPos = ImGui::GetWindowPos();

				style.ItemSpacing = ImVec2(0, 0);

				if (tab == 0) BtnActive(); else BtnNormal();
				style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
				if (ImGui::Button("ragebot", ImVec2(161, 41))) tab = 0;
				ImGui::SameLine();

				if (tab == 1) BtnActive(); else BtnNormal();
				style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
				if (ImGui::Button("legitbot", ImVec2(161, 41))) tab = 1;
				ImGui::SameLine();

				if (tab == 2) BtnActive(); else BtnNormal();
				style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
				if (ImGui::Button("visuals", ImVec2(161, 41))) tab = 2;
				ImGui::SameLine();

				if (tab == 3) BtnActive(); else BtnNormal();
				style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
				if (ImGui::Button("misc", ImVec2(161, 41))) tab = 3;

				ImGui::PushFont(Default);
				


				ImGui::NextColumn();
				TextColor(true);
				Border(true);
				color();



				style.ItemSpacing = ImVec2(8, 6);
				style.Colors[ImGuiCol_Border] = ImColor(80, 80, 80, 0);
				style.Colors[ImGuiCol_ChildWindowBg] = ImColor(18, 18, 18, 0);
				style.WindowPadding = ImVec2(8, 8);
				ImGui::BeginChild("masterchild", ImVec2(705, 530), true); // Master Child
				{

					style.Colors[ImGuiCol_Border] = ImColor(80, 80, 80, 255);
					if (tab == 0) // Ragebot
					{
						ragebot();
					}
					else if (tab == 1)
					{
						ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "i would assume thats gay");
					}
					else if (tab == 2)
					{
						visuals();
					}
					else if (tab == 3)
					{
						misc();
					}
				} ImGui::EndChild();
			} ImGui::End();

			style.WindowPadding = ImVec2(2, 4);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);

			ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x - 5, mainWindowPos.y - 5));
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.f);
			ImGui::SetNextWindowSize(ImVec2(659, 549));
			ImGui::Begin("##border2", &G::ShowMenu, ImVec2(659, 549), 0.98f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_ShowBorders);
			{
				mainWindowPos = ImGui::GetWindowPos();
			} ImGui::End();

		}
		*/


	if (Clientvariables->Visuals.SpreadCrosshair)
		g_ESP->SpreadCrosshair(pDevice);

		ImGui::Render();
	}





	return result;
}

HRESULT __stdcall Hooks::hkdReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresParam)
{
	if (!G::Init)
		return oResetScene(pDevice, pPresParam);

	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto newr = oResetScene(pDevice, pPresParam);
	ImGui_ImplDX9_CreateDeviceObjects();
	

	return newr;
}

