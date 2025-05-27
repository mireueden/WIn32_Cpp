#pragma once


#include "iDefine.h"
#include "iArray.h"
#include "iPoint.h"
#include "iStd.h"

class iPopup
{
public:
	iPopup();
	virtual ~iPopup();

	static void cbArray(void* data);

	void add(iImage img);

	void paint(float dt, iPoint position);

	void show(bool show);


public:
	iArray* array;
};

