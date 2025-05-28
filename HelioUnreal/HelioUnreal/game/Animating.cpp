#include "Animating.h"

static iSort* sort;
AIRobot** ai;
int selectedAI;

void loadAnimating()
{
	loadAnimatingBG();

	sort = new iSort(5);

	ai = new AIRobot * [5];
	for (int i = 0; i < 5; i++)
	{
		ai[i] = new AIRobot(i);
		ai[i]->position = iPointMake(100 + 50 * i, 100 + 50 * i);
	}
	selectedAI = -1;
}

void freeAnimating()
{
	freeAnimatingBG();

	delete sort;

	for (int i = 0; i < 5; i++)
		delete ai[i];
	delete ai;
}

void drawAnimating(float dt)
{
	drawAnimatingBg(dt);

	// draw
	sort->init();
	for (int i = 0; i < 5; i++)
		sort->add(ai[i]->position.y);
	sort->update();

	for (int i = 0; i < 5; i++)
	{
		int j = sort->get(i);
		AIRobot* a = ai[j];
		a->rate = linear(1, 2, a->position.y / devSize.height);
		a->paint(dt);
	}

	// ctrl
	if (selectedAI == -1)
		return;
	AIRobot* ai = ::ai[selectedAI];

	setRGBA(1, 1, 1, 1);
	drawRect(ai->touchRect());

	if (ai->behave != BehaveRepair)
	{
		iPoint v = iPointZero;
		if (keystat & keydown_a)	  v.x = -1;
		else if (keystat & keydown_d) v.x = +1;
		if (keystat & keydown_w)	  v.y = -1;
		else if (keystat & keydown_s) v.y = +1;

		Behave be;
		if (v == iPointZero)
			be = BehaveWait;
		else
		{
			be = BehaveWalk;
			v.loadIdentity();
		}

		if (keydown & keydown_space)
			be = BehaveRepair;
		ai->behave = be;
		ai->position += v * (ai->speed * dt);

		if (be == BehaveRepair)
		{
			ai->imgs[be]->startAnimation(AIRobot::cbRepair, ai);
		}
	}

}

void keyAnimating(iKeyStat stat, iPoint point)
{
	int i, j = -1;

	switch (stat) {

	case iKeyStatBegan:
		for (i = 0; i < 5; i++)
		{
			if (containPoint(point, ai[i]->touchRect()))
			{
				j = i;
				break;
			}
		}
		selectedAI = j;
		break;

	case iKeyStatMoved:
		break;

	case iKeyStatEnded:
		break;

	default:
		break;
	}
}

int tileX, tileY;
int* tile;// 1:이동가능 9:이동불가
void loadAnimatingBG()
{
	// 640 x 480
	// 10 x 8(7.5)
	tileX = 10, tileY = 8;
	tile = new int[tileX * tileY];
	int t[10 * 8] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 9, 1, 1, 1, 1, 1,	1, 1,
		1, 1, 9, 1, 1, 1, 1, 1,	1, 1,
		1, 1, 1, 9, 1, 1, 1, 1,	1, 1,
		1, 1, 1, 1, 9, 1, 1, 1,	1, 1,
		1, 1, 1, 1, 9, 1, 1, 1,	1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,	1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,	1, 1,
	};
	memcpy(tile, t, sizeof(int) * tileX * tileY);
}
void freeAnimatingBG()
{
	delete tile;
}
void drawAnimatingBg(float dt)
{
	iColor4f color[10] = {
		{},
		{1, 1, 1, 0.5f},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{1, 1, 0, 0.5f},
	};

	for (int j = 0; j < tileY; j++)
	{
		for (int i = 0; i < tileX; i++)
		{
			int t = tile[tileX * j + i];
			iColor4f& c = color[t];
			setRGBA(c.r, c.g, c.b, c.a);
			fillRect(64 * i, 64 * j, 64, 64);

			setRGBA(1, 1, 1, 1);
			drawRect(64 * i + 2, 64 * j + 2, 60, 60);
		}
	}
}



AIRobot::AIRobot(int index)
{
	this->index = index;
	struct RobotInfo
	{
		int width, height;
		iColor4f color;
		int frame;
	};
	RobotInfo robotInfo[3] = {
		{30,80, {0, 0, 1, 1}, 3},
		{20,100, {1, 0, 0, 1}, 4},
		{50,50, {0, 1, 0, 1}, 20},
	};
	iGraphics* g = iGraphics::share();

	imgs = new iImage * [3];
	for (int i = 0; i < 3; i++)
	{
		iImage* img = new iImage();

		RobotInfo* ri = &robotInfo[i];
		for (int j = 0; j < ri->frame; j++)
		{
			g->init(ri->width, ri->height);

			iColor4f* c = &ri->color;
			setRGBA(c->r, c->g, c->b, c->a);
			g->fillRect(0, 0, ri->width, ri->height);

			setStringSize(10);
			setStringRGBA(0, 0, 0, 1);
			g->drawString(0, 0, "%d", j);

			Texture* tex = g->getTexture();
			img->add(tex);
			freeImage(tex);
		}

		img->position = iPointMake(-ri->width / 2, -ri->height);
		if (i < 2)
			img->startAnimation();
		imgs[i] = img;
	}

	position = iPointMake(200, 300);
	speed = 300;

	behave = BehaveWait;
}

AIRobot::~AIRobot()
{
	for (int i = 0; i < 3; i++)
		delete imgs[i];
	delete imgs;
}

void AIRobot::paint(float dt)
{
	imgs[behave]->rate = rate;
	imgs[behave]->paint(dt, position);

	setRGBA(1, 1, 1, 1);
	fillRect(position.x - 30, position.y - 1, 60, 2);
	fillRect(position.x - 1, position.y - 30, 2, 60);
}

void AIRobot::cbRepair(void* data)
{
	//int i = -1;
	//for (; i < 5; i++)
	//{
	//	if (ai[i] == data)
	//		break;
	//}

	AIRobot* ai = (AIRobot*)data;
	printf("어느놈[%d]수리가 끝났네? \n", ai->index);
	ai->behave = BehaveWait;
}

iRect AIRobot::touchRect()
{
	return imgs[behave]->touchRect(position);
}