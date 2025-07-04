#include "LemonMenu.h"

#include "Lemon.h"
#include "LemonProc.h"

// =====================================
// Menu(게임설명 + 게임시작)
// =====================================
iStrTex* stExp;
Texture* methodStExp(const char* s);

#define _page 3
int page;

iImage** imgMenuBtn;
iStrTex*** stMenuBtn;
Texture* methodStBtn(const char* s);
int selectedMenu;

void loadLemonMenu()
{
	stExp = new iStrTex(methodStExp);
	stExp->set("%d", page = 0);

	imgMenuBtn = new iImage*[3];
	stMenuBtn = new iStrTex**[3];
	iPoint posBtn[] = { {10, 150}, {590, 150}, { (devSize.width - 300) / 2, 320 }, };
	for (int i = 0; i < 3; i++)
	{
		iImage* img = new iImage();
		stMenuBtn[i] = new iStrTex*[2];
		for (int j = 0; j < 2; j++)
		{
			iStrTex* st = new iStrTex(methodStBtn);
			st->set("%d\n%d", i, j);
			img->add(st->tex);
			stMenuBtn[i][j] = st;
		}
		img->position = posBtn[i];
		imgMenuBtn[i] = img;
	}
	selectedMenu = -1;

}

void freeLemonMenu()
{
	delete stExp;
	for (int i = 0; i < 2; i++)
	{
		delete imgMenuBtn[i];
		for (int j = 0; j < 2; j++)
			delete stMenuBtn[i][j];
		delete stMenuBtn[i];
	}
	delete imgMenuBtn;
	delete stMenuBtn;

}

Texture* methodStExp(const char* s)
{
	int page = atoi(s);

	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(512, 300);
	g->init(size.width, size.height);

	setRGBA(1, 1, 1, 1);
	g->fillRect(0, 0, size.width, size.height);
	if (page == 0)
	{
	}
	else if (page == 1)
	{

	}
	else if (page == 2)
	{

	}
	setStringName("assets/CRRegular.ttf");
	setStringSize(30);
	setStringRGBA(0, 0, 0, 1);
	g->drawString(size.width/2, size.height/2, VCENTER|HCENTER, "%d/%d", page+1, _page);

	Texture* tex = g->getTexture();
	g->clean();
	return tex;
}

Texture* methodStBtn(const char* s)
{
	int lineNum;
	char** line = iString::split(lineNum, s);
	int btnIndex = atoi(line[0]);
	int btnStyle = atoi(line[1]);
	iString::free(line, lineNum);

	iGraphics* g = iGraphics::share();

	setStringName("assets/CRRegular.ttf");
	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);

	if (btnIndex < 2)// 🡀, ➤
	{
		iSize size = iSizeMake(50, 50);
		g->init(size.width, size.height);

		if (btnStyle == 0)
		{
			setRGBA(0.7f, 0.7f, 0.7f, 1.0f);
			g->fillRect(0, 0, size.width, size.height);
		}
		else if (btnStyle == 1)
		{
			setRGBA(0.3f, 0.3f, 0.3f, 1.0f);
			g->fillRect(5, 5, size.width - 10, size.height - 10);
		}
		const char* str[] = { "⮜", "⮞"};
		g->drawString(size.width/2, size.height/2, VCENTER|HCENTER, str[btnIndex]);
	}
	else if (btnIndex == 2)// 게임시작
	{
		iSize size = iSizeMake(300, 50);
		g->init(size.width, size.height);

		if (btnStyle == 0)
		{
			setRGBA(0.7f, 0.7f, 0.7f, 1.0f);
			g->fillRect(0, 0, size.width, size.height);
			setRGBA(1, 1, 1, 1);
			g->drawRect(5, 5, size.width - 10, size.height - 10);
		}
		else
		{
			setRGBA(0.3f, 0.3f, 0.3f, 1.0f);
			g->fillRect(20, 20, size.width-40, size.height-40);
			setRGBA(1, 1, 1, 1);
			g->drawRect(25, 25, size.width - 50, size.height - 50);
		}
		const char* strBtn[] = { "게임시작" };// "크레딧"
		g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[btnIndex-2]);
	}


	Texture* tex = g->getTexture();
	g->clean();
	return tex;
}

void drawLemonMenu(float dt)
{
	setRGBA(0, 0, 0, 0);
	clear();
	setRGBA(1, 1, 1, 1);

	stExp->paint(devSize.width / 2, 10, TOP | HCENTER, "%d", page);

	for (int i = 0; i < 3; i++)
	{
		imgMenuBtn[i]->index = (i == selectedMenu);
		imgMenuBtn[i]->paint(dt, iPointZero);
	}

	//glClearColor(0, 0, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//void drawShadertoy(float dt);
	//drawShadertoy(dt);
}

void keyLemonMenu(iKeyStat stat, iPoint point)
{
	int i, j = -1;

	switch (stat) {
	case iKeyStatBegan:
		if (selectedMenu == -1) break;
		if (selectedMenu == 0)
		{
			page--;
			if (page < 0)
				page += 3;
		}
		else if( selectedMenu==1)
		{
			page = (page + 1) % 3;
		}
		else
		{
			printf("game start~!!\n");
			freeLemonMenu();
			loadLemonProc();
			gameState = GameStateProc;
		}
		break;

	case iKeyStatMoved:
		for (i = 0; i < 3; i++)
		{
			if (containPoint(point, imgMenuBtn[i]->touchRect()))
			{
				j = i;
				break;
			}
		}
		selectedMenu = j;
		break;

	case iKeyStatEnded:
		break;
	}
}

