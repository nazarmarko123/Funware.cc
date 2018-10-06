#include "sdk.h"
#include "ESP.h"
#include "global.h"
#include "xor.h"
#include "Math.h"
#include "Menu.h"
#include "Misc.h"
#include "BacktrackingHelper.h"
#include "GameUtils.h"
#include "Draw.h"
#include "Autowall.h"
#include "aimware.h"


CESP* g_ESP = new CESP;

float flPlayerAlpha[65];

#define TICK_INTERVAL			( g_pGlobals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )



float dot_product_t(const float* a, const float* b) {
	return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}

void vector_transform_a(const float *in1, const matrix3x4& in2, float *out) {
	out[0] = dot_product_t(in1, in2[0]) + in2[0][3];
	out[1] = dot_product_t(in1, in2[1]) + in2[1][3];
	out[2] = dot_product_t(in1, in2[2]) + in2[2][3];
}


inline void vector_transform_z(const Vector& in1, const matrix3x4 &in2, Vector &out) {
	vector_transform_a(&in1.x, in2, &out.x);
}


bool CESP::get_box(CBaseEntity* m_entity, Box& box, bool dynamic)
{
	DWORD m_rgflCoordinateFrame = (DWORD)0x470 - 0x30;
	const matrix3x4& trnsf = *(matrix3x4*)((DWORD)m_entity + (DWORD)m_rgflCoordinateFrame);

	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;

	float left, top, right, bottom;

	vOrigin = m_entity->GetOrigin();
	min = m_entity->GetCollision()->VecMins();
	max = m_entity->GetCollision()->VecMaxs();

	if (!dynamic) {
		min += vOrigin;
		max += vOrigin;
	}

	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	Vector vector_transformed[8];

	if (dynamic)
	{
		for (int i = 0; i < 8; i++)
		{
			vector_transform_z(points[i], trnsf, vector_transformed[i]);
			points[i] = vector_transformed[i];
		}
	}
	if (!GameUtils::WorldToScreen(points[3], flb) || !GameUtils::WorldToScreen(points[5], brt)
		|| !GameUtils::WorldToScreen(points[0], blb) || !GameUtils::WorldToScreen(points[4], frt)
		|| !GameUtils::WorldToScreen(points[2], frb) || !GameUtils::WorldToScreen(points[1], brb)
		|| !GameUtils::WorldToScreen(points[6], blt) || !GameUtils::WorldToScreen(points[7], flt))
		return false;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for (int i = 1; i < 8; i++) {
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	box.x = left;
	box.y = top;
	box.w = right - left;
	box.h = bottom - top;

	return true;
}










char* GetWeaponName(CBaseCombatWeapon* pWeapon)
{
	int ID = pWeapon->WeaponID();

	switch (ID) {
	case 4:
		return "glock";
	case 2:
		return "duals";
	case 36:
		return "p250";
	case 30:
		return "tec9";
	case 1:
		return "deagle";
	case 32:
		return "p2000";
	case 3:
		return "fiveseven";
	case 64:
		return "revolver";
	case 63:
		return "cz75";
	case 61:
		return "usp";
	case 35:
		return "nova";
	case 25:
		return "xm1014";
	case 29:
		return "sawed off";
	case 14:
		return "m249";
	case 28:
		return "negev";
	case 27:
		return "mag7";
	case 17:
		return "mac10";
	case 33:
		return "mp7";
	case 24:
		return "ump45";
	case 19:
		return "p90";
	case 26:
		return "�bizon";
	case 34:
		return "mp9";
	case 10:
		return "famas";
	case 16:
		return "m4a4";
	case 40:
		return "ssg08";
	case 8:
		return "aug";
	case 9:
		return "awp";
	case 38:
		return "scar20";
	case 13:
		return "galil";
	case 7:
		return "ak47";
	case 39:
		return "sg553";
	case 11:
		return "g3sg1";
	case 60:
		return "m4a1s";
	case 46:
	case 48:
		return "molotov";
	case 44:
		return "grenade";
	case 43:
		return "flashbang";
	case 45:
		return "smoke";
	case 47:
		return "decoy";
	case 31:
		return "taser";
	default:
		return "knife";
	}
}

template <class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

bool GetCBaseEntityBox(CBaseEntity* pBaseEntity, Vector& BotCenter, Vector& TopCenter, float& Left, float& Right, bool is_player)
{
	if (!pBaseEntity)
		return false;

	if (!is_player) {
		const VMatrix& trans = pBaseEntity->GetCollisionBoundTrans();
		CollisionProperty* collision = pBaseEntity->GetCollision();

		if (!collision)
			return false;

		Vector min, max;
		min = collision->VecMins();
		max = collision->VecMaxs();

		Vector points[] = { Vector(min.x, min.y, min.z),
			Vector(min.x, max.y, min.z),
			Vector(max.x, max.y, min.z),
			Vector(max.x, min.y, min.z),
			Vector(max.x, max.y, max.z),
			Vector(min.x, max.y, max.z),
			Vector(min.x, min.y, max.z),
			Vector(max.x, min.y, max.z) };

		auto vector_transform = [](const Vector in1, const VMatrix& in2) {
			auto dot_product = [](const Vector& v1, const float* v2) {
				float ret = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
				return ret;
			};

			auto out = Vector();
			out[0] = dot_product(in1, in2[0]) + in2[0][3];
			out[1] = dot_product(in1, in2[1]) + in2[1][3];
			out[2] = dot_product(in1, in2[2]) + in2[2][3];
			return out;
		};

		Vector pointsTransformed[8];
		for (int i = 0; i < 8; i++) {
			pointsTransformed[i] = vector_transform(points[i], trans);
		}

		Vector pos = pBaseEntity->GetAbsOrigin();
		Vector flb;
		Vector brt;
		Vector blb;
		Vector frt;
		Vector frb;
		Vector brb;
		Vector blt;
		Vector flt;

		if (!GameUtils::WorldToScreen(pointsTransformed[3], flb) || !GameUtils::WorldToScreen(pointsTransformed[5], brt)
			|| !GameUtils::WorldToScreen(pointsTransformed[0], blb) || !GameUtils::WorldToScreen(pointsTransformed[4], frt)
			|| !GameUtils::WorldToScreen(pointsTransformed[2], frb) || !GameUtils::WorldToScreen(pointsTransformed[1], brb)
			|| !GameUtils::WorldToScreen(pointsTransformed[6], blt) || !GameUtils::WorldToScreen(pointsTransformed[7], flt))
			return false;

		Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
		float left = flb.x;
		float top = flb.y;
		float right = flb.x;
		float bottom = flb.y;

		for (int i = 1; i < 8; i++) {
			if (left > arr[i].x)
				left = arr[i].x;
			if (top < arr[i].y)
				top = arr[i].y;
			if (right < arr[i].x)
				right = arr[i].x;
			if (bottom > arr[i].y)
				bottom = arr[i].y;
		}
		BotCenter = Vector(right - ((right - left) / 2), top, 0);
		TopCenter = Vector(right - ((right - left) / 2), bottom, 0);
		Left = left;
		Right = right;
	}
	else {
		Vector org = pBaseEntity->GetAbsOrigin();
		Vector head;
		if ((*pBaseEntity->GetFlags() & FL_DUCKING))
			head = org + Vector(0.f, 0.f, 52.f);
		else
			head = org + Vector(0.f, 0.f, 72.f);

		Vector org_screen, head_screen;
		if (!GameUtils::WorldToScreen(org, org_screen) || !GameUtils::WorldToScreen(head, head_screen))
			return false;

		int height = int(org_screen.y - head_screen.y);
		int width = int(height / 2);
		Left = int(head_screen.x - width / 2);
		Right = int(head_screen.x + width / 2);
		BotCenter = Vector(head_screen.x, org_screen.y);
		TopCenter = BotCenter;
		TopCenter.y = head_screen.y;
	}
	return true;
}

void CESP::DrawBones(CBaseEntity* pBaseEntity, int r, int g, int b, int a)
{
	float Red = Clientvariables->Colors.Skeletons[0] * 255;
	float Green = Clientvariables->Colors.Skeletons[1] * 255;
	float Blue = Clientvariables->Colors.Skeletons[2] * 255;


	int index = pBaseEntity->Index();
	studiohdr_t* pStudioModel = g_pModelInfo->GetStudioModel((model_t*)pBaseEntity->GetModel());
	if (pStudioModel && g_BacktrackHelper->PlayerRecord[index].records.size() > 0)
	{
		tick_record record = g_BacktrackHelper->PlayerRecord[index].records.at(0);

		float lerptime = g_BacktrackHelper->GetLerpTime();
		float desired_time = record.m_flSimulationTime + lerptime;
		float estimated_time = g_BacktrackHelper->GetEstimateServerTime();

		float SV_MAXUNLAG = 1.0f;

		float latency = g_BacktrackHelper->GetNetworkLatency();
		float m_flLerpTime = g_BacktrackHelper->GetLerpTime();
		float correct = clamp<float>(latency + m_flLerpTime, 0.0f, SV_MAXUNLAG);

		float deltaTime = correct - (estimated_time + lerptime - desired_time);

		if (fabs(deltaTime) <= 0.2f)
		{

			for (int i = 0; i < pStudioModel->numbones; i++)
			{
				mstudiobone_t* pBone = pStudioModel->pBone(i);

				if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
					continue;

				Vector sBonePos1 = GameUtils::GetBonePosition(pBaseEntity, i, record.boneMatrix), vBonePos1; //.back() for nice skeleton

				if (sBonePos1 == Vector(0, 0, 0))
					continue;
				if (!GameUtils::WorldToScreen(sBonePos1, vBonePos1))
					continue;

				Vector sBonePos2 = GameUtils::GetBonePosition(pBaseEntity, pBone->parent, record.boneMatrix), vBonePos2;//.back() for nice skeleton

				if (sBonePos2 == Vector(0, 0, 0))
					continue;
				if (!GameUtils::WorldToScreen(sBonePos2, vBonePos2))
					continue;

				Color col = Color(Red, Green, Blue, 255);

				if (record.needs_extrapolation)
					col = Color(255, 40, 0, 255);

				g_Draw.LineRGBA((int)vBonePos1.x, (int)vBonePos1.y, (int)vBonePos2.x, (int)vBonePos2.y, col.r(), col.g(), col.b(), 255);

				if (record.needs_extrapolation)
				{
					Vector position = record.m_vecOrigin;
					Vector extr_position = position;
					float simtime = record.m_flSimulationTime;
					g_BacktrackHelper->ExtrapolatePosition(pBaseEntity, extr_position, simtime, record.m_vecVelocity);

					Vector sBonePos1 = GameUtils::GetBonePosition(pBaseEntity, i, record.boneMatrix), vBonePos1; //.back() for nice skeleton

					if (sBonePos1 == Vector(0, 0, 0))
						continue;

					sBonePos1 -= position;
					sBonePos1 += extr_position;

					if (!GameUtils::WorldToScreen(sBonePos1, vBonePos1))
						continue;

					Vector sBonePos2 = GameUtils::GetBonePosition(pBaseEntity, pBone->parent, record.boneMatrix), vBonePos2;//.back() for nice skeleton

					if (sBonePos2 == Vector(0, 0, 0))
						continue;

					sBonePos2 -= position;
					sBonePos2 += extr_position;

					if (!GameUtils::WorldToScreen(sBonePos2, vBonePos2))
						continue;

					Color col = Color(0, 0, 255);

					g_Draw.LineRGBA((int)vBonePos1.x, (int)vBonePos1.y, (int)vBonePos2.x, (int)vBonePos2.y, col.r(), col.g(), col.b(), 255);
				}
			}
		}
	}
}

RECT GetViewport()
{
	RECT Viewport = { 0, 0, 0, 0 };
	int w, h;
	g_pEngine->GetScreenSize(w, h);
	Viewport.right = w;
	Viewport.bottom = h;
	return Viewport;
}

void DrawTexturedPoly(int n, Vertex_t* vertice, Color col)
{
	static int texture_id = g_pSurface->CreateNewTextureID(true);
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	g_pSurface->DrawSetTextureRGBA(texture_id, buf, 1, 1);
	g_pSurface->SetDrawColor(col);
	g_pSurface->DrawSetTexture(texture_id);
	g_pSurface->DrawTexturedPolygon(n, vertice);
}

void DrawFilledCircle(Vector2D center, Color color, float radius, float points)
{
	std::vector<Vertex_t> vertices;
	float step = (float)M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
		vertices.push_back(Vertex_t(Vector2D(radius * cosf(a) + center.x, radius * sinf(a) + center.y)));

	DrawTexturedPoly((int)points, vertices.data(), color);
}


static Color FromHSB(float hue, float saturation, float brightness)
{
	float h = hue == 1.0f ? 0 : hue * 6.0f;
	float f = h - (int)h;
	float p = brightness * (1.0f - saturation);
	float q = brightness * (1.0f - saturation * f);
	float t = brightness * (1.0f - (saturation * (1.0f - f)));

	if (h < 1)
	{
		return Color(
			(unsigned char)(brightness * 255),
			(unsigned char)(t * 255),
			(unsigned char)(p * 255)
		);
	}
	else if (h < 2)
	{
		return Color(
			(unsigned char)(q * 255),
			(unsigned char)(brightness * 255),
			(unsigned char)(p * 255)
		);
	}
	else if (h < 3)
	{
		return Color(
			(unsigned char)(p * 255),
			(unsigned char)(brightness * 255),
			(unsigned char)(t * 255)
		);
	}
	else if (h < 4)
	{
		return Color(
			(unsigned char)(p * 255),
			(unsigned char)(q * 255),
			(unsigned char)(brightness * 255)
		);
	}
	else if (h < 5)
	{
		return Color(
			(unsigned char)(t * 255),
			(unsigned char)(p * 255),
			(unsigned char)(brightness * 255)
		);
	}
	else
	{
		return Color(
			(unsigned char)(brightness * 255),
			(unsigned char)(p * 255),
			(unsigned char)(q * 255)
		);
	}
}

void CircleFilledRainbowColor(float x, float y, float rad, float rotate, int type, int resolution, IDirect3DDevice9* m_device)
{
	LPDIRECT3DVERTEXBUFFER9 g_pVB2;

	std::vector<CUSTOMVERTEX> circle(resolution + 2);

	float angle = rotate * D3DX_PI / 180, pi = D3DX_PI;

	if (type == 1)
		pi = D3DX_PI; // Full circle
	if (type == 2)
		pi = D3DX_PI / 2; // 1/2 circle
	if (type == 3)
		pi = D3DX_PI / 4; // 1/4 circle

	pi = D3DX_PI / type; // 1/4 circle

	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = D3DCOLOR_RGBA(0, 0, 0, 0);

	float hue = 0.f;

	for (int i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - rad * cos(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - rad * sin(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0;
		circle[i].rhw = 1;

		auto clr = FromHSB(hue, 1.f, 1.f);
		circle[i].color = D3DCOLOR_RGBA(clr.r(), clr.g(), clr.b(), clr.a());
		hue += 0.02;
	}

	// Rotate matrix
	int _res = resolution + 2;
	for (int i = 0; i < _res; i++)
	{
		float Vx1 = x + (cosf(angle) * (circle[i].x - x) - sinf(angle) * (circle[i].y - y));
		float Vy1 = y + (sinf(angle) * (circle[i].x - x) + cosf(angle) * (circle[i].y - y));

		circle[i].x = Vx1;
		circle[i].y = Vy1;
	}

	m_device->CreateVertexBuffer((resolution + 2) * sizeof(CUSTOMVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, NULL);

	VOID* pVertices;
	g_pVB2->Lock(0, (resolution + 2) * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(CUSTOMVERTEX));
	g_pVB2->Unlock();

	m_device->SetTexture(0, NULL);
	m_device->SetPixelShader(NULL);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_device->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX));
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
	if (g_pVB2 != NULL)
		g_pVB2->Release();
}
void CESP::SpreadCrosshair(IDirect3DDevice9* pDevice)
{
	int w, h;
	static float rot = 0.f;
	auto localplayer = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
	CBaseCombatWeapon* pWeapon = NULL;
	g_pEngine->GetScreenSize(w, h); w /= 2; h /= 2;
	auto weapons = localplayer->GetWeapons();
	for (size_t i = 0; weapons[i] != 0xFFFFFFFF; i++)
	{
		auto pEntity = g_pEntitylist->GetClientEntityFromHandle(weapons[i]);
		pWeapon = (CBaseCombatWeapon*)pEntity;
	}
	if (pWeapon)
	{
		short Index = pWeapon->GetItemDefinitionIndex();
		if (Index != 42 && Index != 59 && Index != 500)
		{
			auto accuracy = pWeapon->GetInaccuracy() * 550.f;
			CircleFilledRainbowColor(w, h, accuracy, rot, 1, 50, pDevice);

			rot += 1.f;
			if (rot > 360.f) rot = 0.f;
		}
	}

}

void CESP::DoEnemyCircle(CBaseEntity* pLocalPlayer, const Vector &vecDelta, float *flRotation)
{
	float flRadius = 360.0f;
	int iScreenWidth, iScreenHeight;
	g_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);

	Vector vRealAngles;
	g_pEngine->GetViewAngles(vRealAngles);

	Vector vForward, vRight, vUp(0.0f, 0.0f, 1.0f);

	Math::AngleVectors(vRealAngles, &vForward, NULL, NULL);

	vForward.z = 0.0f;
	VectorNormalize(vForward);
	CrossProduct(vUp, vForward, vRight);

	float flFront = DotProduct(vecDelta, vForward);
	float flSide = DotProduct(vecDelta, vRight);
	float flXPosition = flRadius * -flSide;
	float flYPosition = flRadius * -flFront;

	*flRotation = atan2(flXPosition, flYPosition) + M_PI;
	*flRotation *= 180.0f / M_PI;

	float flYawRadians = -(*flRotation) * M_PI / 180.0f;
	float flCosYaw = cos(flYawRadians);
	float flSinYaw = sin(flYawRadians);

	flXPosition = (int)((iScreenWidth / 2.0f) + (flRadius * flSinYaw));
	flYPosition = (int)((iScreenHeight / 2.0f) - (flRadius * flCosYaw));

	g_Draw.DrawRect(flXPosition, flYPosition, 10, 10, Color::Green());
}


void CESP::DrawFakeChams(IMaterial* ignorezf)
{
	
	if (Clientvariables->Visuals.fakechams)
	{
		auto pLocal = g_pEntitylist->GetClientEntity(g_pEngine->GetLocalPlayer());
		if (pLocal)
		{
			static  IMaterial* CoveredLit = ignorezf;
			if (CoveredLit)
			{
				Vector OrigAng;
				OrigAng = pLocal->GetEyeAngles();
				pLocal->SetAngle2(Vector(0, Clientvariables->Visuals.lineFakeAngle, 0));

				bool LbyColor = false;
				float NormalColor[3] = { 1, 1, 1 };
				float lbyUpdateColor[3] = { 0, 1, 0 };
				g_pRenderView->SetColorModulation(Clientvariables->Colors.FakeAngleGhost);
				g_pModelRender->ForcedMaterialOverride(CoveredLit);
				pLocal->draw_model(0x1, 255);
				g_pModelRender->ForcedMaterialOverride(nullptr);
				pLocal->SetAngle2(OrigAng);
			}
		}
	}
}




void drawworld()
{
	if (Clientvariables->Visuals.skycolor)
	{
		for (MaterialHandle_t i = g_pMaterialSystem->FirstMaterial(); i != g_pMaterialSystem->InvalidMaterial(); i = g_pMaterialSystem->NextMaterial(i))
		{
			IMaterial *pMaterial = g_pMaterialSystem->GetMaterial(i);
			if (!pMaterial)
				continue;
			if (strstr(pMaterial->GetTextureGroupName(), "SkyBox"))
				if (Clientvariables->Visuals.skycolor)
					pMaterial->ColorModulate(Clientvariables->Colors.skycolorxd[0], Clientvariables->Colors.skycolorxd[1], Clientvariables->Colors.skycolorxd[2]);
				else
					pMaterial->ColorModulate(1, 1, 1);

		}
	}




	if (Clientvariables->Visuals.worldcolor)
	{
		for (MaterialHandle_t i = g_pMaterialSystem->FirstMaterial(); i != g_pMaterialSystem->InvalidMaterial(); i = g_pMaterialSystem->NextMaterial(i))
		{
			IMaterial *pMaterial = g_pMaterialSystem->GetMaterial(i);
			if (!pMaterial)
				continue;
			const char* group = pMaterial->GetTextureGroupName();
			const char* name = pMaterial->GetName();

			if (strstr(group, "World textures"))
				if (Clientvariables->Visuals.worldcolor)
					pMaterial->ColorModulate(Clientvariables->Colors.worldcolorxd[0], Clientvariables->Colors.worldcolorxd[1], Clientvariables->Colors.worldcolorxd[2]);
				else
					pMaterial->ColorModulate(1, 1, 1);
			
		}
	}





}



void CESP::DrawPlayer(CBaseEntity* pPlayer, CBaseEntity* pLocalPlayer)
{


	drawworld();
	Vector m_position = pPlayer->GetAbsOrigin();
	Vector m_top_position = m_position;
	if (*pPlayer->GetFlags() & IN_DUCK)
		m_top_position += Vector(0, 0, 52);
	else
		m_top_position += Vector(0, 0, 72);

	Vector footpos, headpos;

	if (pPlayer->IsDormant() && flPlayerAlpha[pPlayer->GetIndex()] > 0.f)
		flPlayerAlpha[pPlayer->GetIndex()] -= 0.5f;
	else if (flPlayerAlpha[pPlayer->GetIndex()] < 255.f && !pPlayer->IsDormant())
		flPlayerAlpha[pPlayer->GetIndex()] = 255.f;

	if (pPlayer != G::LocalPlayer && !pPlayer->IsDormant() && Clientvariables->Visuals.SnapLines) {
		RECT View = GetViewport();
		int MidX = View.right / 1.9999999999999;
		int MidY = View.bottom / 2;
		Vector VecOrigin = pPlayer->GetAbsOrigin(), SecondVector;
		float Red = Clientvariables->Colors.Snaplines[0] * 255;
		float Green = Clientvariables->Colors.Snaplines[1] * 255;
		float Blue = Clientvariables->Colors.Snaplines[2] * 255;

		if (GameUtils::WorldToScreen(VecOrigin, SecondVector)) {
			g_Draw.LineRGBA(MidX, MidY, SecondVector.x, SecondVector.y, Red, Green, Blue, flPlayerAlpha[pPlayer->GetIndex()]);
		}
	}

	if (GameUtils::WorldToScreen(m_position, footpos) && GameUtils::WorldToScreen(m_top_position, headpos)) {
		Vector Bot, Top;
		float Left, Right;
		GetCBaseEntityBox(pPlayer, Bot, Top, Left, Right, true);
		int height = Bot.y - Top.y;
		int width = Right - Left;
		int x = Left;
		int y = Top.y;

		CBaseCombatWeapon* pMainWeapon = pPlayer->GetWeapon();

		if (Clientvariables->Visuals.Bones)
			DrawBones(pPlayer, 255, 255, 255, flPlayerAlpha[pPlayer->GetIndex()]);

		if (Clientvariables->Visuals.BoundingBox) {
			float Red = Clientvariables->Colors.BoundingBox[0] * 255;
			float Green = Clientvariables->Colors.BoundingBox[1] * 255;
			float Blue = Clientvariables->Colors.BoundingBox[2] * 255;

			if (pPlayer->IsDormant()) {
				g_Draw.Box(x - 1, y - 1, width + 2, height + 2, 0, 0, 0, flPlayerAlpha[pPlayer->GetIndex()]);
				g_Draw.Box(x, y, width, height, Red, Green, Blue, flPlayerAlpha[pPlayer->GetIndex()]);
				g_Draw.Box(x + 1, y + 1, width - 2, height - 2, 0, 0, 0, flPlayerAlpha[pPlayer->GetIndex()]);
			}
			else {
				g_Draw.Box(x - 1, y - 1, width + 2, height + 2, 0, 0, 0, flPlayerAlpha[pPlayer->GetIndex()]);
				g_Draw.Box(x, y, width, height, Red, Green, Blue, flPlayerAlpha[pPlayer->GetIndex()]);
				g_Draw.Box(x + 1, y + 1, width - 2, height - 2, 0, 0, 0, flPlayerAlpha[pPlayer->GetIndex()]);
			}
		}



		if (Clientvariables->Visuals.Armor)
		{

		}
		int bot_add = 0;
		int top_add = 0;
		if (Clientvariables->Visuals.Flags)
		{
			player_info_s info;
			g_pEngine->GetPlayerInfo(pPlayer->Index(), &info);

			if (pPlayer->IsScoped())
				g_Draw.StringA(g_Draw.font_esp, false, x + width + 3, y + 16, 45, 83, 188, flPlayerAlpha[pPlayer->GetIndex()], "ZOOM");


			if (pPlayer->GetArmor() > 0)
			{
				char hp[50];
				sprintf_s(hp, sizeof(hp), "%i", pPlayer->GetArmor());
				if (pPlayer->HasHelmet())
					g_Draw.StringA(g_Draw.font_esp, false, x + width + 3, y + 8, 255, 255, 255, flPlayerAlpha[pPlayer->GetIndex()], "HK");
				else
					g_Draw.StringA(g_Draw.font_esp, false, x + width + 3, y + 8, 255, 255, 255, flPlayerAlpha[pPlayer->GetIndex()], "K");
			}


			if (G::ResolverMode[pPlayer->GetIndex()] == 0)
				g_Draw.StringA(g_Draw.font_esp, false, x + width + 10, y, 200, 200, 200, 255, " lby");
			else if (G::ResolverMode[pPlayer->GetIndex()] == 1)
				g_Draw.StringA(g_Draw.font_esp, false, x + width + 10, y, 200, 200, 200, 255, " lby Breaker Predict");
			else if (G::ResolverMode[pPlayer->GetIndex()] == 2)
				g_Draw.StringA(g_Draw.font_esp, false, x + width + 10, y, 200, 200, 200, 255, " Bruteforce");
			else if (G::ResolverMode[pPlayer->GetIndex()] == 3)
				g_Draw.StringA(g_Draw.font_esp, false, x + width + 10, y, 200, 200, 200, 255, " lby update");
			else if (G::ResolverMode[pPlayer->GetIndex()] == 4)
				g_Draw.StringA(g_Draw.font_esp, false, x + width + 10, y, 200, 200, 200, 255, " static");
			else if (G::ResolverMode[pPlayer->GetIndex()] == 5)
				g_Draw.StringA(g_Draw.font_esp, false, x + width + 10, y, 200, 200, 200, 255, " stored");
			else if (G::ResolverMode[pPlayer->GetIndex()] == 6)
				g_Draw.StringA(g_Draw.font_esp, false, x + width + 10, y, 200, 200, 200, 255, " brute delta");
			else if (G::ResolverMode[pPlayer->GetIndex()] == 7)
				g_Draw.StringA(g_Draw.font_esp, false, x + width + 10, y, 200, 200, 200, 255, " last lby");
			else if (G::ResolverMode[pPlayer->GetIndex()] == 8)
				g_Draw.StringA(g_Draw.font_esp, false, x + width + 10, y, 200, 200, 200, 255, " adjusting lby");
			else if (G::ResolverMode[pPlayer->GetIndex()] == 9)
				g_Draw.StringA(g_Draw.font_esp, false, x + width + 10, y, 200, 200, 200, 255, " fakewalk");
			else if (G::ResolverMode[pPlayer->GetIndex()] == 10)
				g_Draw.StringA(g_Draw.font_esp, false, x + width + 10, y, 200, 200, 200, 255, " adjusting last lby");
		}
	

		if (Clientvariables->Visuals.Health && Clientvariables->Visuals.HealthType==0) {
			int health = 100 - pPlayer->GetHealth();
			int w = 4;
			if (width < 4)
				w = width;
			int hr, hg, hb;

			hr = 255 - (pPlayer->GetHealth() * 2.55);
			hg = pPlayer->GetHealth() * 2.55;
			hb = 0;

			if (pPlayer->IsDormant()) {
				g_Draw.FillRGBA(x - (5), y, w, height, 0, 0, 0, flPlayerAlpha[pPlayer->GetIndex()]);
				g_Draw.FillRGBA(x - (5), y + health * height / 100, w, pPlayer->GetHealth() * height / 100, hr, hg, hb, flPlayerAlpha[pPlayer->GetIndex()]);
				g_Draw.Box(x - (5), y, w, height, 0, 0, 0, flPlayerAlpha[pPlayer->GetIndex()]);
			}
			else {
				g_Draw.FillRGBA(x - (5), y, w, height, 0, 0, 0, flPlayerAlpha[pPlayer->GetIndex()]);
				g_Draw.FillRGBA(x - (5), y + health * height / 100, w, pPlayer->GetHealth() * height / 100, hr, hg, hb, flPlayerAlpha[pPlayer->GetIndex()]);
				g_Draw.Box(x - (5), y, w, height, 0, 0, 0, flPlayerAlpha[pPlayer->GetIndex()]);
			}
			if (pPlayer->GetHealth() <= 100) {
				char hp[20];
				sprintf_s(hp, sizeof(hp), "%i", pPlayer->GetHealth());

				g_Draw.StringA(g_Draw.font_esp, false, x - g_Draw.getWidht(hp, g_Draw.font_espnum), y + health * height / 100 - 6, 255, 255, 255, flPlayerAlpha[pPlayer->GetIndex()], "%i", pPlayer->GetHealth());
			}
		}

		Box box;
		if (Clientvariables->Visuals.Health && Clientvariables->Visuals.HealthType == 1)
		{
			if (get_box(pPlayer, box, true))
			{
				Box HealthBar = box;
				HealthBar.y += (HealthBar.h + 6);
				HealthBar.h = 4;

				float HealthValue = pPlayer->GetHealth();
				float HealthPerc = HealthValue / 100.f;
				float Width = (box.w * HealthPerc);
				HealthBar.w = Width;

				// --  Main Bar -- //

				Vertex_t Verts[4];
				Verts[0].Init(Vector2D(HealthBar.x, HealthBar.y));
				Verts[1].Init(Vector2D(HealthBar.x + box.w + 5, HealthBar.y));
				Verts[2].Init(Vector2D(HealthBar.x + box.w, HealthBar.y + 5));
				Verts[3].Init(Vector2D(HealthBar.x - 5, HealthBar.y + 5));
				
				g_Draw.polygon_outlined(4, Verts, Color(10, 10, 10, 255), Color(255, 255, 255, 170));

				Vertex_t Verts2[4];
				Verts2[0].Init(Vector2D(HealthBar.x + 1, HealthBar.y + 1));
				Verts2[1].Init(Vector2D(HealthBar.x + HealthBar.w + 4, HealthBar.y + 1));
				Verts2[2].Init(Vector2D(HealthBar.x + HealthBar.w, HealthBar.y + 5));
				Verts2[3].Init(Vector2D(HealthBar.x - 4, HealthBar.y + 5));

				//Color c = GetPlayerColor(pPlayer);
				Color c = Color(0, 250, 0, 250);
				g_Draw.polygon(4, Verts2, c);

				Verts2[0].Init(Vector2D(HealthBar.x + 1, HealthBar.y + 1));
				Verts2[1].Init(Vector2D(HealthBar.x + HealthBar.w + 2, HealthBar.y + 1));
				Verts2[2].Init(Vector2D(HealthBar.x + HealthBar.w, HealthBar.y + 2));
				Verts2[3].Init(Vector2D(HealthBar.x - 2, HealthBar.y + 2));

				g_Draw.polygon(4, Verts2, Color(255, 255, 255, 40));
			}
			
		}






		static class Text {
		public:
			std::string text;
			int side;
			int Font;
			Color color;

			Text(std::string text, int side, int Font, Color color)
				: text(text)
				, side(side)
				, Font(Font)
				, color(color)
			{
			}
		};
		std::vector<Text> texts;

		if (Clientvariables->Visuals.Name) {
			player_info_s info;
			g_pEngine->GetPlayerInfo(pPlayer->Index(), &info);
			g_Draw.StringA(g_Draw.font_esp, true, x + width / 2, y - 12 - top_add, 255, 255, 255, flPlayerAlpha[pPlayer->GetIndex()], "%s", info.m_szPlayerName);
			top_add += 12;
		}


		if (Clientvariables->Visuals.Weapon) {
			UINT* hWeapons = pPlayer->GetWeapons();
			if (hWeapons) {
				if (pMainWeapon) {
					std::string s1 = pMainWeapon->GetGunIcon();
					std::string ammo;
					if (Clientvariables->Visuals.Ammo && pMainWeapon->Clip1() != -1 && pMainWeapon->GetWeaponType() != WEAPONCLASS::WEPCLASS_INVALID) {
						ammo.append(XorStr("("));
						ammo.append(to_string(pMainWeapon->Clip1()));
						ammo.append(XorStr("/"));
						ammo.append(to_string(pMainWeapon->GetMaxAmmoReserve()));
						ammo.append(XorStr(")"));
					}
					g_Draw.StringA(g_Draw.font_icon, true, x + width / 2, y + height + bot_add + 5, 255, 255, 255, flPlayerAlpha[pPlayer->GetIndex()], XorStr("%s"), s1.c_str());
					g_Draw.StringA(g_Draw.font_esp, true, x + width / 2, y + height + bot_add + 15, 255, 255, 255, flPlayerAlpha[pPlayer->GetIndex()], XorStr("%s"), ammo.c_str());
				}
			}
		}
	}
}


void CESP::DrawC4(CBaseEntity* pBomb, bool is_planted, CBaseEntity* pLocalPlayer)
{
	Vector Bot, Top;
	float Left, Right;
	GetCBaseEntityBox(pBomb, Bot, Top, Left, Right, false);
	int height = Bot.y - Top.y;
	int width = Right - Left;
	DWORD col = is_planted ? D3DCOLOR_RGBA(250, 42, 42, 255) : D3DCOLOR_RGBA(255, 255, 0, 255);
	g_Draw.StringA(g_Draw.font_esp, true, Left + width / 2, Bot.y, 255, 0, 0, flPlayerAlpha[pBomb->GetIndex()], "%s", "[C4]");

	if (is_planted) {
		CBomb* bomb = (CBomb*)pBomb;
		float flBlow = bomb->GetC4BlowTime();
		float TimeRemaining = flBlow - (g_pGlobals->interval_per_tick * pLocalPlayer->GetTickBase());
		if (TimeRemaining < 0)
			TimeRemaining = 0;
		char buffer[64];

		if (bomb->IsDefused())
			sprintf_s(buffer, XorStr("Defused"));
		else
			sprintf_s(buffer, XorStr("%.1fs Remaining!"), TimeRemaining);

		g_Draw.StringA(g_Draw.font_esp, true, Left + width / 2, Bot.y + 13, 255, 0, 0, 255, "%s", buffer);

		if (!bomb->IsDefused()) {
			float a = 450.7f;
			float b = 75.68f;
			float c = 789.2f;
			float d = ((pLocalPlayer->GetAbsOrigin().DistTo(pBomb->GetOrigin()) - b) / c);
			float flDamage = a * expf(-d * d);

			auto GetArmourHealth = [](float flDamage, int ArmorValue) {
				float flCurDamage = flDamage;

				if (flCurDamage == 0.0f || ArmorValue == 0)
					return flCurDamage;

				float flArmorRatio = 0.5f;
				float flArmorBonus = 0.5f;
				float flNew = flCurDamage * flArmorRatio;
				float flArmor = (flCurDamage - flNew) * flArmorBonus;

				if (flArmor > ArmorValue) {
					flArmor = ArmorValue * (1.0f / flArmorBonus);
					flNew = flCurDamage - flArmor;
				}

				return flNew;
			};

			float damage = max((int)ceilf(GetArmourHealth(flDamage, pLocalPlayer->GetArmor())), 0);

			g_Draw.StringA(g_Draw.font_esp, true, Left + width / 2, Bot.y + 26, 255, 0, 0, 255, "Damage: %f", damage);
		}
	}
}





bool IsBadReadPtr(void* p)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	if (::VirtualQuery(p, &mbi, sizeof(mbi)))
	{
		DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
		bool b = !(mbi.Protect & mask);
		// check the page is not a guard page
		if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) b = true;

		return b;
	}
	return true;
}

void CESP::DrawThrowable(CBaseEntity* pThrowable)
{
	const model_t* nadeModel = pThrowable->GetModel();

	if (!nadeModel)
		return;

	studiohdr_t* hdr = g_pModelInfo->GetStudioModel(nadeModel);

	if (!hdr)
		return;

	if (!strstr(hdr->name, XorStr("thrown")) && !strstr(hdr->name, XorStr("dropped")))
		return;

	std::string nadeName = XorStr("unknown");

	IMaterial* mats[32];
	g_pModelInfo->GetModelMaterials(nadeModel, hdr->numtextures, mats);

	for (int i = 0; i < hdr->numtextures; i++) {
		IMaterial* mat = mats[i];

		if (!mat)
			continue;

		if (strstr(mat->GetName(), XorStr("flashbang"))) {
			nadeName = XorStr("flashbang");

			break;
		}
		else if (strstr(mat->GetName(), XorStr("m67_grenade")) || strstr(mat->GetName(), XorStr("hegrenade"))) {
			nadeName = XorStr("grenade");

			break;
		}
		else if (strstr(mat->GetName(), XorStr("smoke"))) {
			nadeName = XorStr("smoke");

			break;
		}
		else if (strstr(mat->GetName(), XorStr("decoy"))) {
			nadeName = XorStr("decoy");

			break;
		}
		else if (strstr(mat->GetName(), XorStr("incendiary")) || strstr(mat->GetName(), XorStr("molotov"))) {
			nadeName = XorStr("molotov");

			break;
		}
	}
	Vector Bot, Top;
	float Left, Right;
	GetCBaseEntityBox(pThrowable, Bot, Top, Left, Right, false);
	int height = Bot.y - Top.y;
	int width = Right - Left;

	g_Draw.StringA(g_Draw.font_esp, true, Left + width / 2, Bot.y, 255, 255, 255, 255, nadeName.c_str());
}

void CESP::DrawDroppedWeapon(CBaseCombatWeapon* pWeapon)
{
	CBaseEntity* pCBaseEntity = (CBaseEntity*)pWeapon;
	CBaseCombatWeapon* Weapon = (CBaseCombatWeapon*)pWeapon;

	if (!pCBaseEntity || !g_pModelInfo)
		return;

	std::string sCurWeapon = g_pModelInfo->GetModelName(pCBaseEntity->GetModel());
	if (!sCurWeapon.empty() && sCurWeapon.find(XorStr("w_")) != std::string::npos) {
		std::string name;

		if (sCurWeapon.find(XorStr("defuser")) != std::string::npos /* insert check for defuser here*/)
			name = XorStr("defuser");
		else {
			CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)pCBaseEntity;
			if (!pWeapon)
				return;
			name = GetWeaponName(pWeapon);
		}

		Vector Bot, Top;
		float Left, Right;
		GetCBaseEntityBox(pCBaseEntity, Bot, Top, Left, Right, false);
		int width = Right - Left;
		int height = Bot.y - Top.y;

		float Red = Clientvariables->Colors.DroppedWeapon[0] * 255;
		float Green = Clientvariables->Colors.DroppedWeapon[1] * 255;
		float Blue = Clientvariables->Colors.DroppedWeapon[2] * 255;

		g_Draw.StringA(g_Draw.font_esp, true, Left + width / 2, Bot.y, Red, Green, Blue, 255, name.c_str());
		g_Draw.Box(Left, Top.y, width, height, 0, 0, 0, 255);
		g_Draw.Box(Left + 1, Top.y + 1, width - 2, height - 2, Red, Green, Blue, 255);
		g_Draw.Box(Left + 2, Top.y + 2, width - 4, height - 4, 0, 0, 0, 255);
	}
}

Vector MEME2(Vector EntityPos, Vector LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom, bool* viewCheck)
{
	float r_1, r_2;
	float x_1, y_1;
	r_1 = -(EntityPos.y - LocalPlayerPos.y);
	r_2 = EntityPos.x - LocalPlayerPos.x;
	float Yaw = angle - 90.0f;
	float yawToRadian = Yaw * (float)(M_PI / 180.0F);
	x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20;
	y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20;
	*viewCheck = y_1 < 0;
	x_1 *= zoom;
	y_1 *= zoom;
	int sizX = sizeX / 2;
	int sizY = sizeY / 2;
	x_1 += sizX;
	y_1 += sizY;

	if (x_1 < 5)
		x_1 = 5;

	if (x_1 > sizeX - 5)
		x_1 = sizeX - 5;

	if (y_1 < 5)
		y_1 = 5;

	if (y_1 > sizeY - 5)
		y_1 = sizeY - 5;

	x_1 += posX;
	y_1 += posY;

	return Vector(x_1, y_1, 0);
}

FORCEINLINE float DotProduct64(const Vector& a, const Vector& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

void CESP::Loop()
{
	if (!G::LocalPlayer)
		return;


	for (int i = 1; i <= g_pEntitylist->GetHighestEntityIndex(); i++) {
		CBaseEntity* pBaseEntity = g_pEntitylist->GetClientEntity(i);

		if (!pBaseEntity)
			continue;

		ClientClass* pClass = (ClientClass*)pBaseEntity->GetClientClass();

		if (pClass->m_ClassID == int(EClassIds::CCSPlayer) && Clientvariables->Visuals.EspEnable) {
			if (pBaseEntity->GetHealth() < 1)
				continue;

			bool is_friendly = pBaseEntity->GetTeamNum() == G::LocalPlayer->GetTeamNum();

			if (Clientvariables->Visuals.LocalPlayer && pBaseEntity == G::LocalPlayer)
				g_ESP->DrawPlayer(pBaseEntity, G::LocalPlayer);

			if (!is_friendly || is_friendly && !Clientvariables->Visuals.EnemyOnly)
				g_ESP->DrawPlayer(pBaseEntity, G::LocalPlayer);
		}

		Vector buf, pos = pBaseEntity->GetAbsOrigin();

		if (pos.x == 0 || pos.y == 0 || pos.z == 0 || !GameUtils::WorldToScreen(pos, buf))
			continue;

		if ((pClass->m_ClassID != int(EClassIds::CBaseWeaponWorldModel) && (strstr(pClass->m_pNetworkName, XorStr("Weapon")) || pClass->m_ClassID == int(EClassIds::CDEagle) || pClass->m_ClassID == int(EClassIds::CAK47))) && Clientvariables->Visuals.DroppedWeapons == 1)
			DrawDroppedWeapon((CBaseCombatWeapon*)pBaseEntity);

		if ((pClass->m_ClassID == int(EClassIds::CC4) || pClass->m_ClassID == int(EClassIds::CPlantedC4))) {
			if (Clientvariables->Visuals.Bomb)
				DrawC4(pBaseEntity, pClass->m_ClassID == int(EClassIds::CPlantedC4), G::LocalPlayer);
		}

		if (strstr(pClass->m_pNetworkName, XorStr("Projectile")) && Clientvariables->Visuals.ThrownNades)
			DrawThrowable(pBaseEntity);
	}
}

void CESP::DrawScope(CBaseEntity* pLocalPlayer)
{
	CBaseCombatWeapon* pLocalWeapon = pLocalPlayer->GetWeapon();

	if (pLocalWeapon) {
		if (pLocalWeapon->IsSniper()) {
			if (pLocalPlayer->IsScoped()) {
				int width = 0;
				int height = 0;
				g_pEngine->GetScreenSize(width, height);
				int centerX = static_cast<int>(width * 0.5f);
				int centerY = static_cast<int>(height * 0.5f);
				g_pSurface->SetDrawColor(Color(0, 0, 0, 255));
				g_pSurface->DrawLine(0, centerY, width, centerY);
				g_pSurface->DrawLine(centerX, 0, centerX, height);
			}
		}
	}
}



































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































