// HelioUnreal.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "iRect.h"


iRect iRect::operator +=(const iPoint& p)
{
	//origin += p; 다른 오퍼레이터를 쓰는 것이기 때문에 최적화쪽 문제임

	origin.x += p.x;
	origin.y += p.y;
	return *this;
}

iRect iRect::operator +=(float f)
{
	// size += f;
	size.width += f;
	size.height += f;
	return *this;
}


iRect iRectMake(float x, float y, float width, float height)
{
	iRect rt;
	rt.origin.x = x;
	rt.origin.y = y;
	rt.size.width = width;
	rt.size.height = height;

	return rt;
}

bool containPoint(iPoint p, iRect rt)
{
#if 0 // 기본 생각 골조
	if (p.x >= rt.origin.x &&
		p.x <= rt.origin.x + rt.size.width &&
		p.y >= rt.origin.y &&
		p.y <= rt.origin.y + rt.size.height)
		return true;
	return false;
#else // 거꾸로 생각
	if (p.x < rt.origin.x ||
		p.x > rt.origin.x + rt.size.width ||
		p.y < rt.origin.y ||
		p.y > rt.origin.y + rt.size.height)
		return false;
	return true;
#endif
}

bool containRect(iRect rt0, iRect rt1)
{
	if (rt0.origin.x + rt0.size.width <rt1.origin.x ||
		rt0.origin.x > rt1.origin.x + rt1.size.width ||
		rt0.origin.y + rt0.size.width <rt1.origin.y ||
		rt0.origin.y > rt1.origin.y + rt1.size.width)
		return false;
	return true;

}



float getDegree(float start, float end, float rate)
{
	//while (start > 360) start -= 360;
	//while (start < 0) start += 360;
	//while (end > 360) end -= 360;
	//while (end < 0) end += 360;


	float d = end - start;
	while (d > 360) d -= 360;
	while (d < 0) d += 360;
	if (d > 180) d -= 180;
	else if (d < -180) d += 180;
	

	return start + d - end;
}