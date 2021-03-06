#pragma once

class CDraw {
public:
	void Init();
	int getWidht(const char* input, unsigned int font);
	void DrawRect(int x1, int y1, int x2, int y2, Color clr);
	std::wstring stringToWide(const std::string& text);
	void String(int x, int y, int r, int g, int b, const wchar_t* pszText);
	void StringA(unsigned long Font, bool center, int x, int y, int r, int g, int b, int a, const char* input, ...);
	void FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a);
	void FillRGBAOutlined(int x, int y, int w, int h, int r, int g, int b, int a);
	void Box(int x, int y, int w, int h, int r, int g, int b, int a);
	void polyline(int * x, int * y, int count, Color color);
	void polygon(int count, Vertex_t * Vertexs, Color color);
	void polygon_outlined(int count, Vertex_t * Vertexs, Color color, Color colorLine);
	void LineRGBA(int x1, int y1, int x2, int y2, int r, int g, int b, int a);
	void Text(int x, int y, int r, int g, int b, int a, bool bCenter, unsigned int font, const char* fmt, ...);
	void ColoredCircle(int x, int y, int radius, int r, int g, int b, int a);
	void DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float& flRainbow);
	void Draw3DBox(Vector* boxVectors, Color color);
	void DrawLine(int x0, int y0, int x1, int y1, Color col);
	void DrawTextureByteArray(Vector2D p0, Vector2D p1, Vector2D p2, Vector2D p3, const unsigned char* pRawRGBAData);
	unsigned int font_esp, font_GUI2, font_espinfo, font_espnum, font_build, font_bui2, font_arrows, font_icon, in_game_logging_font;
	Vector2D GetTextSize(unsigned int Font, std::string Input);
	void DrawF(int X, int Y, unsigned int Font, bool center_width, bool center_height, Color Color, std::string Input);
	LPDIRECT3DDEVICE9 device;
};
extern HINSTANCE hAppInstance;
extern CDraw g_Draw;