#pragma once

#include "iDefine.h"

#include "iPoint.h"

// ===================================
// Mouse
// ===================================
#define iQueueKeySet(num, method)	iQueueKey::share()->set(num, method);
#define iQueueKeyUpdate(dt)			iQueueKey::share()->update(dt);
#define iQueueKeyAdd(s,p)			iQueueKey::share()->add(s,p);

struct QueueKey
{
	iKeyStat s;
	iPoint p;
};

class iQueueKey
{
private:
	iQueueKey();
public:
	static iQueueKey* share();
	virtual ~iQueueKey();
	
	void set(int maxNUm, METHOD_KEY m);
	void add(iKeyStat stat, iPoint p);
	void update(float dt);

	METHOD_KEY method;
	QueueKey* qk;
	int qkNum;
};

// ===========================================
// keyboard
// ===========================================
//#define iKeyboardSet(n, m) iKeyboard::share()->set(n, m)
#define iKeyboardSet iKeyboard::share
#define iKeyboardAdd(down, parm) iKeyboard::share()->add(down, parm)
#define iKeyboardUpdate(dt) iKeyboard::share()->update(dt)
#define iKeyboardDown(key)  iKeyboard::share()->keyDown(key)
#define iKeyboardStat(key)  iKeyboard::share()->keyStat(key)

class iKeyboard
{
private:
	iKeyboard();
public:
	static iKeyboard* share();
	virtual ~iKeyboard();

	// 대기 버퍼를 사용하지 않음.
	// void set(int maxNUm, METHOD_KEY m);
	void add(bool down, int parm);
private:
	void add(bool pressed, int& keydown, int key);
public:
	void update(float dt);

	bool keyStat(int key);
	bool keyDown(int key);

	// keydown 눌렀을때 1번 발생
	// keystat 누르고 있을때 계속 발생
	int keydown, keystat;
};


// ===========================================
// GamePad
// ===========================================
