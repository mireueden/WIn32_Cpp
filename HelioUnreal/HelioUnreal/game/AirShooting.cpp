#include "AirShooting.h"



void loadAirShooting()
{
	loadUs();
	loadYou();
	loadBall();

	loadEffect();
	loadUI();
}

void freeAirShooting()
{
	freeUs();
	freeYou();
	freeBall();

	freeEffect();
	freeUI();
}

void drawAirShooting(float dt)
{
	drawUs(dt);
	drawYou(dt);
	drawBall(dt);

	drawEffect(dt);
	drawUI(dt);
}

void keyAirShooting(iKeyStat stat, iPoint point)
{
	if (keyUI(stat, point))
		return;

	// 인게임 컨틀롤
	switch (stat)
	{
	case iKeyStatBegan:
		break;

	case iKeyStatMoved:
		break;

	case iKeyStatEnded:
		break;

	default:
		break;
	}
}

// ====================================
// Us
// ====================================
Us* us;

void loadUs()
{
	us = new Us; // new Us()
	us->position = iPointMake(DEV_WIDTH / 2, DEV_HEIGHT * 2 / 3);
	us->hp = 3;
	us->ap = 1;
	us->moveSpeed = 600;
	us->_rate = 0.2f;
	us->rate = 0.0f;

}
void freeUs()
{
	delete us;
}
void drawUs(float dt)
{
	// paint
	setRGBA(1, 0, 0, 1);
	iRect rt;
	rt.origin = us->position + iPointMake(-15, -15);
	rt.size = iSizeMake(30, 30);
	fillRect(rt);

	// ctrl
	iPoint v = iPointZero;
	if (keydown & keydown_a)		v.x = -1;
	else if (keydown & keydown_d)	v.x = +1;
	if (keydown & keydown_w)		v.y = -1;
	else if (keydown & keydown_s)	v.y = +1;
	v.loadIdentity();
	us->position += v * (us->moveSpeed * dt);

#define keydown_one keydown_space
#define keydown_two keydown_space
	int key = -1;
	if (keydown & keydown_space)		key = 0;
	else if (keydown & keydown_one)		key = 1;
	else if (keydown & keydown_two)		key = 2;
	
	


	if (key != -1)
	{
		us->rate += dt;
		if (us->rate >= us->_rate)
		{
			printf("fire\n");
#if 0
			Ball* b = new Ball;
#else
			Ball* b = NULL;
			for (int i = 0; i < ballMax; i++)
			{
				Ball* t = &_ball[i];
				if (t->own == NULL)
				{
					b = t;
					break;
				}
			}
#endif
			b->own = us;
			b->position = us->position + iPointMake(0, -15);
			b->v = iPointMake(0, -1);
			b->moveSpeed = 400;

			MethodBall m[3] = { paintBallDirect, paintBallFollow, paintBallBomb };
			b->method = m[key];

			ball[ballNum] = b;
			ballNum++;
			printf("추가 ballNum = %d\n", ballNum);
		}
	}
}

// ====================================
// You
// ====================================
You* _you;
You** you;
int youNum;

float createYouDt, _createYouDt;

void loadYou()
{
	_you = new You[youMax];
	memset(_you, 0x00, sizeof(You) * youMax);
	you = new You * [youMax];
	youNum = 0;

	_createYouDt = (200 + rand() % 100) / 100.0f;
	createYouDt = 0.0f;
}
void freeYou()
{
	delete _you;
	delete you;
}
void drawYou(float dt)
{
	iRect rt;
	setRGBA(1, 1, 1, 1);
	for (int i = 0; i < youNum; i++)
	{
		You* y = you[i];
		// paint
		rt.origin = y->position + iPointMake(-10, -10);
		rt.size = iSizeMake(20, 20);
		fillRect(rt);
		// ctrl == ai
		y->position += y->v * (y->moveSpeed * dt);

		// || me랑 충돌이 일어났으며...
		if (!containPoint(y->position,
			iRectMake(-10, -10 - 90, DEV_WIDTH + 20,
				DEV_HEIGHT + 20 + 90)))
		{
			y->hp = 0;

			youNum--;
			for (int j = i; j < youNum; j++)
				you[j] = you[j + 1];
			printf("삭제 youNum = %d\n", youNum);
		}
	}

	// rende 2 ~ 3초에 한번씩 You 생성
	createYouDt += dt;
	if (createYouDt >= _createYouDt)
	{
		createYouDt -= _createYouDt;
		_createYouDt = (100 + rand() % 100) / 100.0f;

		You* y = NULL;
		for (int i = 0; i < youMax; i++)
		{
			You* t = &_you[i];
			if (t->hp == 0)
			{
				y = t;
				break;
			}
		}

		y->hp = 3;
		y->position = iPointMake(50 + rand() % (DEV_WIDTH - 100), -50);
		y->v = iPointMake(0, 1);
		y->moveSpeed = 100 + rand() % 100;

		you[youNum] = y;
		youNum++;
		printf("추가 youNum - %d\n", youNum);
	}


}


// ====================================
// Ball
// ====================================
Ball* _ball;
Ball** ball;
int ballNum;
int* bn;

void loadBall()
{
	_ball = new Ball[ballMax];
	memset(_ball, 0x00, sizeof(Ball) * ballMax);

	ball = new Ball * [ballMax];
	ballNum = 0;
}
void freeBall()
{
	delete _ball;
	delete ball;
}
void drawBall(float dt)
{
	iRect rt;
	setRGBA(0, 0, 1, 1);
	for (int i = 0; i < ballNum; i++)
	{
		Ball* b = ball[i];
		// paint
		rt.origin = b->position + iPointMake(-5, -5);
		rt.size = iSizeMake(10, 10);
		fillRect(rt);
		// ctrl == ai
		b->position += b->v * (b->moveSpeed * dt);

		// || 적이랑 충돌이 일어났으며...
		if (!containPoint(b->position,
			iRectMake(-10, -10, DEV_WIDTH + 20, DEV_HEIGHT + 20)))
		{
#if 0
			delete b;
#else
			b->own = NULL;
#endif			
			ballNum--;
			for (int j = i; j < ballNum; j++)
				ball[j] = ball[j + 1];
			printf("삭제 ballNum = %d\n", ballNum);
		}
	}
}



bool paintBallDirect(Ball* b, float dt)
{
	b->position += b->v * (b->moveSpeed * dt);
	return false;
}

// 충돌후 데미지 paintBallDirect와 동일, 날아가는 궤도
bool paintBallFollow(Ball* ball, float dt)
{
	BallFollow* b = (BallFollow*)ball;
	//b->position += b->v * (b->moveSpeed * dt);

	if (b->findDt < b->_findDt)
	{
		//b->position += b->v * (b->moveSpeed * dt);
		iPoint v = b->tp - b->position;
		v.loadIdentity();

		b->position += v * (b->moveSpeed * dt);
		b->v = v;// backup

		b->findDt += dt;

		if (iPointLength(b->position - b->tp) < 5)
			b->findDt = b->_findDt;
	}
	else
	{
		// 등장한 You중에서 가장 가까운 you
		You* y = NULL;
		float dis = 0xffffff;
		for (int i = 0; i < youNum; i++)
		{
			float d = iPointLength(b->position - you[i]->position);
			if (d < dis)
			{
				dis = d;
				y = you[i];
			}
		}
		if (y)
		{
			b->tp = y->position;
			b->findDt = 0.0f;
		}
		else
		{
			b->tp = b->position + b->tp * 1000;
			b->findDt = -20.0f;
		}
	}

	return false;
}
// 날아가는 paintBallDirect와 동일, 충돌후 데미지 주변 영향
bool paintBallBomb(Ball* b, float dt)
{
	b->position += b->v * (b->moveSpeed * dt);

	return false;
}

void checkCollision()
{
	// collision
	// us + you
	iRect rtDst = us->rt();
	for (int i = 0; i < youNum; i++)
	{
		You* y = you[i];
		if (containRect(y->rt(), rtDst))
		{
			y->hp--; if (y->hp == 0);
			{
				youNum--;
				you[i] = you[youNum];
				i--;
			}
			us->hp--; if (us->hp == 0);
		}
	}
	// us + you + ball

}

void loadEffect()
{
}

void freeEffect()
{
}

void drawEffect(float dt)
{
}

void keyEffect(iKeyStat stat, iPoint point)
{
}

void loadUI()
{
}

void freeUI()
{
}

void drawUI(float dt)
{
}

bool keyUI(iKeyStat stat, iPoint point)
{
	// if (UI영역이 아니면)
	// return false;

	// 인게임 컨트롤

	switch (stat) {

	case iKeyStatBegan:
		break;

	case iKeyStatMoved:
		break;

	case iKeyStatEnded:
		break;

	default:
		break;
	}
	return true;
}
