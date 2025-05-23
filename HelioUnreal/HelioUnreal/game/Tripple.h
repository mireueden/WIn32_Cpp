#pragma once

#include "iStd.h"

void loadTripple();
void freeTripple();
void drawTripple(float dt);
void keyTripple(iKeyStat stat, iPoint point);

#define DEV_WIDTH 640
#define DEV_HEIGHT 480

// --------------------
// Bot
// --------------------

struct Bot 
{
	iRect rt;
	iColor4f color;
	int energy;

	Bot(iRect rt, iColor4f color);
	virtual ~Bot();
	virtual void paint(float dt);

	float sortY()
	{
		return rt.origin.y + rt.size.height;
	}
};

// =======================
// BotRed
// =======================

struct BotRed : Bot
{
	float speed;
	bool left;

	BotRed(iRect rt, iColor4f color, float speed);
	virtual ~BotRed();
	virtual void paint(float dt);// 좌우
};

// =======================
// BotGreen
// =======================


struct BotGreen : Bot
{
	float speed;
	bool up;

	BotGreen(iRect rt, iColor4f color, float speed);
	virtual ~BotGreen();
	virtual void paint(float dt); // 세로
};

// =======================
// BotBlue
// =======================

struct BotBlue : Bot
{
	float speed, _ai, ai, aiFrom, aiTo;
	iPoint tp;

	static void cb(BotBlue* bb);

	BotBlue(iRect rt, iColor4f color, float speed, float aiFrom, float aiTo);
	virtual ~BotBlue();
	virtual void paint(float dt); // 자율
};