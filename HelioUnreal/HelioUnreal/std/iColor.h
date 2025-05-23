#pragma once

#include "iDefine.h"

struct iColor4f;

struct iColor4b
{
	byte r, g, b, a;

	// 파라미터 값 접근 불가능
	// 전처리로 해당 구조체가 있다는 것만 알기 때문
	// iColor4b& operator =(iColor4f& f); 
	iColor4b& operator =(const iColor4f& c);
	iColor4b operator +(const iColor4b c);
	iColor4b operator *(float f);
};

struct iColor4f
{
	float r, g, b, a;

	iColor4f& operator = (const iColor4b& c);
	iColor4f operator +(const iColor4f c);
	iColor4f operator *(float f);
};

iColor4b iColor4bMake(byte r, byte g, byte b, byte a);

iColor4f iColor4fMake(float r, float g, float b, float a);

#define iColor4fRed iColor4fMake(1, 0, 0, 1)
#define iColor4fGreen iColor4fMake(0, 1, 0, 1)
#define iColor4fBlue iColor4fMake(0, 0, 1, 1)
#define iColor4fWhite iColor4fMake(1, 1, 1, 1)
#define iColor4fBlack iColor4fMake(0, 0, 0, 1)