#include "DTObject.h"

#include "DTProcPop.h"

DTItem* dtItem;
int dtItemNum;

DTUnit** unit;
int unitNum;
int selectedUnit;
iPoint positionUnit;

// 목표개수 oi[0]._num
// 공정중인개수, 완료된 개수, 실패된 개수
int curr, complete, broken;

static iPoint off;

OrderInfo* oi;
int oiNum;
bool oiComplete;

OrderInfo* oiBk;
int oiNumBk;

iShortestPath* dtsp;

void startMake(int orderPD, int orderNum)
{
	OrderInfo* o = &oi[oiNum];
	o->pd = orderPD;
	o->num = orderNum / 5 * 3;
	o->_num = orderNum;// uint[0](3 => 15) => unit[3](15 => 5)
	oiNum++;

	if (oiNum == 1)
	{
		curr = 0;
		oiComplete = false;
	}
}

void loadDTObject()
{
	dtItem = new DTItem[dtItemMax];
	for (int i = 0; i < dtItemMax; i++)
		dtItem[i].pd = -1;
	dtItemNum = 0;

	// make : 0, 1, 2, 3
	// move : 4
	// sida : 5
	// success : 6, fail : 7, 8
	unitNum = 9;
	unit = new DTUnit * [unitNum];
	// make
	for (int i = 0; i < 4; i++)
	{
		DTUnit* u = new DTUnitMake(i);
		unit[i] = u;
		u->position = iPointMake(80 + 140 * i, 140 - 30 * i);
		u->positionSida = u->position + iPointMake(-20, 40);
	}
	// move
	DTUnitMove* u = new DTUnitMove(100);
	u->sp = iPointMake(60, 200);
	u->ep = iPointMake(660, 200);
	u->speed = 100.0f;// 레일 움직이는 속도
	u->_delta = 1.0f;// 물건 컨베이어 옮기는 시간
	u->position = (u->sp + u->ep) / 2;
	unit[4] = u;
	// sida
	unit[5] = new DTUnitSida(200);
	unit[5]->position = iPointMake(devSize.width / 2, devSize.height / 2);
	// success
	unit[6] = new DTUnitSuccess(300);
	unit[6]->position = iPointMake(80, devSize.height * 0.7f);
	unit[6]->positionSida = unit[6]->position + iPointMake(-10, -70);
	// fail x 2
	for (int i = 0; i < 2; i++)
	{
		DTUnitFail* u = new DTUnitFail(400 + i);
		u->position = iPointMake(220 + 140 * i, devSize.height * 0.7f);
		u->positionSida = u->position + iPointMake(-10, -70);
		unit[7 + i] = u;
	}

	selectedUnit = -1;
	positionUnit = iPointZero;

	curr = 0;
	complete = 0;
	broken = 0;

	off = iPointZero;

	oi = new OrderInfo[oiMax];
	oiNum = 0;

	oiBk = new OrderInfo[oiMax];
	oiNumBk = 0;

	dtsp = new iShortestPath();
}

void freeDTObject()
{
	delete dtItem;

	for (int i = 0; i < unitNum; i++)
		delete unit[i];
	delete unit;

	delete oi;
	delete oiBk;

	delete dtsp;
}

void drawDTObject(float dt, iPoint off)
{
	::off = off;

	for (int i = 0; i < unitNum; i++)
		unit[i]->paint(dt, off);

	if (selectedUnit == -1)
		showToast(positionUnit, NULL);
	else
		showToast(positionUnit, "unit[%d] 선택됨\n", selectedUnit);

	if (oiNum)
	{
		OrderInfo* o = &oi[0];
		if (o->num == 0 && oiComplete)
		{
			oiComplete = false;

			memcpy(&oiBk[oiNumBk], oi, sizeof(OrderInfo));
			oiNumBk++;

			oiNum--;
			memcpy(oi, &oi[1], sizeof(OrderInfo) * oiNum);
			curr = 0;
			return;
		}
		//else// if (o->num)

		// 주문 우선 순위 나중에 처리...
		DTUnitMake* u = (DTUnitMake*)unit[0];
		int num = 0;// 주문을 받을 수 있는 개수
		for (int i = 0; i < u->slotInNum; i++)
		{
			if (u->slotIn[i] == NULL)
				num++;
		}
		// num 주문을 할 수 있는 개수
		if (oi->num <= num)
		{
			num = oi->num;
			oi->num = 0;
		}
		else if (oi->num > num)
		{
			// num 주문
			oi->num -= num;
		}

		for (int i = 0; i < num; i++)
		{
			//DTItem* it = &dtItem[dtItemNum + i];
			DTItem* it = NULL;
			for (int j = 0; j < dtItemMax; j++)
			{
				if (dtItem[j].pd == -1)
				{
					it = &dtItem[j];
					break;
				}
			}
			it->pd = oi->pd;
			int* p = itemPath[oi->pd];
			memcpy(it->path, &p[1], sizeof(int) * p[0]);
			it->pathNum = p[0];
			it->pathIndex = 0;

			for (int j = 0; j < u->slotInNum; j++)
			{
				if (u->slotIn[j] == NULL)
				{
					u->slotIn[j] = it;
					break;
				}
			}
		}
		dtItemNum += num;
		curr += num;
	}
}

bool keyDTObject(iKeyStat stat, iPoint point)
{
	int i, j = -1;

	switch (stat) {

	case iKeyStatBegan:
		i = selectedUnit; 
		if (i == -1) break;
		showPopUnit(true, i);
		break;

	case iKeyStatMoved:
		for (int i = 0; i < unitNum; i++)
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

	positionSida = iPointZero;

	exe = 0;
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

void DTUnit::displaySida(float dt, iPoint position)
{
	setLineWidth(1);
	setRGBA(1, 1, 1, 1);
	fillRect(positionSida.x + position.x,
			 positionSida.y + position.y, 5, 5, 2.5f);
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
			delta = 0.0f;
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
	RobotInfo* m = &ri[index];

	const char* str[4] = {"대기", "생산", "고장", "완료"};
	int num[4] = {3, 10, 2, 5};
	iColor4f sc = iColor4fMake(0, 0, 0, 1);
	imgs = createRobot(
		index, 4,
		str, num,
		&m->size, &m->color,
		18, &sc);
	sm = StateMakeReady;

	int* n = &slotInOut[index][0];
	slotInNum = n[0];
	slotOutNum = n[1];

	slotIn = new DTItem*[slotInNum];
	memset(slotIn, 0x00, sizeof(DTItem*) * slotInNum);
	slotOut = new DTItem*[slotOutNum];
	memset(slotOut, 0x00, sizeof(DTItem*) * slotOutNum);

	n = &slotInOut[index][2];
	makeInNum = n[0];
	makeOutNum = n[1];
	makeSlotIn = new int[slotInNum];

	_delta = m->delta;
}

DTUnitMake::~DTUnitMake()
{
	for (int i = 0; i < StateMakeMax; i++)
		delete imgs[i];
	delete imgs;
	img = NULL;

	delete slotIn;
	delete slotOut;
	delete makeSlotIn;
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
	iPoint p = this->position + position;
	img->paint(dt, p);

	displaySida(dt, position);

	p.x += img->position.x;
	for (int i = 0; i < slotInNum; i++)
	{
		setRGBA(0, 0, 0, 1);
		fillRect(p.x + 12 * i, p.y + 5, 10, 10);
		if (slotIn[i])
		{
			setRGBA(1, 1, 0, 1);
			fillRect(p.x + 1 + 12 * i, p.y + 1 + 5, 8, 8);
		}
	}
	for (int i = 0; i < slotOutNum; i++)
	{
		setRGBA(0, 0, 0, 1);
		fillRect(p.x + 12 * i, p.y + 20, 10, 10);
		if (slotOut[i])
		{
			setRGBA(0, 1, 1, 1);
			fillRect(p.x + 1 + 12 * i, p.y + 1 + 20, 8, 8);
		}
	}
	setRGBA(1, 1, 1, 1);

	// ctrl

	// 물건을 생산 하는 중.....
	float r = run(dt);
	// r에 따라 프로그래스바 애니메이션 디스플레이
	if (r)// 생산이 완료가 되더라도 디스플레이
	{
		// 생산 프로그래스 바
		p = iPointMake(img->position.x, 0) +
			this->position + position + iPointMake(0, 35);
		setRGBA(0, 0, 0, 1);
		fillRect(p.x, p.y, img->tex->width, 10);
		setRGBA(1, 0, 0, 1);
		fillRect(p.x + 2, p.y + 2, r * (img->tex->width - 4), 10 - 4);
		setRGBA(1, 1, 1, 1);
	}
	else// if (r == 0.0f)
	{
		int inNum = 0;
		for (int i = 0; i < slotInNum; i++)
		{
			if (slotIn[i])
				inNum++;
		}
		if (inNum < makeInNum)
			return;

		int outNum = 0;
		for (int i = 0; i < slotOutNum; i++)
		{
			if (slotOut[i] == NULL)
				outNum++;
		}
		if (outNum < makeOutNum)
			return;

		inNum = 0;
		for (int i = 0; i < slotInNum; i++)
		{
			if (slotIn[i])
			{
				makeSlotIn[inNum] = i;
				inNum++;
				if (inNum == makeInNum)
					break;
			}
		}

		MethodWorked m[4] = {
			cbWorked0, cbWorked1,
			cbWorked2, cbWorked3,
		};
		start(m[index]);
	}
}

void DTUnitMake::cbWorked0(DTUnit* obj)
{
	DTUnitMake* dm = (DTUnitMake*)obj;
	
	// 실 데이터를 반영
	printf("DTUnitMake[0] 생산완료 => UnitMove\n");
	dm->exe++;

	dm->sm = StateMakeReady;

	// 추가적으로 만들어야 하는 DTItem
	DTItem** input;
	if (dm->makeInNum == dm->makeOutNum)
	{
		input = new DTItem * [dm->makeInNum];
		for (int i = 0, j = 0; i < dm->slotInNum; i++)
		{
			if (dm->slotIn[i])
			{
				input[j] = dm->slotIn[i];
				j++;
			}
		}
	}
	else if (dm->makeInNum < dm->makeOutNum)
	{	// 증가
		DTItem* dti = NULL;
		input = new DTItem * [dm->makeOutNum];
		for (int i = 0, j = 0; i < dm->slotInNum; i++)
		{
			if (dm->slotIn[i])
			{
				dti =
				input[j] = dm->slotIn[i];
				j++;
			}
		}
		int add = dm->makeOutNum - dm->makeInNum;
		for (int i = 0; i < add; i++)
		{
			for (int j = 0; j < dtItemMax; j++)
			{
				if (dtItem[j].pd == -1)
				{
					memcpy(&dtItem[j], dti, sizeof(DTItem));
					input[dm->makeInNum + i] = &dtItem[j];
					break;
				}
			}
		}
	}
	else
	{	// 감소
		input = new DTItem * [dm->makeInNum];
		for (int i = 0, j = 0; i < dm->slotInNum; i++)
		{
			if (dm->slotIn[i] == NULL)
				continue;

			if (j < dm->slotOutNum)
			{
				input[j] = dm->slotIn[i];
				j++;
			}
			else
			{
				dm->slotIn[i]->pd = -1;
			}
		}
	}

	for (int i = 0, j = 0; i < dm->slotOutNum; i++)
	{
		if (dm->slotOut[i] == NULL)
		{
			dm->slotOut[i] = input[j];
			j++;
			if (j == dm->makeOutNum)
				break;
		}
	}

	delete input;

	//for (int i = 0, j = 0; i < dm->slotInNum; i++)
	//{
	//	if (dm->slotIn[i])
	//	{
	//		dm->slotIn[i] = NULL;
	//		j++;
	//		if (j == dm->makeInNum)
	//			break;
	//	}
	//}
	for (int i = 0; i < dm->makeInNum; i++)
	{
		int j = dm->makeSlotIn[i];
		dm->slotIn[j] = NULL;
	}
}
void DTUnitMake::cbWorked1(DTUnit* obj)
{
	cbWorked0(obj);
	return;

	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
}
void DTUnitMake::cbWorked2(DTUnit* obj)
{
	cbWorked0(obj);

	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
}
void DTUnitMake::cbWorked3(DTUnit* obj)
{
	cbWorked0(obj);

	DTUnitMake* dm = (DTUnitMake*)obj;

	// 실 데이터를 반영
	complete++;
}

// ========================================
// DTUnitMove
// ========================================
DTUnitMove::DTUnitMove(int index) : DTUnit(index)
{
	index -= 100;// 0 ~ 99
	// index==0 : 가로로 움직이는 로봇
	// index==1 : 세로로 움직이는 로봇
	RobotInfo* m = &ri[4];

	const char* str[3] = {"대기", "이동", "싣기"};
	int num[3] = { 3, 1, 10 };
	iColor4f sc = iColor4fMake(1, 1, 1, 1);
	imgs = createRobot(
		index, 3,
		str, num,
		&m->size, &m->color,
		15, &sc);
	sm = StateMoveReady;

	sp = iPointZero;
	ep = iPointZero;

	have = NULL;
}

DTUnitMove::~DTUnitMove()
{
	for (int i = 0; i < StateMoveMax; i++)
		delete imgs[i];
	delete imgs;
	img = NULL;
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

	if (sm == StateMoveReady)
	{
		// find & go
		DWORD min = 0xffffffff;
		int unitIndex = -1;
		for (int i = 0; i < unitNum; i++)
		{
			//if (unit[i]->index < 100)
			if (unit[i]->index < 3)// 제품 완료가 된, 마지막 공정 제외 exption예상!! 716 line 오버플로우 발생 가능성
			{
				DTUnitMake* u = (DTUnitMake*)unit[i];
				for (int j = 0; j < u->slotOutNum; j++)
				{
					DTItem* it = u->slotOut[j];
					if (it == NULL) continue;
					if (min > it->makeEnd)
					{
						min = it->makeEnd;
						unitIndex = i;
					}
				}
			}
		}
		if (unitIndex != -1)
		{
			tPosition.x = unit[unitIndex]->position.x;
			tPosition.y = this->position.y;
			this->unitIndex = unitIndex;

			sm = StateMoveMove;
		}
	}
	else if (sm == StateMoveMove)
	{
		// arrive & StateMovePick
		iPoint mp = tPosition - this->position;
		mp.loadIdentity();
		mp = mp * (speed * dt);
		if (move(&this->position, &tPosition, mp))
		{
			sm = StateMovePick;
			start(cbWorked);
		}
	}
	else if (sm == StateMovePick)
	{
		// 물건을 싣는중.....
		float r = run(dt);
		if (r)// 싣는(내리는)중인 애니메이션 디스플레이
		{
			iPoint p = iPointMake(img->position.x, 0) +
				this->position + position + iPointMake(0, 35);
			setRGBA(0, 0, 0, 1);
			fillRect(p.x, p.y, img->tex->width, 10);
			setRGBA(1, 0, 0, 1);
			fillRect(p.x + 2, p.y + 2, r * (img->tex->width - 4), 10 - 4);
			setRGBA(1, 1, 1, 1);
		}

		// 콜백 : cbWorked
	}
}

void DTUnitMove::cbWorked(DTUnit* obj)
{
	DTUnitMove* dm = (DTUnitMove*)obj;

	// 물건 내리기
	if (dm->have)
	{
		DTItem* it = dm->have;
		int index = it->path[it->pathIndex];
		DTUnitMake* u = (DTUnitMake*)unit[index];
		for (int i = 0; i < u->slotInNum; i++)
		{
			if (u->slotIn[i] == NULL)
			{
				u->slotIn[i] = dm->have;
				dm->have = NULL;

				dm->sm = StateMoveReady;

				dm->exe++;
				break;
			}
		}
	}
	// 물건 싣기
	else// if (have == NULL)
	{
		DTUnitMake* u = (DTUnitMake*)unit[dm->unitIndex];
		DWORD min = 0xFFFFFFFF;
		int index = -1;
		for (int i = 0; i < u->slotOutNum; i++)
		{
			DTItem* it = u->slotOut[i];
			if (it == NULL) continue;
			if (min > it->makeEnd)
			{
				min = it->makeEnd;
				index = i;
			}
		}
		dm->have = u->slotOut[index];
		u->slotOut[index] = NULL;

		// go
		dm->have->pathIndex++;// exception 예상!! 619 line에서 애초에 못 들어오게 했음
		int targetIndex = dm->have->path[dm->have->pathIndex];
		dm->tPosition.x = unit[targetIndex]->position.x;
		dm->tPosition.y = dm->tPosition.y;

		dm->sm = StateMoveMove;
	}
}

// ========================================
// DTUnitSida
// ========================================
DTUnitSida::DTUnitSida(int index) : DTUnit(index)
{
	index -= 200;// 0 ~ 99

	RobotInfo* m = &ri[5];

	// 기본적으로 왼쪽으로 보는 로봇(상/하)
	const char* str[4] = {"대기", "이(상)", "이(하)", "싣기"};
	int num[4] = { 3, 4, 4, 5 };
	iColor4f sc = iColor4fMake(0, 0, 0, 1);
	imgs = createRobot(
		index, 4,
		str, num,
		&m->size, &m->color,
		8, &sc);
	sm = StateMoveReady;

	path = new iPoint[32 * 24];
	pathNum = 0;
	pathIndex = 0;

	speed = 200;

	have = NULL;
}
DTUnitSida::~DTUnitSida()
{
	for (int i = 0; i < StateMoveMax; i++)
		delete imgs[i];
	delete imgs;
	img = NULL;

	delete path;
}

bool DTUnitSida::start(MethodWorked m)
{
	return false;
}

#include "DTProc.h"
void DTUnitSida::paint(float dt, iPoint position)
{
	img = imgs[sm];
	img->paint(dt, this->position + position);

	setLineWidth(2);
	setRGBA(1, 1, 1, 1);
	int j = pathNum - 1;
	for (int i = 0; i < j; i++)
	{
		drawLine(	path[i] + position,
					path[i + 1] + position);
	}

	// ctrl

	if (sm == StateMoveReady)
	{
		// 불량품 회수
		DWORD min = 0xffffffff;
		int selctedMake = -1;
		for (int i = 0; i < 3; i++)
		{
			DTUnitMake* u = (DTUnitMake*)unit[i];
			for (int j = 0; j < u->slotOutNum; j++)
			{
				DTItem* it = u->slotOut[j];
				if (it == NULL ||
					it->pd < 100)
					continue;

				if (min > it->makeEnd)
				{
					min = it->makeEnd;
					selctedMake = i;
				}
			}
		}

		if (selctedMake != -1)
		{
			dtsp->set(fieldTile, fieldX, fieldY, fieldW, fieldH);
			dtsp->run(this->position, unit[selctedMake]->positionSida, path, pathNum);
			pathIndex = 0;
			targetUnit = selctedMake;
			sm = StateMoveMove;
		}
		// 완제품 회수
		else
		{
			bool exist = false;

			DTUnitMake* u = (DTUnitMake*)unit[3];
			for (int i = 0; i < u->slotOutNum; i++)
			{
				if (u->slotOut[i])
				{
					exist = true;
					break;
				}
			}

			if (exist)
			{
				dtsp->set(fieldTile, fieldX, fieldY, fieldW, fieldH);
				dtsp->run(this->position, u->positionSida, path, pathNum);
				pathIndex = 0;
				targetUnit = 3;
				sm = StateMoveMove;
			}
		}
	}

	if (pathIndex < pathNum)
	{
		iPoint tPosition = path[pathIndex];
		iPoint mp = tPosition - this->position;
		mp.loadIdentity();
		mp = mp * (speed * dt);
		if (move(&this->position, &tPosition, mp))
		{
			pathIndex++;
			if (pathIndex == pathNum)
			{
				printf("arrive sida\n");
				if (have == NULL)
				{
					DWORD min = 0xffffffff;
					DTItem* item = NULL;

					DTUnitMake* u = (DTUnitMake*)unit[targetUnit];
					for (int i = 0; i < u->slotOutNum; i++)
					{
						DTItem* it = u->slotOut[i];
						if (it == NULL)
							continue;
						if (targetUnit < 3)// 불량품
						{
							if (it->pd < 100)
								continue;
						}
						else if (targetUnit == 3)// 정상품
						{
							// 불량품 만들지 않았으니...
						}

						if (min > it->makeEnd)
						{
							min = it->makeEnd;
							item = it;
						}
					}
					have = item;
					//item = NULL;// u->slotOut[i] = NULL
					for (int i = 0; i < u->slotOutNum; i++)
					{
						if (u->slotOut[i] == item)
						{
							u->slotOut[i] = NULL;
							break;
						}
					}

					if (item->pd < 100)
					{
						// 정상품
						targetUnit = 6;
					}
					else
					{
						// 불량품
						int pd = item->pd % 100;
						if (targetUnit == 0)
							targetUnit = 7;
						else
							targetUnit = 8;
					}
					dtsp->set(fieldTile, fieldX, fieldY, fieldW, fieldH);
					dtsp->run(this->position, unit[targetUnit]->positionSida, path, pathNum);
					pathIndex = 0;
					sm = StateMoveMove;
				}
				else// if( have )
				{
					if (targetUnit == 6)
					{
						DTUnitSuccess* u = (DTUnitSuccess*)unit[6];
						for (int i = 0; i < u->_haveNum; i++)
						{
							if (u->have[i] == NULL)
							{
								u->have[i] = have;
								u->haveNum++;
								curr--;
								complete++;
								break;
							}
						}
						have = NULL;

						// 출고개수에 도달....
						if (u->haveNum == oi[0]._num)
						{
							// 출고처리
							printf("출고처리!! %d\n", oi[0]._num);
							oiComplete = true;
							u->haveNum = 0;
							u->exe++;
						}
					}
					else// if (targetUnit == 7 || targetUnit == 8)
					{
						// 폐기처리
						printf("폐기!!\n");
						curr--;
						broken++;
						oi[0].num++;
						have = NULL;

						DTUnitSuccess* u = (DTUnitSuccess*)unit[targetUnit];
						u->exe++;
					}
					sm = StateMoveReady;
					exe++;
				}
			}
		}
	}

}
void DTUnitSida::cbWorked(DTUnit* obj)
{

}

// ========================================
// DTUnitSuccess
// ========================================
DTUnitSuccess::DTUnitSuccess(int index) : DTUnit(index)
{
	index -= 300;

	RobotInfo* m = &ri[6];
	const char* str[1] = { "대기", };
	int num[1] = { 3, };
	iColor4f sc = iColor4fMake(0, 0, 0, 1);
	iImage** imgs = createRobot(
		index, 1,
		str, num,
		&m->size, &m->color,
		15, &sc);
	img = imgs[0];
	delete imgs;

	_haveNum = 100;
	have = new DTItem * [_haveNum];
	memset(have, 0x00, sizeof(DTItem*) * _haveNum);
	haveNum = 0;
}
DTUnitSuccess::~DTUnitSuccess()
{
	delete have;
}

void DTUnitSuccess::paint(float dt, iPoint position)
{
	iPoint p = this->position + position;
	img->paint(dt, p);

	displaySida(dt, position);

	p.x += img->position.x;
	for (int i = 0; i < haveNum; i++)
	{
		setRGBA(0, 0, 0, 1);
		fillRect(p.x + 12 * i, p.y + 5, 10, 10);
		if (have[i])
		{
			setRGBA(1, 1, 0, 1);
			fillRect(p.x + 1 + 12 * i, p.y + 1 + 5, 8, 8);
		}
	}
}

// ========================================
// DTUnitFail
// ========================================
DTUnitFail::DTUnitFail(int index) : DTUnit(index)
{
	index -= 400;

	RobotInfo* m = &ri[6];
	const char* str[1] = { "대기", };
	int num[1] = { 3, };
	iColor4f sc = iColor4fMake(0, 0, 0, 1);
	iImage** imgs = createRobot(
		index, 3,
		str, num,
		&m->size, &m->color,
		15, &sc);
	img = imgs[0];
	delete imgs;
}
DTUnitFail::~DTUnitFail()
{

}

void DTUnitFail::paint(float dt, iPoint position)
{
	img->paint(dt, this->position + position);
	displaySida(dt, position);
}

// ===================================
// 로봇 이미지 생성
// ===================================
iImage** createRobot(
	int index, int beNum,
	const char** strBe, int* beAniNum,
	iSize* s, iColor4f* c,
	float ss, iColor4f* sc)
{
	iImage** imgs = new iImage*[beNum];

	setRGBA(c->r, c->g, c->b, c->a);

	setStringSize(ss);
	setStringRGBA(sc->r, sc->g, sc->b, sc->a);

	iGraphics* g = iGraphics::share();
	for (int i = 0; i < beNum; i++)
	{
		iImage* img = new iImage();
		for (int j = 0; j < beAniNum[i]; j++)
		{
			g->init(s->width, s->height);

			g->fillRect(0, 0, s->width, s->height);
			g->drawString(s->width / 2, s->height / 2 - 10, VCENTER | HCENTER,
				"index%d", index);
			g->drawString(s->width / 2, s->height / 2 + 10, VCENTER | HCENTER,
				"%s%d", strBe[i], j);

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

	return imgs;
}

// ===================================
// 로봇 데이터
// ===================================
RobotInfo ri[8] = {
	// 0 ~ 3 : Make
	{ {80, 40},		{ 1, 0, 0, 1}, 3 },// 쇠 => 볼트, 너트
	{ {120, 40},	{ 0, 1, 0, 1}, 2 },// 도색 => 블랙볼트
	{ {80, 160},	{ 0, 0, 1, 1}, 2 },// 도색 => 화이트볼트
	{ {120, 120},	{ 1, 1, 0, 1}, 10 },// 포장
	// 4 : Move
	{ {50, 50},		{ 0, 0, 0, 1}, 200 },
	// 5 : Sida
	{ {30, 30},		{0.5, 0.5, 0.5, 1}, 1 },
	// 6 : Success
	{ {120, 60},	{0.25, 0.25, 0.25, 1}, 0 },
	// 7 : Fail
	{ { 120, 60 },	{0.25, 0.0, 0.0, 1}, 100 }
};

int itemPath[6][4] = {
	{ 2,     0, 3, },
	{ 2,     0, 3, },
	{ 3,     0, 1, 3, },
	{ 3,     0, 1, 3, },
	{ 3,     0, 2, 3, },
	{ 3,     0, 2, 3, },
};

int slotInOut[4][4] = {
	{3, 20,		1, 5},
	{10, 10,	1, 1},
	{10, 10,	1, 1},
	{10, 2,		3, 1},
};
