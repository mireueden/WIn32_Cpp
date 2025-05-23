#pragma once

#include "iStd.h"

void loadLotto();
void freeLotto();
void drawLotto(float dt);
void keyLotto(iKeyStat stat, iPoint point);

// 돈 투입(1000 ~ 5000) 키 : 1 ~ 5

// 추첨 시작
// 추첨 중 : 2 ~ 3초
// 숫자 등장 간격 : 0.2초
// 이번 주 당첨 번호와 비교 표시

// 다음 추첨

// 버튼 : 버튼을 누르면 애니메이션 시작 > 콜백 등록함수


// -----------------------------------------------
// Button
// -----------------------------------------------

struct Lotto
{
	Lotto(iPoint position);
	virtual ~Lotto();

	float delta;
	iPoint position;
	METHOD_VOID method;

	void start(METHOD_VOID cb);
	virtual void paint(float dt) = 0;
};

// -----------------------------------------------
// Button
// -----------------------------------------------

struct Button : Lotto
{
	Button(iPoint position);
	virtual ~Button();
	
	static void cb();

	iSize size;
	bool selected;

	virtual void paint(float dt);// 돈을 투입 > 사라지기
};

// -----------------------------------------------
// Button
// -----------------------------------------------

struct Logic : Lotto
{
	Logic(iPoint position);
	virtual ~Logic();

	static void cb();

	int number[6], rightNum;
	bool right[6];

	virtual void paint(float dt);// 추첨중 > 숫자를 보여줌 > 당첨여부
	void shuffle();
	void match();
};

struct Result : Lotto
{
	Result(iPoint position);
	virtual ~Result();

	static void cb();

	virtual void paint(float dt);// 모든 추첨이 완료되었습니다. > 축하
};