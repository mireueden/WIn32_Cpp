#include "DTProcPop.h"

#include "DTObject.h"

float dtToast;

void loadDTProcPop()
{
	createPopCaption();
	createPopUnit();

	dtToast = 0.0f;
}

void freeDTProcPop()
{
	freePopCaption();
	freePopUnit();
}

void drawDTProcPop(float dt)
{
	setRGBA(1, 1, 1, 1);
	drawPopCaption(dt);
	drawPopUnit(dt);
}

bool keyDTProcPop(iKeyStat stat, iPoint point)
{
	if (keyPopUnit(stat, point))
		return true;

	return false;
}



void showToast(iPoint center, const char* szFormat, ...)
{
	if (szFormat == NULL)
	{
		showPopCaption(false);
		return;
	}

	char szText[512];
	va_start_end(szFormat, szText);

	if (strcmp(stCaption->str, szText) == 0)
		return;

	dtToast += iFPS::share()->delta;
	printf("dtToast = %f\n", dtToast);
	if (dtToast < 1.0f)
		return;
	dtToast = 0.0f;
	showPopCaption(true, center, szText);
}

// ==========================================
// popCaption
// ==========================================
iPopup* popCaption;
iStrTex* stCaption;
Texture* methodStCaption(const char* str);

void createPopCaption()
{
	iPopup* pop = new iPopup();

	iStrTex* st = new iStrTex(methodStCaption);
	st->set("0");
	iImage* img = new iImage();
	img->add(st->tex);
	pop->add(img);
	stCaption = st;

	pop->style = iPopupStyleAlpha;
	popCaption = pop;
}
void freePopCaption()
{
	delete popCaption;
	delete stCaption;
}

Texture* methodStCaption(const char* str)
{
	setStringSize(20);
	iRect rt = rectOfString(str);
	rt.size += iSizeMake(10, 10);

	iGraphics* g = iGraphics::share();
	g->init(rt.size.width, rt.size.height);

	setRGBA(1, 1, 1, 1);
	g->fillRect(0, 0, rt.size.width, rt.size.height);

	setStringRGBA(0, 0, 0, 1);
	g->drawString(rt.size.width / 2, rt.size.height / 2, VCENTER | HCENTER, str);

	Texture* tex = g->getTexture();
	g->clean();
	return tex;
}

void drawPopCaption(float dt)
{
	popCaption->paint(dt);
}
void showPopCaption(bool show, iPoint center, const char* str)
{
	if (show)
	{
		stCaption->set(str);
		Texture* t = stCaption->tex;
		//iPoint p = center - iPointMake(t->width / 2, t->height / 2);
		iPoint p = center + iPointMake(0, 20);
		if (p.x < 10)									p.x = 10;
		else if (p.x + t->width > devSize.width - 10)	p.x = devSize.width - 10 - t->width;
		if (p.y < 10)									p.y = 10;
		else if (p.y + t->height > devSize.height - 10)	p.y = devSize.height - 10 - t->height;
		popCaption->sp = p;
		popCaption->ep = p;
	}
	else
	{
		//stCaption->str = NULL;// 힙메모리 할당한 주소 잃어버림 => strcmp
		stCaption->str[0] = 0;// 힙메모리 할당 유지(버퍼크기 가짐) + 문자열 길이0
	}
	popCaption->show(show);
}

// ==========================================
// popUnit
// ==========================================
iPopup** popUnit;
iImage** imgUnitBtn;

iStrTex** stUnitBg;
Texture* methodStUnitBg(const char* s);

int* layerUnit;

void createPopUnit()
{
	popUnit = new iPopup * [9];
	stUnitBg = new iStrTex * [9];
	imgUnitBtn = new iImage * [9];
	iImage* imgBtn = NULL;
	for (int i = 0; i < 9; i++)
	{
		iPopup* pop = new iPopup();
		//
		// bg
		//
		iImage* img = new iImage();
		iStrTex* st = new iStrTex(methodStUnitBg);
		st->set("%d\n%d", i, 0);
		img->add(st->tex);
		pop->add(img);
		stUnitBg[i] = st;

		// 
		// btn
		//
		if (imgBtn == NULL)
		{
			img = new iImage();
			iGraphics* g = iGraphics::share();
			iSize size = iSizeMake(20, 20);
			for (int j = 0; j < 2; j++)
			{
				g->init(size.width, size.height);

				setStringSize(15);
				setStringRGBA(1, 1, 1, 1);
				g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "X");

				Texture* tex = g->getTexture();
				img->add(tex);
				freeImage(tex);
			}
			imgBtn = img;
		}
		img = imgBtn->copy();
		img->position = iPointZero;
		pop->add(img);
		imgUnitBtn[i] = img;

		pop->style = iPopupStyleZoom;
		pop->sp = unit[i]->position;
		pop->ep = iPointMake((devSize.width - 200) / 2, (devSize.height - 200) / 2);

		popUnit[i] = pop;
	}
	delete imgBtn;

	layerUnit = new int[9];
	for (int i = 0; i < 9; i++)
		layerUnit[i] = i;
}

void freePopUnit()
{
	for (int i = 0; i < 9; i++)
	{
		delete popUnit[i];
		delete stUnitBg[i];
	}
	delete popUnit;
	delete stUnitBg;

	delete imgUnitBtn;

	delete layerUnit;
}

Texture* methodStUnitBg(const char* s)
{
	int lineNum;
	char** line = iString::split(lineNum, s);
	int unitIndex = atoi(line[0]);
	int exe = atoi(line[1]);
	iString::free(line, lineNum);

	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(200, 200);
	g->init(size.width, size.height);

	setRGBA(1, 1, 1, 0.7f);
	g->fillRect(0, 0, size.width, size.height);

	setStringSize(20);
	setStringRGBA(0, 0, 0, 1);
	g->drawString(10, 10, TOP | LEFT, "Unit Index %d", unitIndex);

	const char* str[9] = {
		"생산 개수", "생산 개수", "생산 개수", "생산 개수",
		"옮긴 횟수", "옮긴 횟수",
		"출고 횟수", "폐기 횟수", "폐기 횟수"
	};
	g->drawString(10, 40, TOP | LEFT, "%s : %d", str[unitIndex], exe);

	Texture* tex = g->getTexture();
	g->clean();
	return tex;
}

void drawPopUnit(float dt)
{
	for (int j = 0; j < 9; j++)
	{
		int i = layerUnit[j];

		if (popUnit[i]->bShow)
			stUnitBg[i]->set("%d\n%d", i, unit[i]->exe);

		imgUnitBtn[i]->index = (popUnit[i]->selected == 0);
		popUnit[i]->paint(dt);
	}
}

int selectedPop = -1;
iPoint prevPop;

bool keyPopUnit(iKeyStat stat, iPoint point)
{
	iPopup* pop;
	int i = 0, j = -1;

	switch (stat) {

	case iKeyStatBegan:
		for (j = 0; j < 9; j++)
		{
			i = layerUnit[8 - j];

			pop = popUnit[i];
			if (pop->selected == 0)
			{
				showPopUnit(false, i);
				pop->selected = -1;
				break;
			}

			if (pop->bShow == false ||
				pop->stat != iPopupStatProc)
				continue;

			iRect rt;
			rt.origin = pop->ep;
			rt.size = iSizeMake(stUnitBg[i]->tex->width,
				stUnitBg[i]->tex->height);
			if (containPoint(point, rt))
			{
				//int t = layerUnit[8 - j];
				for (int k = 8 - j + 1; k < 9; k++)
					layerUnit[k - 1] = layerUnit[k];
				layerUnit[8] = i;//t;

				selectedPop = i;//t;
				prevPop = point;
				return true;//break;
			}
		}
		break;

	case iKeyStatMoved:
		if (selectedPop != -1)
		{
			iPoint mp = point - prevPop;
			prevPop = point;
			popUnit[selectedPop]->ep += mp;
			return true;//break;
		}

		for (int i = 0; i < 9; i++)
		{
			iPopup* pop = popUnit[i];
			if (pop->bShow == false ||
				pop->stat != iPopupStatProc)
				continue;

			j = -1;
			if (containPoint(point, imgUnitBtn[i]->touchRect(pop->ep)))
			{
				j = 0;
			}
			pop->selected = j;
			if (j == 0)
				break;
		}
		break;

	case iKeyStatEnded:
		if (selectedPop != -1)
		{
			selectedPop = -1;
			return true;//break;
		}
		break;
	}

	return false;
}

void showPopUnit(bool show, int unitIndex)
{
	popUnit[unitIndex]->show(show);
}




