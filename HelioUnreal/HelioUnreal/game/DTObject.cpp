#include "DTObject.h"

#include "DTProcPop.h"

DTUnit** unit;
int selectedUnit;
iPoint positionUnit;

void loadDTObject()
{
	unit = new DTUnit * [5];
	for (int i = 0; i < 5; i++)
	{
		unit[i] = new DTUnitMake(i);
		unit[i]->position = iPointMake(80 + 140 * i, 140);
	}
	selectedUnit = -1;
	positionUnit = iPointZero;
}

void freeDTObject()
{
	for (int i = 0; i < 5; i++)
		delete unit[i];
	delete unit;
}

static iPoint off;
void drawDTObject(float dt, iPoint off)
{
	::off = off;

	for (int i = 0; i < 5; i++)
		unit[i]->paint(dt, off);

	if (selectedUnit == -1)
		showToast(positionUnit, NULL);
	else
		showToast(positionUnit, "unit[%d] 선택됨\n", selectedUnit);
}

bool keyDTObject(iKeyStat stat, iPoint point)
{
	int i, j = -1;

	switch (stat)
	{
	case iKeyStatBegan:
		break;

	case iKeyStatMoved:
		for (int i = 0; i < 5; i++)
		{
			if (containPoint(point, unit[i]->touchRect(off)))
			{
				j = i;
				break;
			}
		}
		selectedUnit = j;
		positionUnit = point;
		break;

	case iKeyStatEnded:
		break;

	}

	return false;
}

// ========================================
// DTUnit
// ========================================


DTUnit::DTUnit(int index)
{
	this->index = index;
	img = NULL;
	position = iPointZero;
	_delta = 0.0f;
	delta = 0.0f;
}
DTUnit::~DTUnit()
{
	delete img;
}

iRect DTUnit::touchRect(iPoint position)
{
	return img->touchRect(this->position + position);
}

// ========================================
// DTUnitMake
// ========================================

// 640 x 480
// w : 80 ~ 120, h : 40 ~ 80

DTUnitMake::DTUnitMake(int index) : DTUnit(index)
{
	iSize sizes[] = {
		{ 80, 40 },
		{ 120, 40 },
		{ 80, 160 },
		{ 120, 120 },
		{ 80, 80 },
	};

	iColor4f colors[] = {
		{1, 0, 0, 1},
		{0, 1, 0, 1},
		{0, 0, 1, 1},
		{1, 1, 0, 1},
		{1, 0, 1, 1},
		{1, 1, 1, 1},
	};

	iGraphics* g = iGraphics::share();

	iSize* s = &sizes[index];
	g->init(s->width, s->height);

	iColor4f* c = &colors[index];
	setRGBA(c->r, c->g, c->b, c->a);
	g->fillRect(0, 0, s->width, s->height);
	setRGBA(1, 1, 1, 1);

	setStringSize(30);
	setStringRGBA(0, 0, 0, 1);
	g->drawString(s->width / 2, s->height / 2, VCENTER | HCENTER, "%d", index);

	Texture* tex = g->getTexture();
	g->clean();
	img = new iImage();
	img->add(tex);
	freeImage(tex);
	img->position = iPointMake(-s->width / 2, -s->height);
}

DTUnitMake::~DTUnitMake()
{

}

void DTUnitMake::start(MethodWorked m)
{

}

void DTUnitMake::paint(float dt, iPoint position)
{
	//setRGBA(1, 0, 1, 1);
	//iPoint p = this->position + position;
	//fillRect(p.x, p.y, 50, 50);
	//setRGBA(1, 1, 1, 1);
	img->paint(dt, this->position + position);
}