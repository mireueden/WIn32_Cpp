#include "DTObject.h"

#include "DTProcPop.h"

DTUnit** unit;
int unitNum;
int selectedUnit;
iPoint positionUnit;

int target, curr, complete, broken;
void startMake(int target)
{
	// 주문 우선 순위 나중에 처리...
	::target = target;
	curr = 0;
	complete = 0;
	broken = 0;
}

#include "DTProc.h"
void startMove(int unitIndex)
{
	// 가로나 세로로 움직이는 위치
	const iPoint& pSrc = unit[unitIndex]->position;
	int location = 1;
	if (pSrc.y < FieldHeight * 0.3f)
		location = 0;
	else if (pSrc.y > FieldHeight * 0.7f)
		location = 2;

	for (int i = 0; i < unitNum; i++)
	{
		if (unit[i]->index / 100 == 1)// 이동하는 로봇
		{
			int loc = 1;
			const iPoint& pDst = unit[i]->position;
			if (pDst.y < FieldHeight * 0.3f)
				loc = 0;
			else if (pDst.y > FieldHeight * 0.7f)
				loc = 2;
			if (location == loc)
			{
				DTUnitMove* dm = (DTUnitMove*)unit[i];
				iPoint pDst = iPointMake(pSrc.x, dm->position.y);
				dm->tp[dm->tpNum] = pDst;
				dm->tpNum++;

				dm->havePD = true;
				break;
			}
		}
	}
}

void loadDTObject()
{
	unit = new DTUnit * [6];
	for (int i = 0; i < 5; i++)
	{
		unit[i] = new DTUnitMake(i);
		unit[i]->position = iPointMake(80 + 140 * i, 140);
	}

	DTUnitMove* u = new DTUnitMove(100);
	u->sp = iPointMake(80, 170);
	u->ep = iPointMake(660, 170);
	u->speed = 100.0f;// 레일 움직이는 속도
	u->_delta = 1.0f;// 물건 컨베이어 옮기는 시간
	u->position = (u->sp + u->ep) / 2;
	unit[5] = u;

	selectedUnit = -1;
	positionUnit = iPointZero;

	unitNum = 6;

	target = 0;
	curr = 0;
}

void freeDTObject()
{
	for (int i = 0; i < 6; i++)
		delete unit[i];
	delete unit;
}

static iPoint off;
void drawDTObject(float dt, iPoint off)
{
	::off = off;

	for (int i = 0; i < 6; i++)
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

	switch (stat) {

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
	if (img)
		delete img;
}

bool DTUnit::start(MethodWorked m)
{
	if (delta)
		return false;

	delta = 0.000001f;
	methodWorked = m;
	return true;
}

float DTUnit::run(float dt)
{
	if (delta == 0.0f)
		return 0.0f;

	if (delta < _delta)
	{
		delta += dt;
		float r = delta / _delta;// 진행중 비율
		// r에 따라 싣는중인 애니메이션 디스플레이

		if (delta >= _delta)
		{
			methodWorked(this);
		}
		if (r > 1.0f)
			r = 1.0f;
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
// w : 80 ~ 120,  h : 40 ~ 160

DTUnitMake::DTUnitMake(int index) : DTUnit(index)
{
	MakeInfo* m = &mi[index];

	const char* str[4] = { "대기", "생산", "고장", "완료" };
	int num[4] = { 3, 10, 2, 5 };
	imgs = new iImage * [StateMakeMax];

	iColor4f* c = &m->color;
	setRGBA(c->r, c->g, c->b, c->a);

	setStringSize(18);
	setStringRGBA(0, 0, 0, 1);

	iGraphics* g = iGraphics::share();
	iSize* s = &m->size;
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
			freeImage(tex);
		}
		img->position = iPointMake(-s->width / 2, -s->height);
		img->_aniDt = 0.17f;
		img->startAnimation();
		imgs[i] = img;
	}
	sm = StateMakeReady;

	_delta = m->delta;
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

	sm = StateMakeMake;

	return result;
}

void DTUnitMake::paint(float dt, iPoint position)
{
	img = imgs[sm];
	//setRGBA(1, 0, 1, 1);
	//fillRect(p.x, p.y, 50, 50);
	//setRGBA(1, 1, 1, 1);
	img->paint(dt, this->position + position);

	// ctrl

	// 물건을 생산 하는 중.....
	float r = run(dt);
	// r에 따라 프로그래스바 애니메이션 디스플레이
	if (r)// 생산이 완료가 되더라도 디스플레이
	{
		iPoint p = iPointMake(img->position.x, 0) +
			this->position + position + iPointMake(0, 10);
		setRGBA(0, 0, 0, 1);
		fillRect(p.x, p.y, img->tex->width, 10);
		setRGBA(1, 0, 0, 1);
		fillRect(p.x + 2, p.y + 2, r * (img->tex->width - 4), 10 - 4);
		setRGBA(1, 1, 1, 1);
	}


	if (r < 1.0f)
		return;

	// 다음 생산을 진행
	// 대기 슬롯이 가득찼을때(생산 준비가 되었을때)
	MethodWorked m[6] = {
		cbWorked0, cbWorked1, cbWorked2,
		cbWorked3, cbWorked4, cbWorked5
	};
	start(m[index]);
}

void DTUnitMake::cbWorked0(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
	printf("DTUnitMake[0] 생산완료 => UnitMove\n");
	startMove(dm->index);
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

MakeInfo mi[5] = {
	{ {80, 40},		{ 1, 0, 0, 1}, 3 },
	{ {120, 40},	{ 0, 1, 0, 1}, 200 },
	{ {80, 160},	{ 0, 0, 1, 1}, 1000 },
	{ {120, 120},	{ 1, 1, 0, 1}, 500 },
	{ {80, 80},		{ 1, 0, 1, 1}, 200 },
};

// ========================================
// DTUnitMove
// ========================================
DTUnitMove::DTUnitMove(int index) : DTUnit(index)
{
	index -= 100;// 0 ~ 99
	// index==0 : 가로로 움직이는 로봇
	// index==1 : 세로로 움직이는 로봇

	const char* str[3] = { "대기", "이동", "싣기" };
	int num[3] = { 3, 1, 10 };

	setStringSize(15);
	setStringRGBA(1, 1, 1, 1);
	iGraphics* g = iGraphics::share();
	imgs = new iImage * [StateMoveMax];
	iSize s = iSizeMake(50, 50);
	for (int i = 0; i < StateMoveMax; i++)
	{
		img = new iImage();
		for (int j = 0; j < num[i]; j++)
		{
			g->init(s.width, s.height);

			setRGBA(0, 0, 0, 1);
			g->fillRect(0, 0, s.width, s.height);
			setRGBA(1, 1, 0, 1);
			setLineWidth(2);
			g->drawRect(5, 5, s.width - 10, s.height - 10);

			g->drawString(s.width / 2, s.height / 2, VCENTER | HCENTER, "%s%d", str[i], j);

			Texture* tex = g->getTexture();
			g->clean();
			img->add(tex);
			freeImage(tex);
		}
		img->position = iPointMake(-s.width / 2, -s.height / 2);
		img->_aniDt = 0.17f;
		img->startAnimation();
		imgs[i] = img;
	}

	sm = StateMoveReady;

	sp = iPointZero;
	ep = iPointZero;

	tp = new iPoint[100];
	tpNum = 0;

	havePD = false;
}

DTUnitMove::~DTUnitMove()
{
	for (int i = 0; i < StateMoveMax; i++)
		delete imgs[i];
	delete imgs;
	img = NULL;

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

	sm = StateMovePick;

	return result;
}

void DTUnitMove::paint(float dt, iPoint position)
{
	// sp ep - line 
	iPoint s = sp + position;
	iPoint e = ep + position;

	// ??
	setRGBA(0, 0, 0, 1);
	setLineWidth(10);
	drawLine(s, e);
	setRGBA(1, 1, 0, 1);
	setLineWidth(4);
	drawLine(s + iPointMake(2, 2), e - iPointMake(2, 2));
	setRGBA(1, 1, 1, 1);

	//setRGBA(1, 0, 1, 1);
	//iPoint p = this->position + position;
	//fillRect(p.x, p.y, 50, 50);
	//setRGBA(1, 1, 1, 1);
	img = imgs[sm];
	img->paint(dt, this->position + position);

	// ctrl

	// 물건을 싣는중.....
	float r = run(dt);
	// r에 따라 싣는중인 애니메이션 디스플레이

	if (r && r < 1.0f)
		return;

	if (tpNum)
	{
		sm = StateMoveMove;

		iPoint& curr = this->position;
		iPoint& tPosition = tp[0];
		iPoint mp = tPosition - curr;
		mp.loadIdentity();
		mp = mp * (speed * dt);
		//printf("(%f, %f)\n", curr.x, tPosition.x);
		if (move(&curr, &tPosition, mp))
		{
			tpNum--;
			memcpy(tp, &tp[1], sizeof(iPoint) * tpNum);

			// methodArrive 
			// 도착한 곳에서 컨베이어 물건을 싣는다.
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

	// 만든 물건을 컨베이어 싣기 => 그 다음 공정으로 이동
	if (dm->havePD)
	{
		dm->havePD = false;

		int index = 0;// 무조건 첫번째가 아님
		DTUnit* u = unit[index];
		u->delta = 0.0f;// 상태변경을 위해서
		MethodWorked m[] = {
			DTUnitMake::cbWorked0, DTUnitMake::cbWorked1, DTUnitMake::cbWorked2,
			DTUnitMake::cbWorked3, DTUnitMake::cbWorked4, DTUnitMake::cbWorked5,
			DTUnitMake::cbWorked9
		};
		u->start(m[index]);

		index = 1;// 무조건 두번째가 아님
		u = unit[index];
		iPoint pDst = iPointMake(u->position.x, dm->position.y);// 다음 공정
		dm->tp[dm->tpNum] = pDst;
		dm->tpNum++;
	}
	// 컨베이어 있는 물건을 생산로봇에 전달 => 대기
	else
	{
		dm->sm = StateMoveReady;

		int index = 1;// 무조건 두번째가 아님
		DTUnitMake* u = (DTUnitMake*)unit[index];
		u->start(DTUnitMake::cbWorked1);// 다음 공정 생산 시작

		for (int i = 0; i < 10; i++)
		{
			if (u->slot[i] == false)
			{
				u->slot[i] = true;
			}
		}


	}

}

