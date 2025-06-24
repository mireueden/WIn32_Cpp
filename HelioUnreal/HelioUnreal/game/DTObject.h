#pragma once

#include "iStd.h"

void loadDTObject();
void freeDTObject();
void drawDTObject(float dt, iPoint off);
bool keyDTObject(iKeyStat stat, iPoint point);

struct DTUnit;
typedef void (*MethodWorked)(DTUnit* obj);

struct DTUnit
{
	DTUnit(int index);
	virtual ~DTUnit();

	virtual void start(MethodWorked m) = 0;
	virtual void paint(float dt, iPoint position) = 0;

	iRect touchRect(iPoint position);

	int index;
	iImage* img;
	iPoint position;
	float delta, _delta;
};


struct DTUnitMake : DTUnit
{
	DTUnitMake(int index);
	virtual ~DTUnitMake();

	virtual void start(MethodWorked m);
	virtual void paint(float dt, iPoint position);
};

struct DTUnitMove : DTUnit
{
	DTUnitMove();
	virtual ~DTUnitMove();

	virtual void start(MethodWorked m);
	virtual void paint(float dt, iPoint position);
};

struct DTUnitRepair : DTUnit
{
	DTUnitRepair();
	virtual ~DTUnitRepair();

	virtual void start(MethodWorked m);
	virtual void paint(float dt, iPoint position);
};