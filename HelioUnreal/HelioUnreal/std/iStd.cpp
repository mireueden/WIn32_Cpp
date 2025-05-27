#include "iStd.h"

int keydown = 0;
iSize devSize;
iRect viewport;

ULONG_PTR gdiplusToken;
HDC hdc;
Graphics* graphics;

Bitmap* bmp;
Graphics* gFromBmp;
Texture* texBmp;

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

    keydown = keydown_none;
    devSize = iSizeMake(DEV_WIDTH, DEV_HEIGHT);
    viewport = iRectMake(0, 0, 1, 1);

    bmp = new Bitmap(devSize.width, devSize.height);
    gFromBmp = Graphics::FromImage(bmp);

    Texture* tex = new Texture;
    tex->texID = bmp;
    tex->width = bmp->GetWidth();
    tex->height = bmp->GetHeight();
    tex->potWidth = bmp->GetWidth();
    tex->potHeight = bmp->GetHeight();
    tex->retainCount++;
    texBmp = tex;


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
    //graphics->DrawImage(bmp, 0, 0);
    //graphics->DrawString();

    float r = viewport.size.width / texBmp->width;
#if 0
    static float delta = 0.0f;
    delta += dt;
    float t = 1.0f * 1.0f * fabsf(sin(delta)); // 1 ~ 2
    r *= t;
#endif
    //setRGBA(1, 0, 0, 1);
    //clear();

    setRGBA(1, 1, 1, 1);

    drawImage(texBmp, viewport.origin.x, viewport.origin.y,
        0, 0, texBmp->width, texBmp->height, r, r, 2, 0, TOP | LEFT);

    SwapBuffers(hdc);
}

void keyApp(iKeyStat stat, iPoint point)
{
    methodkey(stat, point);
}

Graphics* getGraphics()
{
    return graphics;
}

void setGraphics(Graphics* g)
{
    graphics = g;
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

uint32 nextPot(uint32 x)
{
    x = x - 1; 
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x + 1;
}

Texture* createImage(const char* szFormat, ...)
{
    char szText[512];
    va_start_end(szFormat, szText);

    wchar_t* path = utf8_to_utf16(szText);
    Image* img = Image::FromFile(path);
    delete path;

    Texture* tex = new Texture;
    tex->texID = img;
    tex->width = img->GetWidth();
    tex->height = img->GetHeight();
    tex->potWidth = nextPot(img->GetWidth());
    tex->potHeight = nextPot(img->GetHeight());
    tex->retainCount++;

    return tex;
}

void freeImage(Texture* tex)
{
    if (tex->retainCount > 1)
    {
        tex->retainCount--;
        return;
    }
    Image* img = (Image*)tex->texID;
    delete img;
    delete tex;
}

void drawImage(Texture* tex, float x, float y, int anc)
{
    drawImage(tex, x, y, 0, 0, tex->width, tex->height, 1.0f, 1.0f, 2, 0, anc);
}

void drawImage(Texture* tex, float x, float y, 
    int sx, int sy, int sw, int sh, 
    float rateX, float rateY, 
    int xyz, float degree, int anc, int reverse)
{
    
    int w = sw * rateX;
    int h = sh * rateY;
    switch (anc)
    {
    case TOP | LEFT:                          break;
    case TOP | HCENTER:     x -= w / 2;    y; break;
    case TOP | RIGHT:       x -= w;        y; break;

    case VCENTER | LEFT:    x;             y -= h / 2; break;
    case VCENTER | HCENTER: x -= w / 2;    y -= h / 2; break;
    case VCENTER | RIGHT:   x -= w;        y -= h / 2; break;

    case BOTTOM | LEFT:                     y -= h; break;
    case BOTTOM | HCENTER:  x -= w / 2;     y -= h; break;
    case BOTTOM | RIGHT:    x -= w;         y -= h; break;

    }
    iPoint p[3] = { {x,y},{x + w,y},{x,y + h} };

    if (reverse & REVERSE_WIDTH)
    {
        p[0].x += w;
        p[1].x -= w;
        p[2].x += w;
    }
    if (reverse & REVERSE_HEIGHT)
    {
        p[0].y += h;
        p[1].y += h;
        p[2].y -= h;
    }

    if (degree)
    {
        int w2 = w / 2, h2 = h / 2;
        if (xyz == 0)
        {
            float r = deg2rad(degree);
            p[0].y =
            p[2].y = y + h2 + h2 * cos(deg2rad(degree));
            p[1].y = y + h2 + h2 * sin(deg2rad(270 + degree));
        }
        else if (xyz == 1)
        {
            float r = deg2rad(degree);
            p[0].x =
            p[2].x = x + w2 + w2 * sin(deg2rad(270 + degree));
            p[1].x = x + w2 + w2 * cos(deg2rad(degree));
        }
        else if (xyz == 2)
        {
            float s = sin(deg2rad(degree));
            float c = cos(deg2rad(degree));

            iPoint dp[] = {
                {-w2, -h2},{w2, -h2},
                {-w2,h2}
            };

            for (int i = 0; i < 3; i++)
            {
                p[i] = p[i] + iPointMake(w2, h2);
                p[i].x = x + w2 + dp[i].x * c - dp[i].y * s;
                p[i].y = y + h2 + dp[i].x * s + dp[i].y * c;
            }
        }
    }

    ColorMatrix m = {
        _r, 0, 0, 0, 0,
        0, _g, 0, 0, 0,
        0, 0, _b, 0, 0,
        0, 0, 0, _a, 0,
        0, 0, 0, 0, 1
    };
    ImageAttributes attr;
    attr.SetColorMatrix(&m);

    graphics->DrawImage((Image*)tex->texID, (PointF*)p, 3,
        sx, sy, sw, sh, UnitPixel, &attr);
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

void drawString(float x, float y, const char* szFormat, ...)
{

    char szText[512];
    va_start_end(szFormat, szText)

    FontFamily  fontFamily(L"Times New Roman");
    Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
    PointF      pointF(x, y);
    SolidBrush  solidBrush(Color(sa * 0xFF, 
                                 sr * 0xFF, 
                                 sb * 0xFF, 
                                 sa * 0xFF));

    wchar_t* wStr = utf8_to_utf16(szText);
    graphics->DrawString(wStr, -1, &font, pointF, &solidBrush);
    delete wStr;
}

wchar_t* utf8_to_utf16(const char* szFormat, ...)
{
    char szText[512];
    va_start_end(szFormat, szText);

    int len = MultiByteToWideChar(CP_UTF8, 0, szText, -1, NULL, 0);
    wchar_t* wStr = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, szText, sizeof(szText), wStr, len);

    return wStr;
}
char* utf16_to_utf8(const wchar_t* wStr)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, wStr, lstrlen(wStr), NULL, 0, 0, NULL);
    char* str = new char[len + 1];
    WideCharToMultiByte(CP_UTF8, 0, wStr, lstrlen(wStr), str, len, 0, NULL);
    str[len] = 0;

    return str;
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
