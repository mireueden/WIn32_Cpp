#pragma once

#if _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

//#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#include <Windows.h>

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib, "opengl32")

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <malloc.h>
//#include <memory.h>
//#include <tchar.h>


#define DEV_WIDTH 640
#define DEV_HEIGHT 480

typedef unsigned char uint8, byte;
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint32;
typedef signed int int32;

enum iKeyStat
{
	iKeyStatBegan = 0,
	iKeyStatMoved,
	iKeyStatEnded,
	iKeyStatCancel
};

struct iPoint;
struct iSize;
struct iRect;

typedef void (*METHOD_VOID)();
typedef void (*METHOD_FLOAT)(float dt);
typedef void (*METHOD_KEY)(iKeyStat stat, iPoint point);


#define keydown_none  0//   000000 000000000 00000000 00000000
#define keydown_w     1//   000000 000000000 00000000 00000001
#define keydown_a     2//   000000 000000000 00000000 00000010
#define keydown_s     4//   000000 000000000 00000000 00000100
#define keydown_d     8//   000000 000000000 00000000 00001000
#define keydown_uo    16//  000000 000000000 00000000 00010000
#define keydown_left  32//  000000 000000000 00000000 00100000
#define keydown_down  64//  000000 000000000 00000000 01000000
#define keydown_right 128// 000000 000000000 00000000 10000000
#define keydown_space 256
#define keydown_what   1<<31// 10000000 00000000 00000000 00000000 

#define LEFT		1
#define RIGHT		2
#define HCENTER		4
#define TOP			8
#define BOTTOM		16
#define VCENTER		32

#define REVERSE_NONE	0
#define REVERSE_WIDTH	1
#define REVERSE_HEIGHT	2

struct Texture
{
	uint32 texID; // OpenGL이면 unsignede int,gdi+면 lmage
	int width, height;
	int potWidth, potHeight;
	int retainCount;
};

#if 1
// window
#define VSNPRINTF _vsnprintf
#else
// linux
#define VSNPRINTF vsnprintf
#endif

#define va_start_end(szFormat, szText)						\
		va_list args;										\
		va_start(args, szFormat);							\
		_vsnprintf(szText, sizeof(szText), szFormat, args);	\
		va_end(args);
