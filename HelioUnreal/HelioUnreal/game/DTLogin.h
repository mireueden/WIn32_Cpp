#pragma once

#include "iStd.h"

void loadDTLogin();
void freeDTLogin();
void drawDTLogin(float dt);
void keyDTLogin(iKeyStat stat, iPoint point);

// ID/ PW

struct LoginBar
{
	LoginBar();
	virtual ~LoginBar();

	bool bShow;
	float delta, _delta;
	void show(bool show);
	void paint(float dt, iPoint position);
};