#include "DTObject.h"

#include "DTProcPop.h"

DTUnit** unit;
int selectedUnit;
iPoint positionUnit;

int target, curr, complete, broken;

void startMake(int target)
{
	// 주문 우선순위는 나중에 처리
	::target = target;
	curr = 0;
	complete = 0;
	broken = 0;
}

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

	target = 0;
	curr = 0;
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

	if (curr < target)
	{
		if (unit[0]->start(DTUnitMake::cbWorked0))
			curr++;
	}
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
	if(img)
		delete img;
}

bool DTUnit::start(MethodWorked m)
{
	if (delta < _delta)
		return false;

	delta = 0.0f;
	methodWorked = m;
	return true;
}

float DTUnit::run(float dt)
{
	if (delta < _delta) 
	{
		delta += dt;
		float r = delta / _delta; // 진행 비율

		if (delta >= _delta)
		{
			methodWorked(this);
		}
		if (r > 1.0f)
		{
			r = 1.0f;
		}
		return r;
	}
	return 1.0f;
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
	};

	const char* str[4] = { "대기","생산","고장","완료" };
	int num[4] = { 3,10,2,5 };
	imgs = new iImage * [StateMakeMax];

	iColor4f* c = &colors[index];
	setRGBA(c->r, c->g, c->b, c->a);

	setStringSize(18);
	setStringRGBA(0, 0, 0, 1);

	iGraphics* g = iGraphics::share();
	iSize* s = &sizes[index];
	for (int i = 0; i < 4; i++)
	{
		img = new iImage();

		for (int j = 0; j < num[i]; j++)
		{
			g->init(s->width, s->height);
			
			g->fillRect(0, 0, s->width, s->height);
			g->drawString(s->width / 2, s->height / 2 - 10, VCENTER | HCENTER,
				"index%d", index);
			g->drawString(s->width / 2, s->height / 2 + 10, VCENTER | HCENTER,
				"%s%d", str[i], j);

			Texture* tex = g->getTexture();
			g->clean();
			img->add(tex);
		}
		img->position = iPointMake(-s->width / 2, -s->height);
		img->_aniDt = 0.17f;
		img->startAnimation();
		imgs[i] = img;
	}
	sm = StateMakeReady;
}

DTUnitMake::~DTUnitMake()
{
	for (int i = 0; i < StateMakeMax; i++)
		delete imgs[i];
	delete imgs;
	img = NULL;
}

bool DTUnitMake::start(MethodWorked m)
{
	bool result = DTUnit::start(m);

	// to do something.. 

	return result;
}

void DTUnitMake::paint(float dt, iPoint position)
{
	img = imgs[sm];
	//setRGBA(1, 0, 1, 1);
	//iPoint p = this->position + position;
	//fillRect(p.x, p.y, 50, 50);
	//setRGBA(1, 1, 1, 1);
	img->paint(dt, this->position + position);

	// ctrl
	// 물건을 싣는중
	float r = run(dt);
	// r에 따라 싣는 중인 프로그래스바 애니메이션 재생

	if (r < 1.0f)
		return;

	// 다음 생산을 진행
	// 대기 슬롯이 가득찼을때(생산 준비가 되었을때)
	MethodWorked m[6] = {
		cbWorked0, cbWorked1, cbWorked2,
		cbWorked3, cbWorked4, cbWorked5,
	};

	start(m[index]);
}

void DTUnitMake::cbWorked0(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
}

void DTUnitMake::cbWorked1(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
}

void DTUnitMake::cbWorked2(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
}

void DTUnitMake::cbWorked3(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
}

void DTUnitMake::cbWorked4(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
}

void DTUnitMake::cbWorked5(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
}

void DTUnitMake::cbWorked9(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
	complete++;
}

// =============================================
// DTUnitMove
// =============================================

DTUnitMove::DTUnitMove(int index) : DTUnit(index)
{
	index -= 100; // 0 ~ 99
	// index == 1 : 가로로 움직이는 로봇
	// index == 2 : 세로로 움직이는 로봇

	iGraphics* g = iGraphics::share();

	iSize s = iSizeMake(50, 50);
	g->init(s.width, s.height);

	setRGBA(0, 0, 0, 1);
	g->fillRect(0, 0, s.width, s.height);
	setRGBA(1, 1, 0, 1);
	setLineWidth(2);
	g->drawRect(5, 5, s.width - 10, s.height - 10);

	Texture* tex = g->getTexture();
	g->clean();
	img = new iImage();
	img->add(tex);
	freeImage(tex);
	img->position = iPointMake(-s.width / 2, -s.height);

	tp = new iPoint[100];
	tpNum = 0;
}
DTUnitMove::~DTUnitMove()
{
	delete tp;
}
void DTUnitMove::setAreaRange(iPoint sp, iPoint ep, float speed)
{
	this->sp = sp;
	this->ep = ep;
	this->speed = speed;
}

bool DTUnitMove::start(MethodWorked m)
{
	bool result = DTUnit::start(m);

	// to do something.. 

	return result;
}
void DTUnitMove::paint(float dt, iPoint position)
{
	// sp ep - line
	setRGBA(0, 0, 0, 1);
	setLineWidth(10);
	drawLine(sp, ep);
	setRGBA(1, 1, 0, 1);
	setLineWidth(4);
	drawLine(sp + iPointMake(2, 2), ep - iPointMake(2, 2));

	//setRGBA(1, 0, 1, 1);
	//iPoint p = this->position + position;
	//fillRect(p.x, p.y, 50, 50);
	//setRGBA(1, 1, 1, 1);
	img->paint(dt, this->position + position);

	// ctrl
	// 물건을 싣는중
	float r = run(dt);
	// r에 따라 싣는 중인 프로그래스바 애니메이션 재생

	if (r < 1.0f)
		return;

	if (tpNum)
	{
		iPoint& tPosition = tp[0];
		position;speed;
		
		iPoint mp = tPosition - position;
		mp.loadIdentity();
		mp = mp * (speed * dt);
		if (move(&position, &tPosition, mp))
		{
			tpNum--;
			memcpy(tp, &tp[1], sizeof(iPoint) * tpNum);

			// methodArrive
			// 도착한 곳에서 컨베이어 물건을 싣는다.
			// 
			start(cbWorked);
		}
	}
	else
	{

	}
}

void DTUnitMove::cbWorked(DTUnit* obj)
{
	DTUnitMove* dm = (DTUnitMove*)obj;

	// 실 데이터를 반영
}
