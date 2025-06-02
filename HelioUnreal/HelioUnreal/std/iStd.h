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

#include "iFPS.h"
#include "iSort.h"
#include "iShortestPath.h"

// keydown 눌럿을때 1번 발생
// keystat 누르고 있을때 계속 발생
extern int keydown, keystat;
extern iSize devSize;
extern iRect viewport;

void LoadApp(HWND hWnd, METHOD_VOID load, METHOD_VOID free, 
	METHOD_FLOAT draw, METHOD_KEY key);
void freeApp();
void drawApp(float dt);
void keyApp(iKeyStat stat, iPoint point);

Graphics* getGraphics();
void setGraphics(Graphics* g);

void getRGBA(float& r, float& g, float& b, float& a);
void setRGBA(float r, float g, float b, float a);

void clear();

void drawLine(float x0, float y0, float x1, float y1);
void drawLine(iPoint p0, iPoint p1);

void drawRect(float x, float y, float width, float height);
void drawRect(iRect r1);
void fillRect(float x, float y, float width, float height);
void fillRect(iRect r1);

uint32 nextPot(uint32 x);
Texture* createImage(const char* szFormat, ...);
void freeImage(Texture* tex);
void drawImage(Texture* tex, float x, float y, int anc);
void drawImage(Texture* tex, float x, float y,
	int sx, int sy, int sw, int sh,
	float rateX, float rateY, 
	int xyz, float degree, int anc, int reverse = REVERSE_NONE);
// 0 : x축으로 회전, 1 : y축으로 회전 , 2 : z축으로 회전


void setStringSize(float size);
void getStringRGBA(float& r, float& g, float& b, float& a);
void setStringRGBA(float r, float g, float b, float a);
iRect rectOfString(const char* szFormat, ...);
iSize sizeOfString(const char* szFormat, ...);
void _drawString(float x, float y, const char* szFormat, ...);
void drawString(float x, float y, int anc,  const char* szFormat, ...);

wchar_t* utf8_to_utf16(const char* szFormat, ...);
char* utf16_to_utf8(const wchar_t* wStr);

#define rad2deg(r) (r * 180.f / M_PI)
#define deg2rad(d) (d * M_PI / 180.f)

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