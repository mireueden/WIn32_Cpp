#include "iStd.h"

int keydown = 0;

ULONG_PTR gdiplusToken;
HDC hdc;
Graphics* graphics;

Bitmap* bmp;
Graphics* gFromBmp;

float _r, _g, _b, _a;

METHOD_VOID methodFree;
METHOD_FLOAT methodDraw;
METHOD_KEY methodkey;

void LoadApp(HWND hWnd, METHOD_VOID load, METHOD_VOID free, METHOD_FLOAT draw, METHOD_KEY key)
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    hdc = GetDC(hWnd);
    graphics = new Graphics(hdc);

    bmp = new Bitmap(640, 480);
    gFromBmp = Graphics::FromImage(bmp);

    _r = 1.0f;
    _g = 1.0f;
    _b = 1.0f;
    _a = 1.0f;

    load();
    methodFree = free;
    methodDraw = draw;
    methodkey = key;
}

void freeApp()
{
    methodFree();

    delete graphics;
    GdiplusShutdown(gdiplusToken);
}

void drawApp(float dt)
{
    // back buffer
    Graphics* bk = graphics;
    graphics = gFromBmp;
    
    methodDraw(dt);
  
    graphics = bk;

    // front buffer
    graphics->DrawImage(bmp, 0, 0);
    //graphics->DrawString();

    SwapBuffers(hdc);
}

void keyApp(iKeyStat stat, iPoint point)
{
    methodkey(stat, point);
}

void setRGBA(float r, float g, float b, float a)
{
    _r = r;
    _g = g;
    _b = b;
    _a = a;
}

void clear()
{
    graphics->Clear(Color(_a * 0xFF,
                          _r * 0xFF,
                          _g * 0xFF,
                          _b * 0xFF));
}

void drawLine(float x0, float y0, float x1, float y1)
{
    Pen pen(Color(_a * 0xFF, 
                  _r * 0xFF, 
                  _g * 0xFF,
                  _b * 0xFF));
    graphics->DrawLine(&pen, x0, y0, x1, y1);
}

void drawLine(iPoint p0, iPoint p1)
{
    drawLine(p0.x, p0.y, p1.x, p1.y);
}

void drawRect(float x, float y, float width, float height)
{
    Pen pen(Color(_a * 0xFF,
        _r * 0xFF,
        _g * 0xFF,
        _b * 0xFF));

    graphics->DrawRectangle(&pen, x, y, width, height);
}

void drawRect(iRect r1)
{
    drawRect(r1.origin.x, r1.origin.y, r1.size.width, r1.size.height);
}

void fillRect(float x, float y, float width, float height)
{
    SolidBrush brush(Color(_a * 0xFF,
        _r * 0xFF,
        _g * 0xFF,
        _b * 0xFF));
    graphics->FillRectangle(&brush, x, y, width, height);
}

void fillRect(iRect r1)
{
    fillRect(r1.origin.x, r1.origin.y, r1.size.width, r1.size.height);
}

Texture* createImage(const char* szFormat, ...)
{
    return NULL;
}

void freeImage(Texture* tex)
{
    if (tex->retainCount > 1)
    {
        tex->retainCount--;
        return;
    }
    // real tex 지우기
}

void drawImage(Texture* tex, float x, float y)
{
}

float stringsize = 25.0f;
float sr = 1.0f, sg = 1.0f, sb = 1.0f, sa = 1.0f;
void setStringSize(float size)
{
    stringsize = size;
}

void setStringRGBA(float r, float g, float b, float a)
{
    sr = r, sg = g, sb = b, sa = a;
}

void drawString(float x, float y, const WCHAR* str)
{
    FontFamily  fontFamily(L"Times New Roman");
    Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
    PointF      pointF(x, y);
    SolidBrush  solidBrush(Color(sa * 0xFF, 
                                 sr * 0xFF, 
                                 sb * 0xFF, 
                                 sa * 0xFF));

    graphics->DrawString(str, -1, &font, pointF, &solidBrush);
}

float linear(float s, float e, float rate)
{
    return s + (e - s) * rate;
}

iPoint linear(iPoint s, iPoint e, float rate)
{
    return s + (e - s) * rate;
}

float easeIn(float s, float e, float rate)
{
    float r = rate * rate;
    return s + (e - s) * r;
}

iPoint easeIn(iPoint s, iPoint e, float rate)
{
    float r = rate * rate;
    return s + (e - s) * r;
}

float easeOut(float s, float e, float rate)
{
    return 0;
}

iPoint easeOut(iPoint s, iPoint e, float rate)
{
    return iPointZero;
}


// #define clamp(a,b,c) min(max(a,b),c)

float clamp(float f, float min, float max)
{
    if (f < min)
       f = min;    
    else if (f > max)
        f = max;
    return f;
}

// x : 20 - 100
// r : 30           => s+(e-s) *r


void move(iPoint* cp, const iPoint* tp, iPoint mp)
{
    if (cp->x < tp->x)
    {
        cp->x += mp.x;
        if (cp->x > tp->x)
            cp->x = tp->x;
    }
    else  if (cp->x > tp->x)
    {
        cp->x += mp.x;
        if (cp->x < tp->x)
            cp->x = tp->x;
    }

    if (cp->y < tp->y)
    {
        cp->y += mp.y;
        if (cp->y > tp->y)
            cp->y = tp->y;
    }
    else  if (cp->y > tp->y)
    {
        cp->y += mp.y;
        if (cp->y < tp->y)
            cp->y = tp->y;
    }
}

char* loadFile(int& len, const char* szFormat, ...)
{
    FILE* pf = fopen(szFormat, "rb");

    // SEEK_CUR 파일 포이너의 현재 위치
    // SEEK_END 파일 끝
    // SEEK_START 파일 시작
    fseek(pf, 0, SEEK_END); // 파일 끝
    len = ftell(pf); // where
    char* buf = new char[len + 1];

    fseek(pf, 0, SEEK_SET);// 파일 시작
    fread(buf, sizeof(char), len, pf);
    buf[len] = 0;

    fclose(pf);
    return buf;


}

void saveFile(char* buf, int bufLen, const char* szFormat, ...)
{
    FILE* pf = fopen(szFormat, "wb");

    fwrite(buf, sizeof(char), bufLen, pf);

    fclose(pf);
}
