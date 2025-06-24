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

// result:-1 네트워크상태
// result:-2 pw
// result:0 성공,
// result:1퇴사
// result:2동접자
typedef void (*cbDtLogin)(int result);
void dtLogin(cbDtLogin func, const char* name, const char* pw, int n);

#define DISPLAY_THREAD 0