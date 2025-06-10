#include "VN.h"

ScriptMgt* sm;
void methodSM(char** line, int lineNum, int pageIndex,int pageNum);

void loadVN()
{
	sm = new ScriptMgt(methodSM);
	sm->set(ws, 10, 150, 3);
}

void freeVN()
{
	delete sm;
}

iStrTex** stVN = NULL;

void methodSM(char** line, int lineNum, int pageIndex, int pageNum)
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
	setStringSize(30);
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
	{"린프야림", "Hello Test0 Hello Test1 Hello Test2\nHello Test3\nHello Test4 Hello Test5 Hello Test6"},
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

void ScriptMgt::set(WhoSay* ws, int wsNum, int lw, int lop)
{
	this->ws = ws;
	this->wsNum = wsNum;
	wsIndex = 0;
	lineWidth = lw;
	linesOfPage = lop;
	load(ws[0].say);
}

void ScriptMgt::load(const char* say)
{
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
	int lop = linesOfPage;
	for (int i = 0; i < linesOfPage; i++)
	{
		if (s[i] == NULL)
		{
			lop = i;
			break;
		}
	}

	method(s, lop, pageIndex, pageNum);
}