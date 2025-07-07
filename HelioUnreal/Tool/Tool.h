#pragma once

#include "resource.h"

#include "iWindow.h"
#include "iArray.h"

void loadApp(HWND hwnd);
void freeApp();
void drawApp(float dt);

#define ITM_FILTER L"ITM Files(*.ITM, *.itm)\0*.ITM;*.itm\0ALL Files(*.*)\0*.*\0"

void methodBtn(HWND hwnd);

void methodList(HWND hwnd);

void methodEdit(HWND hwnd);
void methodEditMulti(HWND hwnd);

struct Item
{
	char* name;
	int hp, mp, atk, def;
	char* exp;
};

extern iArray* array;
void methodArray(void* data);
