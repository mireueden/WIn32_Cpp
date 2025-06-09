#pragma once

#include "iDefine.h"

class iGraphics
{
private:
	iGraphics();
public:
	virtual ~iGraphics();
	static iGraphics* share();

	void init(int width, int height);
	Texture* getTexture();
	void clean();
	void clear();
	static void clear(Graphics* graphics, float r, float g, float b, float a);

	void drawLine(float x0, float y0, float x1, float y1);
	void drawLine(iPoint p0, iPoint p1);

	void drawRect(float x, float y, float width, float height);
	void drawRect(iRect rt);
	void fillRect(float x, float y, float width, float height);
	void fillRect(iRect rt);

	void drawImage(Texture* tex, float x, float y, int anc);
	void drawImage(Texture* tex, float x, float y,
		int sx, int sy, int sw, int sh,
		float rateX, float rateY,
		int xyz, float degree, int anc, int reverse = REVERSE_NONE);

	// in func.
	void drawString(float x, float y, int anc, const char* szFormat, ...);
	// extern func.
	static void drawString(Graphics* graphics, float x, float y, int anc, const char* szFormat, ...);
	// for rectOfString
	static void drawString(Graphics* graphics, float x, float y, const char* szFormat, ...);


public:
	Bitmap* bmp;
	Graphics* g;
};