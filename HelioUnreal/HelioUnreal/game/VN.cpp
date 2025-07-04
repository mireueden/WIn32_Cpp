#include "VN.h"

ScriptMgt* sm;
void methodSM(char** line, int lineNum,
	int pageIndex, int pageNum,
	const char* stringName, float stringSize);

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>


static Texture* texBg;
iPoint offBg;

Texture** texWho;

DicWho* dw;
Texture* texSay;


void loadVN()
{
	WhoSay* whoSay = ws;
	int whoSayNum = 10;
	// File IO
#if 0
	ScriptMgt::save("assets/ws.scr", ws, 10);
#elif 0
	int whoSayNum;
	WhoSay* whoSay = ScriptMgt::call("assets/ws.scr", whoSayNum);
#endif

	sm = new ScriptMgt(methodSM);
	int lineWidth = devSize.width - 60;
	int linesOfPage = 2;
	sm->set(whoSay, whoSayNum,
		"assets/CRRegular.ttf", 30,
		lineWidth, linesOfPage, 0.1f);

	loadDicWho();

	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(devSize.width - 40, 100);
	g->init(size.width, size.height);

	setRGBA(0, 1, 1, 1);
	g->fillRect(0, 0, size.width, size.height);

	setRGBA(1, 1, 1, 1);
	g->drawRect(3, 3, size.width - 6, size.height - 6);

	texSay = g->getTexture();
	g->clean();

	createPopTest();

}

void freeVN()
{
	delete sm;

	freeDicWho();

	freePopTest();
}

iStrTex** stVN = NULL;

void methodSM(char** line, int lineNum,
	int pageIndex, int pageNum,
	const char* stringName, float stringSize)
{
	setRGBA(0, 0, 1, 1);
	fillRect(0, 0, devSize.width, devSize.height);

	if (stVN == NULL)
	{
		stVN = new iStrTex * [4];
		for (int i = 0; i < 4; i++)
			stVN[i] = new iStrTex(NULL);
	}

	setStringName(stringName);
	setStringSize(stringSize);
	setStringRGBA(1, 1, 1, 1);

#if 0
	setRGBA(1, 0, 0, 1);
	float x = 0, y = 0;
	fillRect(x, y, 150, 120);
	setRGBA(1, 1, 1, 1);
#else
	iPoint p = iPointMake(
		(devSize.width - texSay->width) / 2,
		devSize.height - texSay->height - 10);
	setRGBA(1, 1, 1, 1);
	drawImage(texSay, p.x, p.y, TOP | LEFT);
#endif 
	setStringRGBA(0, 0, 0, 1);
	for (int i = 0; i < lineNum; i++)
		stVN[i]->paint(p.x + 10, p.y + 10 + 40 * i, TOP | LEFT, line[i]);

	setStringRGBA(1, 1, 1, 1);
	stVN[lineNum]->paint(p.x, p.y - 40, TOP | LEFT, "%d/%d",
		pageIndex + 1, pageNum);
}

void drawVN(float dt)
{
	setRGBA(0, 0, 0, 0);
	clear();

	// 케릭터 얼굴이 나올때 dt = 0.0f
	// 말풍선 애니메이션 dt = 0.0f;
	sm->paint(dt);

	//for (int i = 0; i < 10; i++)
	//	drawImage(texWho[i], 10 + 80 * (i%5), 20 + 150 * (i/5), TOP | LEFT);
	drawPopTest(dt);
}

void keyVN(iKeyStat stat, iPoint point)
{
	if (keyPopTest(stat, point))
		return;

	if (stat == iKeyStatBegan)
	{
		showPopTest(true);
		return;
		sm->nextPage();
	}
}

WhoSay ws[10]
{
	{"린프야림", "안녕 Test0 Hello 테스트1 Hello\nTest2 안녕\n테스트3 Hello Test4 Hello Test5 Hello Test6"},
	{"린프야림", "사냥언제함Mm "},
	{"해룸잉", "삼성공대 왜그럼? 가뮤 사슴밥이나 줘라"},
	{"알귤", "달달한거 땡긴다"},
	{"리갼", "아오 악질들아 대구 무시하지마라"},
	{"폴딱", "난 악질 아니라니까?"},
	{"토끼아래", "서진핑 @리갼@리갼@리갼@리갼@리갼@리갼"},
	{"하애꼬", "그걸 줄여 말하면 어떡함;"},
	{"레드", "나 바빠요.."},
	{"미르", "챌섭 ㄲ?"},

};

ScriptMgt::ScriptMgt(MethodSM method)
{
	this->method = method;
	pageStr = NULL;
	pageIndex = 0, pageNum = 0;
	linesOfPage = 0;
}

ScriptMgt::~ScriptMgt()
{
	clean();
}

void ScriptMgt::save(const char* path, WhoSay* ws, int wsNum)
{
	FILE* pf = fopen(path, "wb");

	fwrite(&wsNum, sizeof(int), 1, pf);
	for (int i = 0; i < wsNum; i++)
	{
		const char* t = ws[i].who;
		int len = strlen(t);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(t, sizeof(char), len, pf);

		t = ws[i].say;
		len = strlen(t);
		fwrite(&len, sizeof(int), 1, pf);
		fwrite(t, sizeof(char), len, pf);
	}

	fclose(pf);
}

WhoSay* ScriptMgt::call(const char* path, int& wsNum)
{
	FILE* pf = fopen(path, "rb");

	int num;
	fread(&num, sizeof(int), 1, pf);
	WhoSay* ws = new WhoSay[num];
	for (int i = 0; i < num; i++)
	{
		int len;
		fread(&len, sizeof(int), 1, pf);
		char* t = new char[len + 1];
		fread(t, sizeof(char), len, pf);
		t[len] = 0;
		ws[i].who = t;

		fread(&len, sizeof(int), 1, pf);
		t = new char[len + 1];
		fread(t, sizeof(char), len, pf);
		t[len] = 0;
		ws[i].say = t;
	}

	fclose(pf);

	wsNum = num;
	return ws;
}

void ScriptMgt::clean()
{
	if (pageStr == NULL)
		return;

	for (int i = 0; i < pageNum; i++)
	{
		for (int j = 0; j < linesOfPage; j++)
		{
			if (pageStr[i][j])
				delete pageStr[i][j];
		}
		delete pageStr[i];
	}
	delete pageStr;
	pageStr = NULL;
}

void ScriptMgt::set(WhoSay* ws, int wsNum,
	const char* sn, float ss,
	int lw, int lop, float aDt)
{
	this->ws = ws;
	this->wsNum = wsNum;
	wsIndex = 0;
	stringName = iString::copy(sn);
	stringSize = ss;
	lineWidth = lw;
	linesOfPage = lop;
	aniStr = new char* [lop];
	for (int i = 0; i < lop; i++)
		aniStr[i] = new char[512];
	_aniDt = aDt;
	aniDt = 0.0f;
	aniIndex = 0;
	load(ws[0].say);
}

void ScriptMgt::load(const char* say)
{
	char* bkSN = iString::copy(getStringName());
	float bkSS = getStringSize();
	setStringName(stringName);
	setStringSize(stringSize);

	char t[100];

	iArray* array = new iArray();

	int lineNum;
	char** line = iString::split(lineNum, say);// '\n'
	for (int i = 0; i < lineNum; i++)
	{
		int lNum;
		char** l = iString::splitWidth(lNum, line[i], lineWidth);// width
		for (int j = 0; j < lNum; j++)
			array->add(l[j]);
		delete l;
		delete line[i];
	}
	delete line;

	for (int i = 0; i < array->count; i++)
	{
		const char* s = (char*)array->at(i);
		printf("line[%d] = %s\n", i, s);
	}

	// arrayLine->count
	pageNum = 1 + (array->count - 1) / linesOfPage;
	pageIndex = 0;

	pageStr = new char** [pageNum];
	for (int i = 0; i < pageNum; i++)
	{
		char** t = new char* [linesOfPage];
		memset(t, 0x00, sizeof(char*) * linesOfPage);

		pageStr[i] = t;
	}
	for (int i = 0; i < array->count; i++)
		pageStr[i / linesOfPage][i % linesOfPage] = (char*)array->at(i);
	delete array;

	setStringName(bkSN);
	setStringSize(bkSS);
}

bool ScriptMgt::nextWs()
{
	wsIndex++;
	if (wsIndex == wsNum)
		return false;

	clean();
	load(ws[wsIndex].say);
	return true;
}

bool ScriptMgt::nextPage()
{
	pageIndex++;
	if (pageIndex < pageNum)
	{
		if (aniIndex < aniTotal)
		{
			// skip
			aniIndex = aniTotal;
		}
		else
		{
			// next page
			aniIndex = 0;
			aniDt = 0.0f;
		}
	}
	else// if (pageIndex == pageNum)
	{
		return nextWs();
	}
	return true;
}



void ScriptMgt::paint(float dt)
{
	if (pageIndex == pageNum)
		return;

	char** s = pageStr[pageIndex];
	int sNum = linesOfPage;
	for (int i = 0; i < linesOfPage; i++)
	{
		if (s[i] == NULL)
		{
			sNum = i;
			break;
		}
	}
	 
	// aniTotal
	int total = 0;
	for (int i = 0; i < sNum; i++)
	{
		char* t = s[i];
		for (int j = 0; t[j]; )
		{
			total++;

			j += (iString::isUTF8(&t[j]) ? 3 : 1);
		}
	}
	aniTotal = total;

	// aniIndex
	aniDt += dt;
	while (aniDt >= _aniDt)
	{
		aniDt -= _aniDt;
		aniIndex++;
		if (aniIndex > aniTotal)
			aniIndex = aniTotal;
	}
	//method(s, sNum, pageIndex, pageNum, stringName, stringSize);

	// aniTotal, aniIndex 고려해서 aniStr입력, aniStrNum
	void copyLine(	char** src, int srcNum,
					char** dst, int& dstNum, int len);
	
	int aniNum;
	for (int i = 0; i < linesOfPage; i++)
		memset(aniStr[i], 0x00, sizeof(char) * 512);
	copyLine(s, sNum, aniStr, aniNum, aniIndex);
	//printf("============================= (%d, %d)\n", aniIndex, aniNum);
	//for (int i = 0; i < aniNum; i++)
	//	printf("aniStr[%d] = %s\n", i, aniStr[i]);
	method(aniStr, aniNum, pageIndex, pageNum, stringName, stringSize);
}

void copyLine(char** src, int srcNum,
	char** dst, int& dstNum, int len)
{
	int m = 0, n = 0;
	for (int i = 0; i < len; i++)
	{
		int cpy = iString::isUTF8(&src[m][n]) ? 3 : 1;
		memcpy(&dst[m][n], &src[m][n], cpy);
		//dst[m][n + cpy] = 0;// <<< end of string
		n += cpy;
		if (src[m][n] == 0)
		{
			m++;
			n = 0;
		}
	}
	dstNum = m + 1;
	if (dstNum > srcNum)
		dstNum = srcNum;
}


// total 19 1 ~ 19
// src[2][20];
// Hello World
// My World

// index 2
// dst[2][20];
// dst[0][0~1] = src[0][0~1];



void loadDicWho()
{
	const char* strKey[10] = {
		"해룸잉", "린프야림", "폴딱", "리갼", "알귤",
		"미르", "삔머", "하애꼬", "토끼아래", "레드"
	};
	Texture** texs = createImage(5, 2, "assets/who.jpg");
	dw = new DicWho[10];
	for (int i = 0; i < 10; i++)
	{
		DicWho* d = &dw[i];
		d->tex = texs[i];
		d->key = iString::copy(strKey[i]);

		Texture* t = d->tex;
		t->width /= 8.;
		t->height /= 8.;
		t->potWidth /= 8.;
		t->potHeight /= 8.;
	}
	delete texs;
}

void freeDicWho()
{
	for (int i = 0; i < 10; i++)
	{
		DicWho* d = &dw[i];
		freeImage(d->tex);
		delete d->key;
	}
	delete dw;
}

Texture* getDicWho(const char* key)
{
	for (int i = 0; i < 10; i++)
	{
		DicWho* d = &dw[i];
		if (strcmp(d->key, key) == 0)
			return d->tex;
	}
	return NULL;
}

// ============================================================
// popTest
// ============================================================

iPopup* popTest;

void openPopTest(iPopup* pop);
void closePopTest(iPopup* pop);
void drawPopTest(iPopup* pop, float rate);

void createPopTest()
{
	iPopup* pop = new iPopup();

	Texture* tex = createImage("assets/download2.jpg");
	iImage* img = new iImage();
	img->add(tex);
	freeImage(tex);
	pop->add(img);

	pop->style = iPopupStyleZoom;
	pop->_aniDt = 0.5f;
	pop->methodOpen = openPopTest;
	pop->methodClose = closePopTest;
	pop->methodDrawBefore = drawPopTest;
	pop->methodDrawAfter = NULL;

	pop->sp = iPointZero;
	pop->ep = iPointMake(100, 100);

	popTest = pop;
}


void freePopTest()
{
	delete popTest;
}

void showPopTest(bool show)
{
	if (show)
	{

	}
	else
	{

	}
	popTest->show(show);
}

void openPopTest(iPopup* pop)
{
	printf("popTest 열림\n");
}

void closePopTest(iPopup* pop)
{
	printf("popTest 닫힘\n");
}

void drawPopTest(iPopup* pop, float rate)
{
	float alpha;
	if (pop->stat == iPopupStatOpen)
	{
		alpha = 0.7f * rate;
	}
	else if (pop->stat == iPopupStatProc)
	{
		alpha = 0.7f;
	}
	else if (pop->stat == iPopupStatClose)
	{
		alpha = 0.7f * (1.0f - rate);
	}

	setRGBA(0, 0, 0, alpha);
	fillRect(0, 0, devSize.width, devSize.height);
	setRGBA(1, 1, 1, 1);
}

void drawPopTest(float dt)
{
	popTest->paint(dt);
}

bool keyPopTest(iKeyStat stat, iPoint point)
{
	if (popTest->bShow == false)
		return false;
	if (popTest->stat != iPopupStatProc)
		return true;

	switch (stat) {
	case iKeyStatBegan:
		showPopTest(false);
		break;
	case iKeyStatMoved:
		break;
	case iKeyStatEnded:
		break;
	}
	return true;
}