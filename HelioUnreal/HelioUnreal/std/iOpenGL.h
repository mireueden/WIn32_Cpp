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

// glew.c 프로젝트 포함
// glew.h / wglew.h gl/ 복사
// #include <gl/glew.h> <gl/gl.h> 먼저포함
//  glewExperimental = TRUE; && glewInit(); 설정
class iFBO
{
public:
	// iFBO();
	iFBO(int width, int height);
	virtual ~iFBO();


	static Texture* createImage(int width, int height);

	void bind();
	void bind(Texture* tex);
	void unbind();

	uint32 fbo;
	uint32 depthBuffer;
	Texture* tex;
};

extern iFBO* fbo;