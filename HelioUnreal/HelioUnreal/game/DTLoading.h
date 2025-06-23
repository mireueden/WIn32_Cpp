#pragma once

#include "iStd.h"

enum DTState
{
	DTStateLogin = 0,
	DTStateProc,

	DTStateMax
};

extern DTState ds;

void setLoading(DTState dsNext, METHOD_VOID free, METHOD_VOID load);
void drawDTLoading(float dt);
bool keyDTLoading(iKeyStat stat, iPoint point);
