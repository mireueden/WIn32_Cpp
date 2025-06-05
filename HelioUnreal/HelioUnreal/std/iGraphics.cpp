#include "iGraphics.h"

#include "iStd.h"

iGraphics::iGraphics()
{
	bmp = NULL;
	g = NULL;
}


iGraphics::~iGraphics()
{
	clean();
}

iGraphics* iGraphics::share()
{
	static iGraphics* g = new iGraphics();
	return g;
}

void iGraphics::init(int width, int height)
{
	clean();

	bmp = new Bitmap(width, height);
	g = Graphics::FromImage(bmp);
}



Texture* iGraphics::getTexture()
{
	if (bmp == NULL)
		return NULL;

	int width, height;
	uint8* rgba = bmp2rgba(bmp, width, height);
	clean();

	Texture* tex = createImageWithRGBA(rgba, width, height);
	delete rgba;

	return tex;
}

void iGraphics::clean()
{
	if (bmp)
	{
		delete bmp;
		bmp = NULL;
	}
	if (g)
	{
		delete g;
		g = NULL;
	}
}

void iGraphics::drawLine(float x0, float y0, float x1, float y1)
{
#if 0
	Graphics* bk = getGraphics();
	setGraphics(g);
	::drawLine(x0, y0, x1, y1);
	setGraphics(bk);
#endif
}

void iGraphics::drawLine(iPoint p0, iPoint p1)
{
#if 0
	Graphics* bk = getGraphics();
	setGraphics(g);
	::drawLine(p0, p1);
	setGraphics(bk);
#endif
}

void iGraphics::drawRect(float x, float y, float width, float height)
{
#if 0
	Graphics* bk = getGraphics();
	setGraphics(g);
	::drawRect(x, y, width, height);
	setGraphics(bk);
#endif
}

void iGraphics::drawRect(iRect rt)
{
#if 0
	Graphics* bk = getGraphics();
	setGraphics(g);
	::drawRect(rt);
	setGraphics(bk);
#endif
}

void iGraphics::fillRect(float x, float y, float width, float height)
{
#if 0
	Graphics* bk = getGraphics();
	setGraphics(g);
	::fillRect(x, y, width, height);
	setGraphics(bk);
#endif
}

void iGraphics::fillRect(iRect rt)
{
#if 0
	Graphics* bk = getGraphics();
	setGraphics(g);
	::fillRect(rt);
	setGraphics(bk);
#endif
}

void iGraphics::drawImage(Texture* tex, float x, float y, int anc)
{
#if 0
	Graphics* bk = getGraphics();
	setGraphics(g);
	::drawImage(tex, x, y, anc);
	setGraphics(bk);
#endif
}

void iGraphics::drawImage(Texture* tex, float x, float y, int sx, int sy, int sw, int sh, float rateX, float rateY, int xyz, float degree, int anc, int reverse)
{
#if 0
	Graphics* bk = getGraphics();
	setGraphics(g);
	::drawImage(tex, x, y, sx, sy, sw, sh, rateX, rateY, xyz, degree, anc, reverse);
	setGraphics(bk);
#endif
}

void iGraphics::drawString(float x, float y,int anc, const char* szFormat, ...)
{
#if 0
	char szText[512];
	va_start_end(szFormat, szText);

	Graphics* bk = getGraphics();
	setGraphics(g);
	::drawString(x, y, anc, szText);
	setGraphics(bk);
#endif
}