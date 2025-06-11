#include "iOpenGL.h"

HWND hwnd;
HDC hdc;
HGLRC hrc;

void loadOpenGL(HWND hwnd)
{
    ::hwnd = hwnd;
    hdc = GetDC(hwnd);
    // OpenGL Setiing
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0x00, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);
    hrc = wglCreateContext(hdc);

    setMakeCurrent(true);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LINE_SMOOTH);

    setMakeCurrent(false);
}

void freeOpenGL()
{
    wglMakeCurrent(NULL, NULL); // 지금부터 OpenGL 사용 끝
    wglDeleteContext(hrc);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);
}

#include "iRect.h"
extern iRect viewport;
extern iSize devSize;
void resizeOpenGL(int width, int height)
{
    glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, devSize.width, devSize.height, 0, -1024, 1024);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
}

void swapBuffer()
{
    SwapBuffers(hdc);
}

void setMakeCurrent(bool enable)
{
    if (enable)
        wglMakeCurrent(hdc, hrc);// 지금부터 OpenGL을 사용 시작
    else
        wglMakeCurrent(NULL, NULL);// 지금부터 OpenGL 사용 끝
}
