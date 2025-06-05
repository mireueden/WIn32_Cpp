#pragma once

#include "iDefine.h"

extern HWND hwnd;
extern HDC hdc;
extern HGLRC hrc;

void loadOpenGL(HWND hwnd);
void freeOpenGL();
void resizeOpenGL(int width, int height);

void swapBuffer();
void setMakeCurrent(bool enable);

