#include "Game.h"

#include "iDefine.h"
#include "Lotto.h"
#include "Tripple.h"
#include "AirShooting.h"
#include "Memory.h"
#include "PS.h"
#include "Animating.h"
#include "Oops.h"
#include "Comp.h"
#include "ImageText.h"
#include "VN.h"

ParticleSystem* ps;

void testGame();

iImage** imgBtn;
int selectedBtn;

#include "CtrlImage.h"
Texture* texBg;
Texture* texMirror;

static METHOD_VOID methodFree;
static METHOD_FLOAT methodDraw;
static METHOD_KEY methodKey;


void loadGame()
{

	texBg = createImageFilter("assets/download0.png");
	setImageFilter(imageFilterMirror);
	texMirror = createImageFilter("assets/download0.png");

	loadImageText();

#if 1
	METHOD_VOID mLoad[] = { loadVN, loadComp, loadOops, loadAnimating, loadLotto, loadTripple, loadMemory };
	METHOD_VOID mFree[] = { freeVN, freeComp, freeOops, freeAnimating, freeLotto, freeTripple, freeMemory };
	METHOD_FLOAT mDraw[] = { drawVN, drawComp, drawOops, drawAnimating, drawLotto, drawTripple, drawMemory };
	METHOD_KEY mKey[] = { keyVN, keyComp, keyOops, keyAnimating, keyLotto, keyTripple, keyMemory };
	int runIndex = 0;// !!!!!!!!!!!!!!!!!!!!!!!!!!!
	mLoad[runIndex]();
	methodFree = mFree[runIndex];
	methodDraw = mDraw[runIndex];
	methodKey = mKey[runIndex];


#elif 0
	ps = new ParticleSystem();
	ps->save("test.ptc");
	//ps = new ParticleSystem("test.ptc");
#endif

	struct BtnInfo
	{
		iSize size;
		char str[32];
		iColor4f cB, cS;
	};
	BtnInfo btnInfo[3] = {
		{{100, 30}, "Hello", {0, 1, 0, 1}, {1, 0, 0, 1}},
		{{150, 35}, "World", {0, 0, 1, 1}, {0, 1, 0, 1}},
		{{120, 32}, "Hi", {1, 0, 0, 1}, {1, 1, 1, 1}},
	};
	iGraphics* g = iGraphics::share();

	imgBtn = new iImage * [3];
	for (int i = 0; i < 3; i++)
	{
		iImage* img = new iImage();
		img->position = iPointMake(10, 10 + 40 * i);

		BtnInfo* bi = &btnInfo[i];

		for (int j = 0; j < 2; j++)
		{
			g->init(bi->size.width, bi->size.height);

			setRGBA(bi->cB.r * (1.0f - 0.5f * j),
					bi->cB.g * (1.0f - 0.5f * j),
					bi->cB.b * (1.0f - 0.5f * j),
					bi->cB.a);

			g->fillRect(0, 0, bi->size.width, bi->size.height);

			setRGBA(bi->cS.r, bi->cS.g, bi->cS.b, bi->cS.a);
			g->drawString(3, 3, TOP | LEFT, bi->str);

		Texture*  tex = g->getTexture();
		img->add(tex);
		freeImage(tex);
		}
		imgBtn[i] = img;
	}
	selectedBtn = -1;
}

void freeGame()
{
	freeImage(texBg);
	freeImage(texMirror);


	freeImageText();

#if 1
	methodFree();
	return;
#elif 0
	delete ps;
#endif
	for (int i = 0; i < 3; i++)
		delete(imgBtn[i]);
	delete imgBtn;
}


void drawGame(float dt)
{
	setRGBA(0, 0, 0, 1);
	clear();

	setRGBA(1, 1, 1, 1);
	drawImage(texBg, 0, 200, BOTTOM | LEFT);
	drawImage(texMirror, 0, 200, TOP | LEFT);
	
	drawImageText(dt);
#if 0
	static iStrTex* st = new iStrTex();
	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	st->paint(200, 200, TOP | LEFT, "안녕하세요");


#elif 0
	static float delta = 0.0f;
	delta += dt;
	iPoint off = iPointMake(50 * sin(delta), 0);

	for (int i = 0; i < 3; i++)
	{
		imgBtn[i]->index = (selectedBtn == i);
		imgBtn[i]->paint(dt, off);
	}
#endif	

#if 1
	methodDraw(dt);
	return;
#elif 0
	ps->paint(dt, iPointMake(DEV_WIDTH / 2, DEV_HEIGHT / 2));
	return;
#endif
}



void KeyGame(iKeyStat stat, iPoint point)
{
#if 1
	methodKey(stat, point);
	return;
#endif

	if (stat == iKeyStatBegan)
	{
		
	}
	else if (stat == iKeyStatMoved)
	{
		int j = -1;
		for (int i = 0; i < 3; i++)
		{

			if (containPoint(point, imgBtn[i]->touchRect()))
			{
				j = i;
				break;
			}
		}
		selectedBtn = j;
	}
	else if (stat == iKeyStatEnded)
	{

	}
}

void testGame()
{
#if 1
	struct Score
	{
		char name[32];
		int kor, eng, math;
	};

	Score score[3] = {
		{"조성목", 100, 100, 100},
		{"임수아", 100, 99, 98},
		{"김성민", 0, 100, 100},
	};
	//saveFile((char*)score, sizeof(score), "test.sav");
	//int len = sizeof(score);
	//char* t = new char[len];
	//for (int i = 0; i < len; i++)
	//	t[i] = ((char*)score)[i] + i % 128;
	//saveFile(t, len, "test.sav");

	//Score* s = (Score*)loadFile("test.sav");
	int len;
	char* t = loadFile(len, "test.sav");
	for (int i = 0; i < len; i++)
		t[i] -= i % 128;
	Score* s = (Score*)t;

	for (int i = 0; i < 3; i++)
		printf("[%s] %d %d %d\n",
			s[i].name, s[i].kor, s[i].eng, s[i].math);
#else

#pragma pack(push, 1)
	struct Score
	{
		char kor;// 1
		//char d0[3];
		int eng;// 4
		short math;// 2
		//char d1[2];
	};
#pragma pack(pop)

	printf("%d\n", sizeof(Score));
#endif
}
