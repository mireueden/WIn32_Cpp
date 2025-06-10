#pragma once

#include "iStd.h"

void loadImageText();
void freeImageText();
void drawImageText(float dt);
void keyImageText(iKeyStat stat, iPoint point);

struct ImageText
{
	ImageText();
	virtual ~ImageText();

	void loadFont(const char* s);
	void loadImage(Texture** texs);
	void paint(float x, float y, int anc, const char* szFormat, ...);


	Texture** texs;
	int interval;
};