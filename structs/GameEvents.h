#pragma once
class IGameEvent {
public:
	virtual ~IGameEvent() = 0;
	virtual const char* GetName() const = 0;

	virtual bool IsReliable() const = 0;
	virtual bool IsLocal() const = 0;
	virtual bool IsEmpty(const char* keyName = NULL) = 0;

	virtual bool GetBool(const char* keyName = NULL, bool defaultValue = false) = 0;
	virtual int GetInt(const char* keyName = NULL, int defaultValue = 0) = 0;
	virtual unsigned long GetUint64(const char* keyName = NULL, unsigned long defaultValue = 0) = 0;
	virtual float GetFloat(const char* keyName = NULL, float defaultValue = 0.0f) = 0;
	virtual const char* GetString(const char* keyName = NULL, const char* defaultValue = "") = 0;
	virtual const wchar_t* GetWString(const char* keyName, const wchar_t* defaultValue = L"") = 0;

	virtual void SetBool(const char* keyName, bool value) = 0;
	virtual void SetInt(const char* keyName, int value) = 0;
	virtual void SetUint64(const char* keyName, unsigned long value) = 0;
	virtual void SetFloat(const char* keyName, float value) = 0;
	virtual void SetString(const char* keyName, const char* value) = 0;
	virtual void SetWString(const char* keyName, const wchar_t* value) = 0;
};
extern int GetServerSideID(int CBaseEntityIndex);
class IGameEventListener2 {
public:
	virtual ~IGameEventListener2() {}
	virtual void FireGameEvent(IGameEvent* Event) = 0;
	virtual int GetEventDebugID() { return 42; }
};
extern float logged_hits[64];

struct ResolveData
{
	int ShotsFiredAtTarget = 0;
	QAngle LastAnglesHit;
	QAngle AdjustedAngles = QAngle(0, 0, 0);
	QAngle LastAttemptedAngles = QAngle(0, 0, 0);
	int MissedShots = 0;
};
extern std::map<int, ResolveData> PlayerData;
class bf_read;
class bf_write;
class IGameEventManager {
public:
	/*
	bool AddListener(IGameEventListener2* listener, const char* name, bool bServerSide)
	{
		typedef bool(__thiscall * OriginalFn)(PVOID, IGameEventListener2*, const char*, bool);
		return CallVFunction<OriginalFn>(this, 3)(this, listener, name, bServerSide);
	}

	bool FireEventClientSide(IGameEvent* event)
	{
		typedef bool(__thiscall * OriginalFn)(PVOID, IGameEvent*);
		return CallVFunction<OriginalFn>(this, 9)(this, event);
	}*/
	virtual int __Unknown_1(int* dwUnknown) = 0;

	// load game event descriptions from a file eg "resource\gameevents.res"
	virtual int LoadEventsFromFile(const char *filename) = 0;

	// removes all and anything
	virtual void Reset() = 0;

	// adds a listener for a particular event
	virtual bool AddListener(IGameEventListener2 *listener, const char *name, bool bServerSide) = 0;

	// returns true if this listener is listens to given event
	virtual bool FindListener(IGameEventListener2 *listener, const char *name) = 0;

	// removes a listener 
	virtual int RemoveListener(IGameEventListener2 *listener) = 0;

	// create an event by name, but doesn't fire it. returns NULL is event is not
	// known or no listener is registered for it. bForce forces the creation even if no listener is active
	virtual IGameEvent *CreateEvent(const char *name, bool bForce, unsigned int dwUnknown) = 0;

	// fires a server event created earlier, if bDontBroadcast is set, event is not send to clients
	virtual bool FireEvent(IGameEvent *event, bool bDontBroadcast = false) = 0;

	// fires an event for the local client only, should be used only by client code
	virtual bool FireEventClientSide(IGameEvent *event) = 0;

	// create a new copy of this event, must be free later
	virtual IGameEvent *DuplicateEvent(IGameEvent *event) = 0;

	// if an event was created but not fired for some reason, it has to bee freed, same UnserializeEvent
	virtual void FreeEvent(IGameEvent *event) = 0;

	// write/read event to/from bitbuffer
	virtual bool SerializeEvent(IGameEvent *event, bf_write *buf) = 0;

	// create new KeyValues, must be deleted
	virtual IGameEvent *UnserializeEvent(bf_read *buf) = 0;
};

class cGameEvent : public IGameEventListener2 {

public:
	void FireGameEvent(IGameEvent* event);
	int GetEventDebugID();
	void RegisterSelf();
	void Register();
};

extern cGameEvent g_Event;

class trace_info {
public:
	trace_info(Vector starts, Vector positions, float times, int userids)
	{
		this->start = starts;
		this->position = positions;
		this->time = times;
		this->userid = userids;
	}

	Vector position;
	Vector start;
	float time;
	int userid;
};

extern std::vector<trace_info> trace_logs;
