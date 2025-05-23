// HelioUnreal.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "iSize.h"

iSize& iSize::operator +=(const iPoint& p)
{
	width += p.x;
	height += p.y;
	return *this;
}
iSize& iSize::operator+=(const iSize& s)
{
	width += s.width;
	height += s.height;
	return *this;
}

iSize& iSize::operator +=(float f)
{
	width += f;
	height += f;

	return *this;
}

iSize iSizeMake(float width, float height)
{
	iSize s;
	s.height = height;
	s.width = width;
	return s;
}
