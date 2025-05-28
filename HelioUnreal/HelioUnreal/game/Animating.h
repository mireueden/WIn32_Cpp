#pragma once

#include "iStd.h"

void loadAnimating();
void freeAnimating();
void drawAnimating(float dt);
void keyAnimating(iKeyStat stat, iPoint point);

enum Behave
{
	BehaveWait = 0,
	BehaveWalk,
	BehaveRepair,

	BehaveMax// 3
};

void loadAnimatingBG();
void freeAnimatingBG();
void drawAnimatingBg(float dt);

class AIRobot
{
public:
	AIRobot(int index);
	virtual ~AIRobot();

	void paint(float dt);

	static void cbRepair(void* data);

	iRect touchRect();

public:
	int index;
	iImage** imgs;// 0:wait, 1:walk, 2:repair
	iPoint position;
	float speed, rate;

	Behave behave;
};