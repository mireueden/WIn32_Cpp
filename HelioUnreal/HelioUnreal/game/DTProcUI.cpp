#include "DTProcUI.h"

iPopup** popProcUI;
static iStrTex** stProcUI;
Texture* methodStProcUI(const char* str);
Texture* methodStOrder(const char* str);

int orderPD, orderNum;
iImage** imgProcUIBtn;

void loadDTProcUI()
{
	loadProcData();
	float takeTime = (pd->playtimeCurr - pd->playtimeLast) / 1000.0f;
	// 내가 접속하지 않은 시간 만큼 시뮬레이션

	popProcUI = new iPopup * [2];
	stProcUI = new iStrTex * [3];

	//
	// popup-top
	//
	iPopup* pop = new iPopup();

	iImage* img = new iImage();
	iStrTex* st = new iStrTex(methodStProcUI);
	st->set("0");
	img->add(st->tex);
	pop->add(img);
	stProcUI[0] = st;

	pop->style = iPopupStyleMove;
	pop->sp = iPointMake(0, -40);
	pop->ep = iPointMake(0, 10);
	pop->_aniDt = 1.0f;
	popProcUI[0] = pop;

	//
	// popup-bottom
	//
	pop = new iPopup();

	img = new iImage();
	st = new iStrTex(methodStProcUI);
	st->set("0");
	img->add(st->tex);
	pop->add(img);
	stProcUI[1] = st;

	setStringSize(20);
	setStringRGBA(0, 0, 1, 1);
	img = new iImage();
	st = new iStrTex(methodStOrder);
	orderPD = 0;
	orderNum = 0;
	st->set("%d\n%d", orderPD, orderNum);
	img->add(st->tex);
	img->position = iPointMake(devSize.width - 290 - 150, 5);
	pop->add(img);
	stProcUI[2] = st;

	imgProcUIBtn = new iImage * [5];
	const char* str[5] = { "▲", "▼", "▲", "▼", "주문"};
	iSize size[5] = { {30,30}, {30,30}, {30,30}, {30,30}, {50, 30} };
	for (int i = 0; i < 5; i++)
	{
		img = new iImage();

		iGraphics* g = iGraphics::share();
		iSize& s = size[i];
		for (int j = 0; j < 2; j++)
		{
			g->init(s.width, s.height);

			if (j == 0)
			{
				setRGBA(1, 1, 1, 1);
				g->fillRect(0, 0, s.width, s.height);

				setStringSize(20);
				setStringRGBA(0, 0, 0, 1);
			}
			else
			{
				setRGBA(0.5, 0.5, 0.5, 1);
				g->fillRect(5, 5, s.width-10, s.height-10);

				setStringSize(18);
				setStringRGBA(0, 0, 0, 1);
			}
			g->drawString(s.width / 2, s.height / 2, VCENTER | HCENTER, str[i]);

			Texture* tex = g->getTexture();
			img->add(tex);
			freeImage(tex);
		}
		img->position = iPointMake(devSize.width - 280 + 60 + 35 * i, 5);
		pop->add(img);
		imgProcUIBtn[i] = img;
	}

	pop->style = iPopupStyleMove;
	pop->sp = iPointMake(0, devSize.height);
	pop->ep = iPointMake(0, devSize.height - 40);
	pop->_aniDt = 1.0f;
	popProcUI[1] = pop;

	showDTProcUI(true);
}

void freeDTProcUI()
{
	for (int i = 0; i < 2; i++)
		delete popProcUI[i];
	delete popProcUI;

	for (int i = 0; i < 3; i++)
		delete stProcUI[i];
	delete stProcUI;

	delete imgProcUIBtn;
}

Texture* methodStProcUI(const char* str)
{
	iGraphics* g = iGraphics::share();

	iSize size = iSizeMake(devSize.width, 35);
	g->init(size.width, size.height);

	setRGBA(0, 0, 0, 0.5f);
	g->fillRect(5, 0, size.width - 10, size.height);
	setRGBA(1, 1, 1, 1);

	setStringSize(20);
	setStringRGBA(1, 1, 1, 1);
	g->drawString(10, size.height / 2, VCENTER | LEFT, str);

	Texture* tex = g->getTexture();
	g->clean();
	return tex;
}
Texture* methodStOrder(const char* str)
{
	int lineNum;
	char** line = iString::split(lineNum, str);
	int orderPD = atoi(line[0]);
	int orderNum = atoi(line[1]);
	iString::free(line, lineNum);

	iGraphics* g = iGraphics::share();

	iSize size = iSizeMake(200, 35);
	g->init(size.width, size.height);

	setRGBA(0, 0, 0, 0.8f);
	g->fillRect(0, 0, size.width, size.height);
	setRGBA(1, 1, 1, 1);

	setStringSize(20);
	setStringRGBA(1, 0, 1, 1);
	const char* material[6] = {
		"bolt-silver-pack",
		"nut-silver-pack",
		"bolt-black-pack",
		"nut-black-pack",
		"bolt-white-pack",
		"nut-white-pack",
	};
	g->drawString(size.width, size.height / 2, VCENTER | RIGHT, "%s x %d",
		material[orderPD], orderNum);

	Texture* tex = g->getTexture();
	g->clean();
	return tex;
}

#include "DTObject.h"// curr 
void drawDTProcUI(float dt)
{
	int total = 0, curr = 0;
	for (int i = 0; i < oiNumBk; i++)
	{
		total += oiBk[i]._num;
	}
	for (int i = 0; i < oiNum; i++)
	{
		total += oi[i]._num;
		curr += oi[i].num;
	}

	stProcUI[0]->set("총 수주 개수 : %d개 완료 개수 : %d개, 공정 개수 : %d개",
		total, complete, curr);
	stProcUI[1]->set("총 접속 시간 : %.0fs 현재 접속 시간 : %.0fs", 
		pd->playtimeTotal/1000.0f,
		(GetTickCount() - pd->playtimeCurr) / 1000.0f);
	stProcUI[2]->set("%d\n%d", orderPD, orderNum);

	for (int i = 0; i < 5; i++)
		imgProcUIBtn[i]->index = (popProcUI[1]->selected == i);

	for (int i = 0; i < 2; i++)
		popProcUI[i]->paint(dt);
}

bool keyDTProcUI(iKeyStat stat, iPoint point)
{
	iPopup* pop = popProcUI[1];
	if (pop->bShow == false ||
		point.y < pop->ep.y)
		return false;

	int i, j = -1;

	switch (stat) {
	case iKeyStatBegan:
		i = pop->selected;
		if (i == -1) break;

		if (i == 0)
		{
			orderPD--;
			if (orderPD < 0)
				orderPD = 0;
		}
		else if (i == 1)
		{
			orderPD++;
			if (orderPD > 5)
				orderPD = 5;
		}
		else if (i == 2)
			orderNum += 5;
		else if (i == 3)
		{
			orderNum -= 5;
			if (orderNum < 0)
				orderNum = 0;
		}
		else// if (i == 4)
		{
			// 1 => 5
			printf("주문 하기 %d\n", orderNum);
			startMake(orderPD, orderNum);
			orderNum = 0;
		}
		break;

	case iKeyStatMoved:
		for (i = 0; i < 5; i++)
		{
			if (containPoint(point, imgProcUIBtn[i]->touchRect(pop->ep)))
			{
				j = i;
				break;
			}
		}
		if (pop->selected != j)
			printf("snd btn %s\n", j!=-1 ? "버튼음" : "취소음");
		pop->selected = j;
		break;

	case iKeyStatEnded:
		break;
	}

	return true;
}

void showDTProcUI(bool show)
{
	for (int i = 0; i < 2; i++)
		popProcUI[i]->show(show);
}


ProcData* pd;
void loadProcData()
{
	int len;
	pd = (ProcData*)loadFile(len, ProcDataSaveFile);
	if (pd == NULL)
	{
		pd = new ProcData;
		pd->playtimeTotal = 0;
		pd->playtimeCurr = GetTickCount();
		pd->playtimeLast = 0;

		pd->unitTotal = 10;		// 공장 들어온 설비 개수
		pd->unitBroken = 0;		// 소모된 개수
		pd->unitRun = 10;
		pd->unitRepairing = 0;	//
		pd->unitRepaired = 0;	//
		pd->unitReplace = 0;

		pd->target = 0;			// 생산 목표 개수(수주)
		pd->made = 0;			// 현재 진행 개수

		memset(pd->unitMakeTime, 0x00, sizeof(float)*10);// 10개라고 가정
		saveProcData();
	}
	pd->playtimeCurr = GetTickCount();
}

void saveProcData()
{
	pd->playtimeLast = GetTickCount();
	pd->playtimeTotal += pd->playtimeLast - pd->playtimeCurr;
	saveFile((char*)pd, sizeof(ProcData), ProcDataSaveFile);
}
