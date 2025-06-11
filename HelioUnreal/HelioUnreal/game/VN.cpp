#include "VN.h"

ScriptMgt* sm;
void methodSM(char** line, int lineNum,
	int pageIndex, int pageNum,
	const char* stringName, float stringSize);

void loadVN()
{
	sm = new ScriptMgt(methodSM);
	int lineWidth = 150;
	int linesOfPage = 3;
	sm->set(ws, 10,
		"assets/CookieRun_Regular.ttf", 25,
		//"assets/CRRegular.ttf", 25,
		lineWidth, linesOfPage, 0.1f);
}

void freeVN()
{
	delete sm;
}

iStrTex** stVN = NULL;

void methodSM(char** line, int lineNum, int pageIndex, int pageNum, const char* stringName, float stringSize)
{
	setRGBA(1, 0, 0, 1);
	float x = 0, y = 0;
	fillRect(x, y, 150, 120);
	setRGBA(1, 1, 1, 1);

	if (stVN == NULL)
	{
		stVN = new iStrTex * [4];
		for (int i = 0; i < 4; i++)
			stVN[i] = new iStrTex(NULL);
	}

	setStringName(stringName);
	setStringSize(stringSize);
	setStringRGBA(1, 1, 1, 1);

	for (int i = 0; i < lineNum; i++)
		stVN[i]->paint(x, y + 40 * i, TOP | LEFT, line[i]);

	stVN[lineNum]->paint(x, y + 40 * lineNum, TOP | LEFT, "%d/%d",
		pageIndex + 1, pageNum);
}

void drawVN(float dt)
{
	setRGBA(0, 0, 0, 0);
	clear();

	sm->paint(dt);
}

void keyVN(iKeyStat stat, iPoint point)
{
	if (stat == iKeyStatBegan)
	{
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

	// ani Total

	aniDt = 0.0f;
	aniIndex = 0;
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
	else // if (pageIndex == pageNum)
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
	 
	// s, sNum;
	// aniStr, tmp;


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
	void copyLine(char** src, int srcNum, char** dst, int& dstNum, int len);

	int aniNum;
	copyLine(s, sNum, aniStr, aniNum, aniIndex);
	//printf("============================= (%d, %d)\n", aniIndex, aniNum);
	//for (int i = 0; i < aniNum; i++)
	//	printf("aniStr[%d] = %s\n", i, aniStr[i]);
	method(aniStr, aniNum, pageIndex, pageNum, stringName, stringSize);
}

void copyLine(char** src, int srcNum, char** dst, int& dstNum, int len)
{
	int m = 0, n = 0;
	for (int i = 0; i < len; i++)
	{
		int cpy = iString::isUTF8(&src[m][n]) ? 3 : 1;
		memcpy(&dst[m][n], &src[m][n], cpy);
		dst[m][n + cpy] = 0;// <<< end of string
		n += cpy;
		if (src[m][n] == 0)
		{
			m++;
			n = 0;
		}
	}
	// dstNum = m; 일때 한 줄만 계속 출력
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