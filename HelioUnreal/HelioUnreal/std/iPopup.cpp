#include "iPopup.h"

#include "iStd.h"

iPopup::iPopup()
{
	array = new iArray(cbArray);

	bShow = false;
	style = iPopupStyleAlpha;
	stat = iPopupStatClose;
	sp = iPointZero;
	ep = iPointZero;
	_aniDt = 0.2f;
	aniDt = 0.0f;
	selected = -1;
}

iPopup::~iPopup()
{
	delete array;
}

void iPopup::cbArray(void* data)
{
	iImage* img = (iImage*)data;
	delete img;
}

void iPopup::add(iImage* img)
{
	array->add(img);
}

void iPopup::paint(float dt)
{
	if (bShow == false)
		return;

	float alpha;
	iPoint p;

	if (style == iPopupStyleAlpha)
	{
		p = ep;
		if (stat == iPopupStatOpen)
		{
			alpha = linear(0.0f, 1.0f, aniDt / _aniDt);
			aniDt += dt;
			if (aniDt >= _aniDt)
				stat = iPopupStatProc;
		}
		else if (stat == iPopupStatProc)
		{
			alpha = 1.0f;
		}
		else if (stat == iPopupStatClose)
		{
			alpha = linear(1.0f, 0.0f, aniDt / _aniDt);
			aniDt += dt;
			if (aniDt >= _aniDt)
				bShow = false;
		}
	}
	else if (style == iPopupStyleMove)
	{
		alpha = 1.0f;
		if (stat == iPopupStatOpen)
		{
			p = linear(sp, ep, aniDt / _aniDt);
			aniDt += dt;
			if (aniDt >= _aniDt)
				stat = iPopupStatProc;
		}
		else if (stat == iPopupStatProc)
		{
			p = ep;
		}
		else if (stat == iPopupStatClose)
		{
			p = linear(ep, sp, aniDt / _aniDt);
			aniDt += dt;
			if (aniDt >= _aniDt)
				bShow = false;
		}
	}


	setRGBA(1, 1, 1, alpha);
	for (int i = 0; i < array->count; i++)
	{
		iImage* img = (iImage*)array->at(i);
		img->paint(dt, p);
	}
	setRGBA(1, 1, 1, 1);
}

void iPopup::show(bool show)
{
	if (show)
	{
		if (bShow)
			return;
		bShow = true;
		stat = iPopupStatOpen;
	}
	else
	{
		if (stat != iPopupStatProc)
			return;
		stat = iPopupStatClose;
	}
	aniDt = 0.0f;
}
