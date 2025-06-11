#pragma once

#include "iStd.h"

void loadVN();
void freeVN();
void drawVN(float dt);
void keyVN(iKeyStat stat, iPoint point);

struct WhoSay
{
	const char* who;
	const char* say;
};

typedef void (*MethodSM)(char** line, int lineNum, 
	int pageIndex, int pageNum,
	const char* stringName, float stringSize);

struct ScriptMgt
{
	ScriptMgt(MethodSM method);
	virtual ~ScriptMgt();

	void set(WhoSay* ws, int wsNum,
		const char* stringName, float stringSize,
		int lineWidth, int linesOfPage, float aniDt = 0.1);
	void clean();
	void load(const char* say);
	bool nextPage(); // pageIndex++
	void paint(float dt);
private:
	bool nextWs(); // wsIndex++;
public:
	MethodSM method;

	// set
	WhoSay* ws;
	char* stringName;
	float stringSize;
	int wsNum, wsIndex;
	int lineWidth, linesOfPage;

	// load
	char*** pageStr; 
	int pageIndex, pageNum;

	// animation
	char** aniStr;
	float aniDt, _aniDt;
	int aniIndex, aniTotal;
};

extern WhoSay ws[10];