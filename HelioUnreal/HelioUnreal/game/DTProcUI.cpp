#include "DTProcUI.h"

iPopup** popProcUI;
static iStrTex** stProcUI;
Texture* methodStProcUI(const char* str);

int orderNum;
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
	st = new iStrTex();
	orderNum = 0;
	st->set("%d", orderNum);
	img->add(st->tex);
	img->position = iPointMake(devSize.width - 200, 5);
	pop->add(img);
	stProcUI[2] = st;

	imgProcUIBtn = new iImage * [3];
	const char* str[3] = { "▲", "▼", "주문" };
	iSize size[3] = { {30,30}, {30,30}, {50, 30} };
	for (int i = 0; i < 3; i++)
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
				g->fillRect(5, 5, s.width - 10, s.height - 10);

				setStringSize(18);
				setStringRGBA(0, 0, 0, 1);
			}
			g->drawString(s.width / 2, s.height / 2, VCENTER | HCENTER, str[i]);

			Texture* tex = g->getTexture();
			img->add(tex);
			freeImage(tex);
		}
		img->position = iPointMake(devSize.width - 220 + 60 + 35 * i, 5);
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


#include "DTObject.h"// curr 
void drawDTProcUI(float dt)
{
	stProcUI[0]->set("총 수주 개수 : %d개 완료 개수 : %d개, 공정 개수 : %d개",
		pd->target, pd->made, curr);
	stProcUI[1]->set("총 접속 시간 : %.0fs 현재 접속 시간 : %.0fs",
		pd->playtimeTotal / 1000.0f,
		(GetTickCount() - pd->playtimeCurr) / 1000.0f);
	setStringSize(20);
	setStringRGBA(0, 0, 1, 1);
	stProcUI[2]->set("%d", orderNum);

	for (int i = 0; i < 3; i++)
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
			orderNum += 10;
		else if (i == 1)
		{
			orderNum -= 10;
			if (orderNum < 0)
				orderNum = 0;
		}
		else// if (i == 2)
		{
			printf("주문 하기 %d\n", orderNum);
			startMake(orderNum);
			orderNum = 0;
		}
		break;

	case iKeyStatMoved:
		for (i = 0; i < 3; i++)
		{
			if (containPoint(point, imgProcUIBtn[i]->touchRect(pop->ep)))
			{
				j = i;
				break;
			}
		}
		if (pop->selected != j)
			printf("snd btn %s\n", j != -1 ? "버튼음" : "취소음");
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

		memset(pd->unitMakeTime, 0x00, sizeof(float) * 10);// 10개라고 가정
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
