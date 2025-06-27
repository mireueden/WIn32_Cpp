#pragma once

#include "iStd.h"

// target 생산 목표 개수(수주 개수)
// curr(생산중 + 생산완료)
// complete 생산완료

// 디스플레이 
// 진행률 curr / target
// 생산률 compelte / target
extern int target, curr, complete, broken;


void loadDTObject();
void freeDTObject();
void drawDTObject(float dt, iPoint off);
bool keyDTObject(iKeyStat stat, iPoint point);

void startMake(int target);
void startMove(int unitIndex);

struct DTUnit;
typedef void (*MethodWorked)(DTUnit* obj);

struct DTUnit
{
	DTUnit(int index);
	virtual ~DTUnit();

	virtual bool start(MethodWorked m);
	virtual void paint(float dt, iPoint position) = 0;

	float run(float dt);
	iRect touchRect(iPoint position);

	int index;
	iImage* img;
	iPoint position;
	float delta, _delta;

	MethodWorked methodWorked;
};

// index==0
// 수주가 있을경우 생산시작

// 그외
// index-1프로세서에서 생산된 물건 있으면 생산시작

// index==max-1
// 생산완료되면, 수주 채워짐
enum StateMake
{
	StateMakeReady = 0,
	StateMakeMake,
	StateMakeBroken,
	StateMakeComplete,

	StateMakeMax
};

// index : 0 ~ 99
// delta == 0.0f	: 명령 받을 준비
// delta < _delta	: 생산중 & 명령 받을 수 X
// delta >= _delta	: 생산완료 & 명령 받을 수 X
struct DTUnitMake : DTUnit
{
	iImage** imgs;// img = imgs[sm];
	StateMake sm;

	bool* slot; // 0 ~ 4 재료 담는 곳, 5 ~ 9 생산 완료 담는곳

	DTUnitMake(int index);
	virtual ~DTUnitMake();

	virtual bool start(MethodWorked m);
	virtual void paint(float dt, iPoint position);

	static void cbWorked0(DTUnit* obj);
	static void cbWorked1(DTUnit* obj);
	static void cbWorked2(DTUnit* obj);
	static void cbWorked3(DTUnit* obj);
	static void cbWorked4(DTUnit* obj);
	static void cbWorked5(DTUnit* obj);
	static void cbWorked9(DTUnit* obj);
};

struct MakeInfo
{
	iSize size;
	iColor4f color;
	float delta;
};

extern MakeInfo mi[5];


enum StateMove
{
	StateMoveReady = 0,
	StateMoveMove,
	StateMovePick,

	StateMoveMax
};

// index : 100 ~ 199
struct DTUnitMove : DTUnit
{
	iImage** imgs;
	StateMove sm;

	DTUnitMove(int index);
	virtual ~DTUnitMove();

	virtual bool start(MethodWorked m);
	virtual void paint(float dt, iPoint position);

	void setAreaRange(iPoint sp, iPoint ep, float speed);
	static void cbWorked(DTUnit* obj);

	iPoint sp, ep;
	float speed;

	iPoint* tp;
	int tpNum;

	bool havePD;
};

// index : 200 ~ 299
struct DTUnitRepair : DTUnit
{
	DTUnitRepair();
	virtual ~DTUnitRepair();

	virtual bool start(MethodWorked m);
	virtual void paint(float dt, iPoint position);
	static void cbWorked(DTUnit* obj);
};
