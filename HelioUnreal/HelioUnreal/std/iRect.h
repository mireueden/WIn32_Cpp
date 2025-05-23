#pragma once

#include "iPoint.h"
#include "iSize.h"

struct iRect
{
	iPoint origin;
	iSize size;

	iRect operator +=(const iPoint& p);
	iRect operator +=(float f);

};

iRect iRectMake(float x, float y, float width, float height);

bool containPoint(iPoint p, iRect rt);
bool containRect(iRect rt0, iRect rt1);


float getDegree(float start, float end, float rate);