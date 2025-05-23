#pragma once

#include "iStd.h"

void loadAirShooting();
void freeAirShooting();
void drawAirShooting(float dt);
void keyAirShooting(iKeyStat stat, iPoint point);

// ====================================
// Us
// ====================================
struct Us
{
	// 총알을 가진다.
	iPoint position;
	int hp, ap;
	float moveSpeed, rate, _rate;

	iRect rt()
	{
		return iRectMake(position.x - 15, position.y - 15, 30, 30);
	}
};

extern Us* us;

void loadUs();
void freeUs();
void drawUs(float dt);

// ====================================
// You
// ====================================
struct You
{
	int hp;
	iPoint position, v;
	float moveSpeed;


	iRect rt()
	{
		return iRectMake(position.x - 10, position.y - 10, 20, 20);
	}
};

extern You* _you;
extern You** you;
extern int youNum;
#define youMax 50

extern float createYouDt, _createYouDt;

void loadYou();
void freeYou();
void drawYou(float dt);

// ====================================
// Ball
// ====================================
struct _Ball;
typedef bool (*MethodBall)(_Ball* ball, float dt);

typedef struct _Ball
{
	void* own;
	iPoint position, v;
	float moveSpeed;

	MethodBall method;
	// diff
	float dummy[8];
} Ball, BallDirect;

struct BallFollow
{
	void* own;
	iPoint position, v;
	float moveSpeed;

	MethodBall method;
	// diff
	iPoint tp;
	float findDt, _findDt;
};

struct BallBomb
{
	void* own;
	iPoint position, v;
	float moveSpeed;

	MethodBall method;
	// diff
	float range;
};

extern Ball* _ball;
extern Ball** ball;
extern int ballNum;
#define ballMax 120 // 100 + 10 + 5
extern int* bn;

void loadBall();
void freeBall();
void drawBall(float dt);

bool paintBallDirect(Ball* b, float dt);
bool paintBallFollow(Ball* b, float dt);
bool paintBallBomb(Ball* b, float dt);

// ====================================
// Collision
// ====================================
void checkCollision();

// ====================================
// Effect
// ====================================
// 피격, 잔상, 경고
enum Effect
{
	EffectDmg = 0,
	EffectLine,
	EffectWarn,
};

void loadEffect();
void freeEffect();
void drawEffect(float dt);
void keyEffect(iKeyStat stat, iPoint point);

// ====================================
// UI
// ====================================

void loadUI();
void freeUI();
void drawUI(float dt);
bool keyUI(iKeyStat stat, iPoint point);