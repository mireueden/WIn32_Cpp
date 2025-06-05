#include "Oops.h"

Texture* texOops;
Graphics* gOops;

iPoint clickS, clickE;
bool click = false;
iRect rtClick;

void loadOops()
{
#if 0
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
#endif
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
#if 0
	// back buffer 작업
	static bool bDraw = false;
	//if (bDraw == false)
	{
		bDraw = true;
		Graphics* bk = getGraphics();
		setGraphics(gOops);

		setRGBA(0, 0, 0, 0);
		clear();

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

		// front buffer
		setGraphics(bk);
		

	}

	

	//setStringSize(30);
	//setStringRGBA(1, 1, 1, 1);
	//drawString(100, 100, TOP | LEFT, "한글입니다.");


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
#endif
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
bool* visit, * canGo;
#if 0
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
#else
void _findPixel(uint8* bgra, int stride,int w,int h, int x, int y)
{
	// Initalize
	left = x;
	right = x;
	top = y;
	bottom = y;

	// logic
	canGo[w * y + x] = true;

	// 반복 실행을 하되, 재귀함수를 피하기 위해서 for문으로 작성
	// 상하좌우를 이동할 수 있는 곳인지 검사
	// 168,108 > 상하좌우 순회. 오른쪽부터 방문을 할 수 있다?
	// 선택을 했을 때, 단순 자음or모음만 선택. 글자 하나를 선택하게 하기 위해서는?
	for (;;)
	{
		bool exist = false;
		// canGo :: visit
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				if (canGo[w * j + i] == false ||
					bgra[stride * j + 4 * i + 3] == 0)
					continue;

				canGo[w * j + i] = false;
				visit[w * j + i] = true;
				exist = true;

				if (i < left)	left = i;
				if (i > right)	right = i;
				if (j < top)	top = j;
				if (j > bottom) bottom = j;

				if (i > 0 &&
					visit[w * j + i - 1] == false)
					canGo[w * j + i - 1] = true; // left
				if (i < w - 1 &&
					visit[w * j + i - 1] == false)
					canGo[w * j + i - 1] = true; // right 
				if (j > 0 &&
					visit[w * (j - 1) + i] == false)
					canGo[w * (j - 1) + i] = true; // top
				if (j < h - 1 && 
					visit[w * (j + 1) + i] == false)
					canGo[w * (j + 1) + i] = true; // bottom
			}
		}

		if (exist == false)
			break;
	}

}

// 1) 갈 수 있는 곳을 지점(canGo)을 다음 프레임에서 호출해야 하는데,, 의도하지 않게 실행.
// 2) 'ㅅ'과 관련된 문자의 경우. 정상적으로 구하지 못하는 문제 

void findPixel(uint8* bgra, int stride, int w, int h, int x, int y, bool linked)
{
	if (linked == false)
	{
		_findPixel(bgra, stride, w, h, x, y);// left, right, top, bottom
		return;
	}

	for (;;)
	{
		bool theEnd = true;

		_findPixel(bgra, stride, w, h, x, y);// left, right, top, bottom
		for (int j = top; j < bottom; j++)
		{
			for (int i = left; i < right; i++)
			{
				if (visit[w * j + i] == false &&
					bgra[stride * j + 4 * i + 3])
				{
					x = i, y = j;
					theEnd = false;
					break;
				}
			}
		}

		if (theEnd)
			break;
	}

}
#endif // 0

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
	canGo = new bool[1024 * 1024];
	memset(visit, false, sizeof(bool) * 1024 * 1024);
	memset(canGo, false, sizeof(bool) * 1024 * 1024);

	int w = rt.Width, h = rt.Height;
	findPixel(bgra, stride, w, h, p.x, p.y, false);
	//findPixel(bgra, stride, w, h, p.x, p.y, true);
	delete visit;
	delete canGo;
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
