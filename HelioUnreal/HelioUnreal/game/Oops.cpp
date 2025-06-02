#include "Oops.h"

Texture* texOops;
Graphics* gOops;

iPoint clickS, clickE;
bool click = false;
iRect rtClick;

void loadOops()
{
	Bitmap* bmp = new Bitmap(devSize.width, devSize.height);
	Texture* tex = new Texture;
	tex->texID = bmp;
	tex->width = devSize.width;
	tex->height = devSize.height;
	tex->potWidth = devSize.width;
	tex->potHeight = devSize.height;
	tex->retainCount = 1;
	texOops = tex;
	gOops = Graphics::FromImage(bmp);
}

void freeOops()
{
	freeImage(texOops);
	delete gOops;
}

//iRect dragRect(iPoint s, iPoint e)
iRect dragRect(const iPoint& s, const iPoint& e)
{
	iRect rt;
	if (s.x < e.x)
	{
		rt.origin.x = s.x;
		rt.size.width = e.x - s.x;
	}
	else
	{
		rt.origin.x = e.x;
		rt.size.width = s.x - e.x;
	}

	if (clickS.y < clickE.y)
	{
		rt.origin.y = s.y;
		rt.size.height = e.y - s.y;
	}
	else
	{
		rt.origin.y = e.y;
		rt.size.height = s.y - e.y;
	}
	return rt;
}

void drawOops(float dt)
{
	const char* s = "한글TEST";
	setStringRGBA(1, 1, 1, 1);
	drawString(0, 0, TOP | LEFT, s);

	setRGBA(1, 0, 0, 1);
	iRect rt = rectOfString(s);
	drawRect(rt);

	const char* str[] = {
		"부산광역시",
		"해운대구",
		"센텀동로99",
		"벽산e센텀클래스원",
		"506호"
	};
	for (int i = 0; i < 5; i++)
	{
		drawString(300, 20 + 40 * i, TOP | RIGHT, str[i]);
		iRect rt = rectOfString(str[i]);
		printf("str[%d] = (%.0f, %.0f)\n", i, rt.size.width, rt.size.height);
	}

	return;

	Graphics* bk = getGraphics();
	setGraphics(gOops);

	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	drawString(100, 100, TOP | LEFT, "한글입니다.");

	setGraphics(bk);
	setRGBA(1, 1, 1, 1);
	drawImage(texOops, 0, 0, TOP | LEFT);

	setRGBA(1, 0, 0, 1);
	drawRect(rtClick);

	if (click)
	{
		iRect rt = dragRect(clickS, clickE);
		setRGBA(0, 0, 1, 0.5f);
		fillRect(rt);

		setRGBA(0, 0, 1, 1.0f);
		drawRect(rt);
	}
}

iRect rectOfString(Bitmap* bmp, iPoint s, iPoint e)
{// 하나의 가상 백버퍼에서
	iRect rtSrc = dragRect(s, e);
	iRect rtDst;

	/////////////////////////////////////////////////////////////
	Rect rt;
	rt.X = 0; rt.Y = 0;
	rt.Width = bmp->GetWidth(); rt.Height = bmp->GetHeight();
	BitmapData bmpData;
	bmp->LockBits(&rt, ImageLockModeRead,
		PixelFormat32bppARGB, &bmpData);

	uint8* bgra = (uint8*)bmpData.Scan0;
	int stride = bmpData.Stride;
	int x = rtSrc.origin.x, y = rtSrc.origin.y;
	int w = rtSrc.size.width, h = rtSrc.size.height;
	

	// left -> right
	int left = 0;
	for (int i = x; i < w; i++)
	{
		bool found = false;
		int dh = y + h;
		for (int j = y; j < dh; j++)
		{
			if (bgra[stride * j + 4 * i + 3])
			{
				found = true;
				left = i;
				break;
			}
		}
		if (found)
			break;
	}

	// right -> left
	int right = bmp->GetWidth();
	int sx = x - 1;
	for (int i = x + w - 1; i > sx; i--)
	{
		bool found = false;
		int dh = y + h;
		for (int j = y; j < dh; j++)
		{
			if (bgra[stride * j + 4 * i + 3])
			{
				found = true;
				right = i;
				break;
			}
		}
		if (found)
			break;
	}

	

	// top -> bottom
	int top = 0;
	int dh = y + h;
	for (int j = y; j < dh; j++)
	{
		bool found = false;
		for (int i = x; i < w; i++)
		{
			if (bgra[stride * j + 4 * i + 3])
			{
				found = true;
				top = j;
				break;
			}
		}
		if (found)
			break;
	}

	// bottom -> top
	int bottom = 0;
	int ty = y - 1;
	for (int j = y + h - 1; j > ty; j--)
	{
		bool found = false;
		for (int i = x; i < w; i++)
		{
			if (bgra[stride * j + 4 * i + 3])
			{
				found = true;
				bottom = j;
				break;
			}
		}
		if (found)
			break;
	}

	rtDst.origin.x = left;
	rtDst.origin.y = top;
	rtDst.size.width = right - left + 1;
	rtDst.size.height = bottom - top + 1;

	bmp->UnlockBits(&bmpData);
	/////////////////////////////////////////////////////////////

	return rtDst;
}

int left = 0, right = 1024, top = 0, bottom = 1024;
bool* visit;
void findPixel(uint8* bgra, int stride, int x, int y)
{
	visit[stride * y + x] = true;
	if (left < x)	left = x;
	if (right > x)	right = x;
	if (top < y)	top = y;
	if (bottom > y)	bottom = y;

	int index = stride * y + 4 * (x - 1) + 3;
	if (!visit[index] && bgra[index]) findPixel(bgra, stride, x - 1, y);
	index = stride * y + 4 * (x + 1) + 3;
	if (!visit[index] && bgra[index]) findPixel(bgra, stride, x + 1, y);
	index = stride * (y - 1) + 4 * x + 3;
	if (!visit[index] && bgra[index]) findPixel(bgra, stride, x, y - 1);
	index = stride * (y + 1) + 4 * x + 3;
	if (!visit[index] && bgra[index]) findPixel(bgra, stride, x, y + 1);
}

iRect rectOfString(Bitmap* bmp, iPoint p)
{
	Rect rt;
	rt.X = 0; rt.Y = 0;
	rt.Width = bmp->GetWidth(); rt.Height = bmp->GetHeight();
	BitmapData bmpData;
	bmp->LockBits(&rt, ImageLockModeRead | ImageLockModeWrite,
		PixelFormat32bppARGB, &bmpData);

	uint8* bgra = (uint8*)bmpData.Scan0;
	int stride = bmpData.Stride;

	////////////////////////////////////////////////////////
	// 사용되는 영역
	visit = new bool[1024 * 1024];
	memset(visit, false, sizeof(bool) * 1024 * 1024);

	findPixel(bgra, stride, p.x, p.y);

	delete visit;
	////////////////////////////////////////////////////////

	bmp->UnlockBits(&bmpData);

	return iRectMake(left, top, right - left + 1, bottom - top + 1);
}

void keyOops(iKeyStat stat, iPoint point)
{
	if (stat == iKeyStatBegan)
	{
		clickS = point;
		click = true;
	}
	else if (stat == iKeyStatMoved)
	{
		clickE = point;
	}
	else if (stat == iKeyStatEnded)
	{
		click = false;
		if (iPointLength(clickS - clickE) < 3)
			rtClick = rectOfString((Bitmap*)texOops->texID, point);
		else
			rtClick = rectOfString((Bitmap*)texOops->texID, clickS, clickE);
	}
}
