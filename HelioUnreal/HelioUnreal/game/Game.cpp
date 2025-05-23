#include "Game.h"

#include <math.h>
#include <stdio.h>
iRect me;
iPoint tp;
float takeTime;

#include "Lotto.h"
#include "Tripple.h"
#include "AirShooting.h"
#include "Memory.h"
#include "PS.h"

ParticleSystem* ps;

void loadGame()
{
#if 1
	struct Score
	{
		char name[32];
		int kor, eng, math;
	};

	Score score[3] = {
		{"조성목", 100, 100, 100},
		{"임수아", 100, 99, 98},
		{"김성민", 0, 100, 100},
	};


	//saveFile((char*)score, sizeof(score), "test.sav");
	//int len = sizeof(score);
	//char* t = new char[len];
	//for (int i = 0; i < len; i++)
	//	t[i] = ((char*)score)[i] + 128;
	//saveFile(t, len, "test.sav");

	// --
	//Score* s = (Score*)loadFile("test.sav");
	// --

	//int len;
	//char* t = loadFile(len, "test.sav");
	//for (int i = 0; i < len; i++)
	//	t[i] -= 128;
	//Score* s = (Score*)t;

	//for (int i = 0; i < 3; i++)
	//	printf("[%s] %d %d %d\n",
	//		s[i].name, s[i].kor, s[i].eng, s[i].math);


#else

	//char* str = loadFile("HelioUnreal.h");
	//printf("HelioUnreal.h\n%s\n", str);
	//delete str;


#pragma pack(push, 1)
	struct Score
	{
		char kor;// 1
		//char d0[3];
		int eng;// 4
		short math;// 2
		//char d1[2];
	};

	Score s;
	s.kor; 
	s.eng;

#pragma pack(pop)

	printf("%d\n", sizeof(Score));
#endif
	me = iRectMake(0, 0, 50, 50);
	takeTime = 0.0f;

#if 0
	loadLotto();
#elif 0 
	loadTripple();
#elif 0 
	loadAirShooting();
#else
	loadMemory();
#endif

	ps = new ParticleSystem();
	ps->save("test.ptc");
	//ps = new ParticleSystem("test.ptc");
}

void freeGame()
{
#if 0
	freeLotto();
#elif 0
	freeTripple();
#elif 0
	freeAirShooting();
#else
	freeMemory();
#endif
	delete ps;
}

void drawGame(float dt)
{
	setRGBA(0, 0, 0, 1);
	clear();

#if 0
	drawLotto(dt);
	return;
#elif 0
	drawTripple(dt);
	return;
#elif 0
	drawAirShooting(dt);
	return;
#elif 0
	drawMemory(dt);
	return;
#else
	ps->paint(dt, iPointMake(DEV_WIDTH / 2, DEV_HEIGHT / 2));
	return;
#endif
	drawString(300, 100, L"Hi");

	takeTime += dt;

	//float dx = sin(takeTime) * 50;

	setRGBA(1, 0, 0, 1);
	//drawLine(50 + dx, 0, 150 + dx, 100);
	fillRect(me);

#define move_speed 300
#if 0
	extern int keydown;
	iPoint v = iPointZero;
	if (keydown & keydown_a)
		v.x = -1;
	else if (keydown & keydown_d)
		v.x = 1;

	if (keydown & keydown_w)
		v.y = -1;
	else if (keydown & keydown_w)
		v.y = 1;

	//if (v != iPointZero)
	//	v /= iPointLength(v);

	v.loadIdentity();


	me.origin += v * move_speed * dt;

#else
	if (me.origin != tp)
	{
		iPoint v = tp - me.origin;
		v.loadIdentity();

		//me.origin += v * move_speed * dt;
		iPoint mp = v * move_speed * dt;
		if (me.origin.x < tp.x)
		{
			me.origin.x += mp.x;
			if (me.origin.x > tp.x)
				me.origin.x = tp.x;
		}
		else if (me.origin.x > tp.x)
		{
			me.origin.x += mp.x;
			if (me.origin.x < tp.x)
				me.origin.x = tp.x;
		}

		if (me.origin.y < tp.y)
		{
			me.origin.y += mp.y;
			if (me.origin.y > tp.y)
				me.origin.y = tp.y;
		}
		else if (me.origin.y > tp.y)
		{
			me.origin.y += mp.y;
			if (me.origin.y < tp.y)
				me.origin.y = tp.y;
		}
	}

#endif


}



void KeyGame(iKeyStat stat, iPoint point)
{
#if 0
	keyLotto(stat, point);
	return;
#elif 0
	keyTripple(stat, point);
#elif 0
	keyAirShooting(stat, point);
#else
	keyMemory(stat, point);
#endif

	if (stat == iKeyStatBegan)
	{
		tp = point;
		
	}
	else if (stat == iKeyStatMoved)
	{

	}
	else if (stat == iKeyStatEnded)
	{

	}
}
