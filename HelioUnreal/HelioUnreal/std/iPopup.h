#pragma once


#include "iDefine.h"
#include "iArray.h"
#include "iPoint.h"
#include "iStd.h"

enum iPopupStat
{
	iPopupStatOpen = 0,
	iPopupStatProc,
	iPopupStatClose
};

enum iPopupStyle
{
	iPopupStyleAlpha = 0,
	iPopupStyleMove,
};

class iPopup
{
public:
	iPopup();
	virtual ~iPopup();

	static void cbArray(void* data);

	void add(iImage* img);

	void paint(float dt);

	void show(bool show);

public:
	iArray* array;
		
	bool bShow;
	iPopupStyle style;
	iPopupStat stat;
	iPoint sp, ep;
	float aniDt, _aniDt;

	int selected;
};

