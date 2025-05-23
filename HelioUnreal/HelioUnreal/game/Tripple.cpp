#include "Tripple.h"

#if 1

Bot** bot;
iSort* sort;

void loadTripple()
{
	bot = new Bot * [3];
	bot[0] = new BotRed(iRectMake(10, 200, 50, 50),
		iColor4fRed, 100.0f);
	bot[1] = new BotGreen(iRectMake(300, 10, 80, 40),
		iColor4fGreen, 200.0f);
	bot[2] = new BotBlue(iRectMake(200, 200, 30, 30),
		iColor4fBlue, 300.0f, 1, 2);

	sort = new iSort;

}

void freeTripple()
{
	for (int i = 0; i < 3; i++)
		delete bot[i];
	delete bot; 
	delete sort; 
}

#include "iSort.h"

void drawTripple(float dt)
{
	sort->init();

	for (int i = 0; i < 3; i++)
		sort->add(bot[i]->sortY());
	sort->update();

	for (int i = 0; i < 3; i++)
		bot[sort->get(i)]->paint(dt);

}

void keyTripple(iKeyStat stat, iPoint point)
{
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
}




#else 


iRect* _rtObj;
iRect** rtObj;
int numObj;

bool selectedObj;
iPoint prevPoint;

void loadTripple()
{
	_rtObj = new iRect[3];
	rtObj = new iRect*[3];

	for (int i = 0; i < 3; i++)
	{
		_rtObj[i] = iRectMake(50 * i, 50 * i, 50, 50);
		rtObj[i] = &_rtObj[i];
	}
	
	selectedObj = false;
}

void freeTripple()
{
	delete _rtObj;
	delete rtObj;
}

void drawTripple(float dt)
{

	iColor4f color[3] = {
		{1,0,0,1},
		{0,1,0,1},
		{0,0,1,1}
	};

	for (int i = 0; i < 3; i++)
	{
#if 0
		iColor4f* c = &color[i];
#else
		iColor4f* c = NULL;
		for (int j = 0; j < 3; j++)
		{
			if (rtObj[i] == &_rtObj[j])
			{
				c = &color[j];
				break;
			}
		}
#endif
		setRGBA(c->r, c->g, c->b, c->a);
		fillRect(*rtObj[i]);
	}

	if (selectedObj)
	{
		setRGBA(1, 1, 1, 1);
		iRect rt = *rtObj[2];
		rt.origin += iPointMake(5, 5);
		rt.size += iPointMake(-10, -10);
		fillRect(rt);
	}

}


void keyTripple(iKeyStat stat, iPoint point)
{
	switch (stat) {

	case iKeyStatBegan:
		//for (int i = 0; i < 3; i++)
		for (int i = 2; i > -1; i--)
		{
			if (containPoint(point, *rtObj[i]))
			{
				iRect* t = rtObj[i];
				for (int j = i + 1; j < 3; j++)
					rtObj[j - 1] = rtObj[j];
				rtObj[2] = t;

				selectedObj = true;
				prevPoint = point;
				break;
			}
		}
		break;

	case iKeyStatMoved:
		if (selectedObj)
		{
			iPoint mp = point - prevPoint;

			rtObj[2]->origin += mp;

			prevPoint = point;
		}
		break;

	case iKeyStatEnded:
		selectedObj = false;
		break;

	default:
		break;
	}
}

#endif

Bot::Bot(iRect rt, iColor4f color)
{
	this->rt = rt;
	this->color = color;
	energy = 0;
}

Bot:: ~Bot()
{

}

void Bot::paint(float dt)
{
	iColor4f* c = &color;
	setRGBA(c->r, c->g, c->b, c->a);
	fillRect(rt);
}


// =======================
// BotRed
// =======================

BotRed::BotRed(iRect rt, iColor4f color, float speed) : Bot(rt, color)
{
	this->speed = speed;
	left = true;
}

BotRed::~BotRed()
{

}

void BotRed::paint(float dt)// 좌우
{
	if (left)
	{
		rt.origin.x -= speed * dt;
		if (rt.origin.x < 0)
		{
			rt.origin.x = 0;
			left = false;
		}
	}
	else// if (left == false)
	{
		rt.origin.x += speed * dt;
		if (rt.origin.x + rt.size.width > DEV_WIDTH)
		{
			rt.origin.x = DEV_WIDTH - rt.size.width;
			left = true;
		}
	}
	Bot::paint(dt);
}

// =======================
// BotGreen
// =======================

BotGreen::BotGreen(iRect rt, iColor4f color, float speed) : Bot(rt,color)
{
	this->speed = speed;
	up = true;
}

BotGreen::~BotGreen()
{

}

void BotGreen::paint(float dt)
{
	if (up)
	{
		rt.origin.y -= speed * dt;
		if (rt.origin.y < 0)
		{
			rt.origin.y = 0;
			up = false;
		}
	}
	else// if (up == false)
	{
		rt.origin.y += speed * dt;
		if (rt.origin.y + rt.size.height > DEV_HEIGHT)
		{
			rt.origin.y = DEV_HEIGHT - rt.size.height;
			up = true;
		}
	}
	Bot::paint(dt);
}

// =======================
// BotBlue
// =======================

BotBlue::BotBlue(iRect rt, iColor4f color, float speed, float aiFrom, float aiTo) : Bot(rt, color)
{
	this->speed = speed;
	this->aiFrom = aiFrom;
	this->aiTo = aiTo;
	_ai = 0.0f;
	ai = 0.0f;
	tp = rt.origin;
}

BotBlue::~BotBlue()
{
	// to do
}

void BotBlue::cb(BotBlue* bb)
{
	bb->tp = iPointMake(rand() % DEV_WIDTH, rand() % DEV_HEIGHT);
	bb->_ai = linear(bb->aiFrom, bb->aiTo, rand() % 100 / 100.f);
	bb->ai = 0.0f;
	
}


void BotBlue::paint(float dt)
{
	if (rt.origin != tp)
	{
		iPoint mp = tp - rt.origin;
		mp.loadIdentity();
		move(&rt.origin, &tp, mp * (speed * dt));
	}
	else if (rt.origin == tp)
	{
		ai += dt;
		if (ai >= _ai)
		{
			cb(this); // 임시로 콜백 불러 처리
		}
	}
	Bot::paint(dt);
}