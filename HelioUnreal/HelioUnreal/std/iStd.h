#pragma once

#include "iDefine.h"

#include "iPoint.h"
#include "iSize.h"
#include "iRect.h"
#include "iColor.h"
#include "iString.h"
#include "iArray.h"
#include "iGraphics.h"
#include "iImage.h"
#include "iPopup.h"
#include "iStrTex.h"

#include "iOpenGL.h"
#include "iFPS.h"
#include "iSort.h"
#include "iShortestPath.h"
#include "iShadertoy.h"

// keydown 눌렀을때 1번 발생
// keystat 누르고 있을때 계속 발생
extern int keydown, keystat;
extern iSize devSize;
extern iRect viewport;

struct DelayPoint
{
	iKeyStat s;
	iPoint p;
};
extern DelayPoint* delayPoint;
extern int delayNum;

void loadApp(HWND hWnd, METHOD_VOID load, METHOD_VOID free,
	METHOD_FLOAT draw, METHOD_KEY key);
void freeApp();
void drawApp(float dt);
void keyApp(iKeyStat stat, iPoint point);

void getRGBA(float& r, float& g, float& b, float& a);
void setRGBA(float r, float g, float b, float a);

void clear();

void setClip(float x, float y, float width, float height);

void setLineWidth(float width);
void drawLine(float x0, float y0, float x1, float y1);
void drawLine(iPoint p0, iPoint p1);

void drawRect(float x, float y, float width, float height, float radius = 0.0f);
void drawRect(iRect rt, float radius = 0.0f);
void fillRect(float x, float y, float width, float height, float radius = 0.0f);
void fillRect(iRect rt, float radius = 0.0f);

typedef void (*MethodImageFilter)(uint8* bgra, int width, int height, int stride);
void setImageFilter(MethodImageFilter method);
Texture* createImageFilter(const char* szFormat, ...);

void setImage(TextureWrap w, TextureFilter f);
void applyImage();

void setImage(Texture* tex, TextureWrap w, TextureFilter f);

void imageFilterGrey(uint8* bgra, int width, int height, int stride);
void imageFilterMirror(uint8* bgra, int width, int height, int stride);

uint32 nextPot(uint32 x);
uint8* bmp2rgba(Bitmap* bmp, int& width, int& height);
Texture* createImageWithRGBA(uint8* rgba, int width, int height);

Texture* createImage(const char* szFormat, ...);
Texture** createImage(int wNum, int hNum, const char* szFormat, ...);
void freeImage(Texture* tex);
void drawImage(Texture* tex, float x, float y, int anc);
void drawImage(Texture* tex, float x, float y,
	int sx, int sy, int sw, int sh,
	float rateX, float rateY,
	int xyz, float degree, int anc, int reverse = REVERSE_NONE);
// xyz 0:x축, 1:y축, 2:z축으로 회전

const char* getStringName();
void setStringName(const char* name);
float getStringSize();
void setStringSize(float size);
float getStringLineHeight();
void setStringLineHeight(float height);
void getStringRGBA(float& r, float& g, float& b, float& a);
void setStringRGBA(float r, float g, float b, float a);
iRect rectOfString(const char* szFormat, ...);
void drawString(float x, float y, int anc, const char* szFormat, ...);

wchar_t* utf8_to_utf16(const char* szFormat, ...);
char* utf16_to_utf8(const wchar_t* wStr);

#define rad2deg(r) ((r) * 180 / M_PI)
#define deg2rad(d) ((d) * M_PI / 180)

float linear(float s, float e, float rate);
iPoint linear(iPoint s, iPoint e, float rate);

float easeIn(float s, float e, float rate);
iPoint easeIn(iPoint s, iPoint e, float rate);

float easeOut(float s, float e, float rate);
iPoint easeOut(iPoint s, iPoint e, float rate);

float clamp(float f, float min, float max);

bool move(iPoint* cp, const iPoint* tp, const iPoint& mp);

char* loadFile(int& len, const char* szFormat, ...);
void saveFile(char* buf, int bufLen, const char* szFormat, ...);
// bmp, tga, png, jpeg
// obj, md3, md4, fbx, 