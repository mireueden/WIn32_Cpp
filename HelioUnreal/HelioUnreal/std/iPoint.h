#pragma once

#include <math.h>

struct iPoint
{
	float x, y;

	bool operator ==(const iPoint& p);
	bool operator !=(const iPoint& p);

	iPoint& operator +=(const iPoint &p);
	iPoint& operator -=(const iPoint &p);

	iPoint operator -(const iPoint &p);
	iPoint operator +(const iPoint &p);

	iPoint operator *(float i);
	iPoint operator /(float i);
	
	float length();
	void loadIdentity();
};

iPoint iPointMake(float x, float y);
#define iPointZero iPointMake(0, 0)
void iPointAbs(iPoint& p);

float iPointLength(const iPoint& p);
float ipointDot(const iPoint& p0, const iPoint& p1);

