#pragma once

#include "iDefine.h"
#include "iStd.h"
#include "iColor.h"

typedef Texture* (*MethodSt)(const char* s);

struct StInfo
{
	const char* str;
	iColor4f color;
};

struct iStrTex
{
	iStrTex(MethodSt m = NULL);
	virtual ~iStrTex();

	void clean();
	void set(const char* szFormat, ...);
	void paint(float x, float y, int anc, float rateX, float rateY, 
		int xyz, float  degree, int reverse = REVERSE_NONE);
	void paint(float x, float y, int anc);
	void paint(float x, float y, int anc, const char* szFormat, ...);

	static Texture* createImage(StInfo* si, int siNum, const char* szFormat, ...);

	char* str;
	int strLength;
	Texture* tex;
	MethodSt method;
};