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

	Texture* tex = new Texture;
	tex->texID = bmp;
	tex->width = bmp->GetWidth();
	tex->height = bmp->GetHeight();
	tex->potWidth = bmp->GetWidth();
	tex->potHeight = bmp->GetHeight();
	tex->retainCount = 1;

	bmp = NULL;
	clean();

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
	Graphics* bk = getGraphics();
	setGraphics(g);
	::drawLine(x0, y0, x1, y1);
	setGraphics(bk);
}

void iGraphics::drawLine(iPoint p0, iPoint p1)
{
	Graphics* bk = getGraphics();
	setGraphics(g);
	::drawLine(p0, p1);
	setGraphics(bk);
}

void iGraphics::drawRect(float x, float y, float width, float height)
{
	Graphics* bk = getGraphics();
	setGraphics(g);
	::drawRect(x, y, width, height);
	setGraphics(bk);
}

void iGraphics::drawRect(iRect rt)
{
	Graphics* bk = getGraphics();
	setGraphics(g);
	::drawRect(rt);
	setGraphics(bk);
}

void iGraphics::fillRect(float x, float y, float width, float height)
{
	Graphics* bk = getGraphics();
	setGraphics(g);
	::fillRect(x, y, width, height);
	setGraphics(bk);
}

void iGraphics::fillRect(iRect rt)
{
	Graphics* bk = getGraphics();
	setGraphics(g);
	::fillRect(rt);
	setGraphics(bk);
}

void iGraphics::drawImage(Texture* tex, float x, float y, int anc)
{
	Graphics* bk = getGraphics();
	setGraphics(g);
	::drawImage(tex, x, y, anc);
	setGraphics(bk);
}

void iGraphics::drawImage(Texture* tex, float x, float y, int sx, int sy, int sw, int sh, float rateX, float rateY, int xyz, float degree, int anc, int reverse)
{
	Graphics* bk = getGraphics();
	setGraphics(g);
	::drawImage(tex, x, y, sx, sy, sw, sh, rateX, rateY, xyz, degree, anc, reverse);
	setGraphics(bk);
}

void iGraphics::drawString(float x, float y,int anc, const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	Graphics* bk = getGraphics();
	setGraphics(g);
	::drawString(x, y, anc, szText);
	setGraphics(bk);
}