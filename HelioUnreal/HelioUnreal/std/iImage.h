#pragma once

#include "iDefine.h"

#include "iStd.h"

class iImage;
typedef void (*MethodImage)(iImage* img);

class iImage
{
public:
	iImage();
	virtual ~iImage();

	static void cb(void* data);
	void add(Texture* tex);
	void paint(float dt, iPoint position);

	void startAnimation(MethodImage cb);

	iRect touchRect();
public:
	iArray* array;
	MethodImage method;
	bool animation;
	float aniDt, _aniDt;
	int index;
	iPoint position;
	Texture* tex;// ref
};