class CPanel
{
public:
	const char* GetName( unsigned int Panel )
	{
		typedef const char*(__thiscall* Fn)(void*, unsigned int);
		return CallVFunction<Fn>(this, 36)(this, Panel );
	}
	void SetMouseInputEnabled(unsigned int panel, bool state)
	{
		return CallVFunction<void(__thiscall *)(PVOID, int, bool)>(this, 32)(this, panel, state);
	}
};