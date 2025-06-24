#include "DTProcPop.h"

float dtToast;

void loadDTProcPop()
{
	createPopCaption();
	
	dtToast = 0.0f;
}

void freeDTProcPop()
{
	freePopCaption();
}

void drawDTProcPop(float dt)
{
	setRGBA(1, 1, 1, 1);
	drawPopCaption(dt);
}

bool keyDTProcPop(iKeyStat stat, iPoint point)
{
	return false;
}

void showToast(iPoint center, const char* szFormat,...)
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
	if (dtToast < 1.0f)
		return;
	dtToast = 0.0f;
	showPopCaption(true, center, szText);
}

// ================================================================
// popCaption
// ================================================================
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
		iPoint p = center + iPointMake(t->width / 2, t->height / 2);

		if (p.x < 10)									p.x = 10;
		else if (p.x + t->width > devSize.width - 10)	p.x = devSize.width - 10 - t->width;
		if (p.y < 10)									p.y = 10;
		else if (p.y + t->height > devSize.height - 10)	p.x = devSize.height - 10 - t->height;

		popCaption->sp = p;
		popCaption->ep = p;
	}

	popCaption->show(show);
}
