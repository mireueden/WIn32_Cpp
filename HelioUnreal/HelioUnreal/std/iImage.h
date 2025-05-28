#pragma once

#include "iDefine.h"
#include "iPoint.h"
#include "iArray.h"

#define aniDtDefault 0.017

typedef void (*iImageAnimation)(void* data);

class iImage
{
public:
	iImage();
	virtual ~iImage();
	static void cbArray(void* data);

	void add(Texture* tex);

	void paint(float dt, iPoint position);

	void startAnimation(iImageAnimation m = NULL, void* p = NULL);

	iRect touchRect(iPoint position=iPointZero);

public:
	iArray* array;
	Texture* tex;
	int index;
	bool animation;
	float aniDt, _aniDt;
	iPoint position;
	float rate;
	int anc;
	int reverse;

	iImageAnimation method;
	void* parm;
};
