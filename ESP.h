#pragma once


struct Box
{
	int x = -1, y = -1, w = -1, h = -1;
};

struct CUSTOMVERTEX {
	FLOAT x, y, z;
	FLOAT rhw;
	DWORD color;
	//  FLOAT tu, tv;   
};

struct player_esp {
	CBaseEntity* pPlayer = nullptr;
	bool is_visible = false;
	bool render = false;
	int screen_x = 0, screen_y = 0, width = 0, height = 0;
	DWORD color = D3DCOLOR_RGBA(255, 255, 255, 255);

};
class CESP {
public:

	player_esp player_draw_array[64];
	bool get_box(CBaseEntity* m_entity, Box& box, bool dynamic);
	void DrawPlayer(CBaseEntity* pPlayer, CBaseEntity* pLocalPlayer);
	void DrawC4(CBaseEntity* pBomb, bool is_planted, CBaseEntity* pLocalPlayer);

	void DrawThrowable(CBaseEntity* pThrowable);
	void DrawDroppedWeapon(CBaseCombatWeapon* pWeapon);

	void DrawScope(CBaseEntity* pLocalPlayer);
	void DrawBones(CBaseEntity* pBaseEntity, int r, int g, int b, int a);
	void SpreadCrosshair(IDirect3DDevice9 * pDevice);
	void DoEnemyCircle(CBaseEntity* pLocalPlayer, const Vector &vecDelta, float *flRotation);
	void DrawFakeChams(IMaterial*);
	void Loop();
};
extern CESP* g_ESP;