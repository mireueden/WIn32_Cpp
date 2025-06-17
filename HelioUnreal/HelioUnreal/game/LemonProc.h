#pragma once

#include "iStd.h"

struct Lemon
{
	int num;
	bool exist;
	bool selected;
};

extern Lemon* lemon;// 17 x 10

#define LEMON_SX 65
#define LEMON_SY 100

#define LEMON_W 30
#define LEMON_H 30

// =====================================
// Proc(17 x 10 타일에 1~9숫자로 구성)
// =====================================
void loadLemonProc();
void freeLemonProc();
void drawLemonProc(float dt);
void keyLemonProc(iKeyStat stat, iPoint point);

// =====================================
// ProcUI(위-점수,남은시간, 아래-현재점수)
// =====================================
struct Number;
extern Number* score;
extern float takeTime;

void loadLemonProcUI();
void freeLemonProcUI();
void drawLemonProcUI(float dt);
//void keyLemonProcUI(iKeyStat stat, iPoint point);

// 순차적으로 숫자의 증감을 표현하기 위해?(오토넘버)
struct Number
{
	Number();
	virtual ~Number();

	float delta, _delta;
	int curr; // 100
	int next; // 200

	void update(float dt);
	int get();
	void add(int n);
	
	Number& operator+= (int n)
	{
		add(n);
		return* this;
	}
};

// =====================================
// Setting
// =====================================
void loadLemonSetting();
void freeLemonSetting();
void drawLemonSetting(float dt);
bool keyLemonSetting(iKeyStat stat, iPoint point);

// =====================================
// CountDown
// =====================================
extern int numCountDown;

void loadLemonCountDown();
void freeLemonCountDown();
void drawLemonCountDown(float dt);
bool keyLemonCountDown(iKeyStat stat, iPoint point);
