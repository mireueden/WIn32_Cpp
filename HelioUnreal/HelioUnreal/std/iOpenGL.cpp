#include "iOpenGL.h"

#include "iStd.h"

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

    glewExperimental = TRUE;
    GLenum error = glewInit();
    if (error != GLEW_OK)
        return;

    glEnable(GL_BLEND);

    // DST (0,0,0,1)        SRC(1,1,1,0.2);
    // 0 * (1.0 - 0.2) + 1 * 0.2
    // RGB (0.2,0.2,0.2,?)

    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Input Color.rgb = DST.rgb * (1.0 - SRC.alpha)  + SRC.rgb * SRC.alpha
    // Input Color.a   = DST.a * (1.0 - SRC.alpha) + SRC.alpha * SRC.alpha
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    // DST(background)  SRC(image)
    // Input Color.rgb = DST.rgb * (1.0 - SRC.alpha)  + SRC.rgb * SRC.alpha
    // Input Color.a   = DST.a * (1.0 - SRC.alpha) + SRC.alpha
    
    // fbo : frame buffer에 바인딩된 텍스쳐는 이렇게
    // Input Color.rgb = DST.rgb * (1.0 - SRC.alpha)  + SRC.rgb//(SRC.rgb * SRC.alpha)
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LINE_SMOOTH);

    //fbo = new iFBO(devSize.width, devSize.height);
    fbo = new iFBO(1920, 1080);
    fbo->tex->width = devSize.width;
    fbo->tex->height = devSize.height;
    

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

iFBO* fbo;

Texture* iFBO::createImage(int width, int height)
{
    uint32 texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glBindTexture(GL_TEXTURE_2D, 0);

    Texture* tex = new Texture;
    tex->texID = texID;
    tex->width = width;
    tex->height = height;
    tex->potWidth = width;
    tex->potHeight = height;
    tex->retainCount = 1;

    return tex;
}

iFBO::iFBO(int width, int height)
{
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    tex = createImage(width, height);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    //glDrawBuffer(GL_COLOR_ATTACHMENT0);
    GLenum fboBuffs[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, fboBuffs);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->texID, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    texPrev = new Texture * [10];
    numPrev = 0;
}

iFBO::~iFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo);

    glDeleteRenderbuffers(1, &depthBuffer);
    freeImage(tex);

    delete texPrev;
}

void iFBO::bind()
{
    bind(tex);
}

void iFBO::_bind(Texture* tex)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);// fbo screen

    //glDrawBuffer(GL_COLOR_ATTACHMENT0);
    GLenum fboBuffs[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, fboBuffs);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->texID, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    glViewport(0, 0, tex->width, tex->height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, tex->width, tex->height, 0, -1024, 1024);
}

void iFBO::bind(Texture* tex)
{
    _bind(tex);

    texPrev[numPrev] = tex; // [0] = tex, 1
    numPrev++;
}

void iFBO::unbind()
{
    numPrev--;
    if (numPrev)
        //_bind(texPrev[numPrev]);
        _bind(texPrev[numPrev - 1]);
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);// real screen

        glViewport(viewport.origin.x, viewport.origin.y,
            viewport.size.width, viewport.size.height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, devSize.width, devSize.height, 0, -1024, 1024);
    }

        

}
