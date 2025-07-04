#pragma once

#include "iStd.h"

// target 생산 목표 개수(수주 개수)
// curr(생산중 + 생산완료)
// complete 생산완료

// 디스플레이 
// 진행률 curr / target
// 생산률 compelte / target
extern int curr, complete, broken;

enum Material
{
	Stain = 0,
	
	Bolt,
	BoltSilver = Bolt,
	BoltBlack, BoltWhite,
	BoltSilverPack, BoltBlackPack, BoltWhitePack,

	Nut,
	NutSilver = Nut,
	NutBlack, NutWhite,
	NUtSilverPack, NUtBlackPack, NUtWhitePack,
};

// DTUnitMake
// 0 : Stain => Bolt 3, Nut 4
// 1 : 블랙 => BoltBlack, NutBlack
// 2 : 화이트 => BoltWhite, NutWhite
// 3 : pack =>	BoltSilverPack, NutSilverPack
//				BoltWhitePack, NutWhitePack
//				BoltBlackPack, NutBlackPack

// BoltSilverPack : 0 => 3
// NutSilverPack : 0 => 3
// 
// BoltBlackPack : 0 => 1 => 3
// NutBlackPack : 0 => 1 => 3
// 
// BoltWhitePack : 0 => 2 => 3
// NutWhitePack : 0 => 2 => 3

struct DTItem
{
	int pd;// 0 ~ 5
	int path[3];
	int pathIndex, pathNum;
	DWORD makeStart, makeEnd;
};

extern DTItem* dtItem;
extern int dtItemNum;
#define dtItemMax 1000

struct DTUnit;
extern DTUnit** unit;
extern int unitNum;

void loadDTObject();
void freeDTObject();
void drawDTObject(float dt, iPoint off);
bool keyDTObject(iKeyStat stat, iPoint point);

// for DTProcUI...
struct OrderInfo
{
	int pd;
	int num, _num;
};

extern OrderInfo* oi;
extern int oiNum;
#define oiMax 100

extern OrderInfo* oiBk;
extern int oiNumBk;

extern iShortestPath* dtsp;

void startMake(int orderPD, int orderNum);

typedef void (*MethodWorked)(DTUnit* obj);

struct DTUnit
{
	DTUnit(int index);
	virtual ~DTUnit();

	virtual bool start(MethodWorked m);
	virtual void paint(float dt, iPoint position) = 0;
	void displaySida(float dt, iPoint position);

	float run(float dt);
	iRect touchRect(iPoint position);

	// 0 ~ 99 : 생산 로봇	
	// 100 ~ 199 : 운반 로봇	
	// 200 ~ 299 : 수리 로봇	
	int index;
	// 현재 사용하고 있는 이미지
	iImage* img;
	// 로봇의 기준좌표(img->position:축좌표, offMap:카메라좌표)
	iPoint position;
	// 수행시간
	float delta, _delta;
	// 수리로봇이 도착지
	iPoint positionSida;
	// 몇 번 실행했는지
	int exe;

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

	// 저장소개수(총 입고개수, 출고개수)
	DTItem** slotIn, ** slotOut;
	int slotInNum, slotOutNum;
	// 재료개수(만드는데 필요한 재료 + 만든 결과 재료)
	int makeInNum, makeOutNum;
	int* makeSlotIn;

	DTUnitMake(int index);
	virtual ~DTUnitMake();

	virtual bool start(MethodWorked m);
	virtual void paint(float dt, iPoint position);
	
	static void cbWorked0(DTUnit* obj);// 볼트,너트
	static void cbWorked1(DTUnit* obj);// 블랙 도색
	static void cbWorked2(DTUnit* obj);// 화이트 도색
	static void cbWorked3(DTUnit* obj);// 포장
};

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
	iPoint tPosition;
	int unitIndex;
	float speed;

	DTItem* have;
};

// index : 200 ~ 299
struct DTUnitSida : DTUnit
{
	iImage** imgs;
	StateMove sm;

	iPoint* path;
	int pathIndex, pathNum;
	float speed;

	int targetUnit;
	DTItem* have;

	DTUnitSida(int index);
	virtual ~DTUnitSida();

	virtual bool start(MethodWorked m);
	virtual void paint(float dt, iPoint position);
	static void cbWorked(DTUnit* obj);
};

extern iShortestPath* dtsp;

// index : 300 ~ 399
struct DTUnitSuccess : DTUnit
{
	DTItem** have;
	int haveNum, _haveNum;

	DTUnitSuccess(int index);
	virtual ~DTUnitSuccess();

	virtual void paint(float dt, iPoint position);
};

// index : 400 ~ 499
struct DTUnitFail : DTUnit
{
	DTUnitFail(int index);
	virtual ~DTUnitFail();

	virtual void paint(float dt, iPoint position);
};

// ===================================
// 로봇 이미지 생성
// ===================================
iImage** createRobot(
	int index, int beNum,
	const char** strBe, int* beAniNum,
	iSize* s, iColor4f* c,
	float ss, iColor4f* sc);

// ===================================
// 로봇 데이터
// ===================================
struct RobotInfo
{
	iSize size;
	iColor4f color;
	float delta;
};

extern RobotInfo ri[8];

extern int itemPath[6][4];

// 저장소 + 재료
extern int slotInOut[4][4];
