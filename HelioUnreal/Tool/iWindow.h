#pragma once

//#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#if _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

#if 1
#define VSNPRINTF _vsnprintf
#else
#define VSNPRINTF vsnprintf
#endif

#define va_start_end(szFormat, szText)						\
		va_list args;										\
		va_start(args, szFormat);							\
		_vsnprintf(szText, sizeof(szText), szFormat, args);	\
		va_end(args);

wchar_t* utf8_to_utf16(const char* szFormat, ...);
char* utf16_to_utf8(const wchar_t* wStr);

// hwnd parent
extern HWND hWnd;

enum WndStyle
{
	WndStatic = 0,
	WndButton,
	WndCheckBox,
	WndComboBox,
	WndListBox,
	WndRadio,
	WndEdit,
};

typedef void (*MethodHwnd)(HWND hwnd);

struct WndCtrl
{
	WndStyle style;
	int id;
	HWND hwnd;
	MethodHwnd m;
};
extern WndCtrl* wndCtrl;
extern int wndCtrlNum;
#define wndCtrlMax 100

void loadCtrl();
void freeCtrl();
void updateCtrl(WPARAM wParam);

HWND createWndStatic(
	int x, int y, int width, int height,
	MethodHwnd m, const char* szFormat, ...);

HWND createWndButton(
	int x, int y, int width, int height,
	MethodHwnd m, const char* szFormat, ...);

HWND createWndCheckBox(
	int x, int y, int width, int height,
	MethodHwnd m, const char* szFormat, ...);
bool getCheckBox(HWND hwnd);
void setCheckBox(HWND hwnd, bool on);

HWND createWndComboBox(
	int x, int y, int width, int height,
	MethodHwnd m, const char** str, int strNum);
void addWndComboBox(HWND hwnd, int index, const char* str);
int indexWndComboBox(HWND hwnd);
int countWndComboBox(HWND hwnd);
char* getWndComboBox(HWND hwnd, int index);
void setWndComboBox(HWND hwnd, int index);

HWND createWndListBox(
	int x, int y, int width, int height,
	MethodHwnd m, const char** str, int strNum);
void addWndListBox(HWND hwnd, int index, const char* str);
int indexWndListBox(HWND hwnd);
int countWndListBox(HWND hwnd);
char* getWndListBox(HWND hwnd, int index);
void setWndListBox(HWND hwnd, int index);

HWND createWndRadio(
	int x, int y, int width, int height,
	MethodHwnd m, const char* str);
bool getWndRadio(HWND hwnd);
void setWndRadio(HWND hwnd, bool on);

enum WndEditStyle
{
	WndEditAll = 0,
	WndEditInt,
	WndEditFloat,
};

HWND createWndEdit(
	int x, int y, int width, int height,
	WndEditStyle s, MethodHwnd m, const char* str, int strLengthMax = 0);

HWND createWndEditfMultiline(
	int x, int y, int width, int height,
	MethodHwnd m, const char* str, int strLengthMax = 0);

// ---------------------------------------------------
// Common Function
// ---------------------------------------------------
void setWndText(HWND hwnd, const char* szFormat, ...);
char* getWndText(HWND hwnd);
int getWndInt(HWND hwnd);
float getWndFloat(HWND hwnd);

const char* openFileDlg(bool open, LPCWSTR filter);