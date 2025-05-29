#pragma once

#include "iDefine.h"

#define CX 9 // 이동 불가
#define Inf 0xFFFFFF

struct Path
{
	bool visit;
	int value;
	int* path;
	int pathNum;
};

class iShortestPath
{
public:
	iShortestPath();
	virtual ~iShortestPath();
	void clean();

	void set(uint8* tile, int tileX, int tileY, int tileW, int tileH);
	void run(iPoint start, iPoint end, iPoint* result, int& resultNum);

public:
	uint8* tile;
	int tileX, tileY, tileW, tileH;
	Path* path;
};