#include "iStd.h"

int keydown, keystat;
iSize devSize;// 640 x 480, 1920 x 1080
iRect viewport;

ULONG_PTR           gdiplusToken;

static float _r, _g, _b, _a;

METHOD_VOID methodFree;
METHOD_FLOAT methodDraw;
METHOD_KEY methodKey;

void loadApp(HWND hWnd, METHOD_VOID load, METHOD_VOID free,
    METHOD_FLOAT draw, METHOD_KEY key)
{
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    keydown = keydown_none;
    keystat = keydown_none;
    devSize = iSizeMake(DEV_WIDTH, DEV_HEIGHT);
    viewport = iRectMake(0, 0, 1, 1);

    loadOpenGL(hWnd);

    _r = 1.0f;
    _g = 1.0f;
    _b = 1.0f;
    _a = 1.0f;

    setMakeCurrent(true);
    load();
    setMakeCurrent(false);
    methodFree = free;
    methodDraw = draw;
    methodKey = key;
}

void freeApp()
{
    methodFree();

    freeOpenGL();
    GdiplusShutdown(gdiplusToken);
}

void drawApp(float dt)
{
#if 0
    setMakeCurrent(true);

    glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);

    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 선긋기 / 사각형(fill/draw) / 이미지
    setLineWidth(20);

    setRGBA(0, 1, 0, 0.5f);
    drawLine(30, 30, 300, 200);
    setRGBA(0, 0, 1, 0.5f);
    fillRect(100, 60, 60, 60, 30);

    //static Texture* tex = createImage("assets/def.png");
    //setRGBA(1, 1, 1, 1);
    //drawImage(tex, 0, 0, TOP | LEFT);

    swapBuffer();
    setMakeCurrent(false);
    return;
#endif

    setMakeCurrent(true);

    resizeOpenGL(0, 0);// wm_size, wm_sizing, wm_move

#if 0// double buffering
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    methodDraw(dt);
#else
    // back buffer(bmp::graphics)
    fbo->bind();
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    methodDraw(dt);
    fbo->unbind();

    // front buffer(draw bmp)
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);// Pre-multiplied Apha

    Texture* t = fbo->tex;
    drawImage(t, 0, 0, 0, 0, t->width, t->height, 1, 1, 2, 0, TOP | LEFT, REVERSE_HEIGHT);
    // MINI-MAP
    //drawImage(t, devSize.width-10, devSize.height-10, 0, 0, t->width, t->height,
    //    0.2f, 0.2f, 2, 0, BOTTOM | RIGHT, REVERSE_HEIGHT);
#endif

    swapBuffer();
    setMakeCurrent(false);
}

void keyApp(iKeyStat stat, iPoint point)
{
    methodKey(stat, point);
}

void getRGBA(float& r, float& g, float& b, float& a)
{
    r = _r, g = _g, b = _b, a = _a;
}

void setRGBA(float r, float g, float b, float a)
{
    _r = r; _g = g; _b = b; _a = a;
}

void clear()
{
    glClearColor(_r, _g, _b, _a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void setClip(float x, float y, float width, float height)
{
    if (x == 0 && y == 0 && width == 0 && height == 0)
    {
        glDisable(GL_SCISSOR_TEST);
    }
    else
    {
        glEnable(GL_SCISSOR_TEST);
        //glScissor(x, y, width, height);
        glScissor(x, devSize.height - y - height, width, height);
    }
}

float lineWidth = 1;
void setLineWidth(float width)
{
    lineWidth = width;
}

void drawLine_deprecated(float x0, float y0, float x1, float y1)
{
    float position[] = { x0, y0, x1, y1 };

    glLineWidth(lineWidth);
    glColor4f(_r, _g, _b, _a);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, position);

    uint8 indices[] = { 0, 1 };
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_BYTE, indices);

    glDisableClientState(GL_VERTEX_ARRAY);
}

void drawLine(float x0, float y0, float x1, float y1)
{
    static uint32 programID = iShader::buildFromPath("assets/shader/gdi.vert", "assets/shader/line.frag");
    glUseProgram(programID);

    float x = (x0 + x1) / 2.0f;
    float y = (y0 + y1) / 2.0f;
    float half = lineWidth / 2 + 0.5f;
    float dx = x1 - x0, dy = y1 - y0;
    float d = sqrtf(dx * dx + dy * dy) / 2 + half;
    float position[] = {
        -d, -half, 0, 1,            d, -half, 0, 1,
        -d, +half, 0, 1,            d, +half, 0, 1,
    };
    glm::mat4 projMatrix = glm::ortho(0.0f, devSize.width, devSize.height, 0.0f, -1000.0f, 1000.0f);

    glm::mat4 viewMatrix(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(x, y, 0));
    float theta = atan((float)dy / dx);// *180 / M_PI;
    viewMatrix = glm::rotate(viewMatrix, theta, glm::vec3(0, 0, 1));

    glBindBuffer(GL_ARRAY_BUFFER, vbo);// 1
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, position);
    uint32 pAttr = glGetAttribLocation(programID, "position");
    glEnableVertexAttribArray(pAttr);// 2
    glVertexAttribPointer(pAttr, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)0);

    uint32 pID = glGetUniformLocation(programID, "projMatrix");
    glUniformMatrix4fv(pID, 1, false, (float*)&projMatrix);
    uint32 vID = glGetUniformLocation(programID, "viewMatrix");
    glUniformMatrix4fv(vID, 1, false, (float*)&viewMatrix);
    // x0, y0 in 640x480 => in viewport
    //float r = viewport.size.width / devSize.width;
    //x0 = viewport.origin.x + x0 * r;
    //y0 = devSize.height - y0;
    //y0 = viewport.origin.y + y0 * r;
    //x1 = viewport.origin.x + x1 * r;
    //y1 = devSize.height - y1;
    //y1 = viewport.origin.y + y1 * r;
    glUniform2f(glGetUniformLocation(programID, "u_start"), x0, y0);
    glUniform2f(glGetUniformLocation(programID, "u_end"), x1, y1);
    glUniform1f(glGetUniformLocation(programID, "u_width"), lineWidth);
    glUniform4f(glGetUniformLocation(programID, "u_color"), _r, _g, _b, _a);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);// 3
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);// 3

    glDisableVertexAttribArray(pAttr);// 2
    glBindBuffer(GL_ARRAY_BUFFER, 0);// 1
}

void drawLine(iPoint p0, iPoint p1)
{
    drawLine(p0.x, p0.y, p1.x, p1.y);
}

void drawRect(float x, float y, float width, float height, float radius)
{
    drawLine(x, y, x + width, y);
    drawLine(x, y + height, x + width, y + height);
    drawLine(x, y, x, y + height);
    drawLine(x + width, y, x + width, y + height);
}
void drawRect(iRect rt, float radius)
{
    drawRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}

void fillRect_deprecated(float x, float y, float width, float height)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float rt[] = {
    x, y,          _r, _g, _b, _a,   x + width, y,          _r, _g, _b, _a,
    x, y + height, _r, _g, _b, _a,   x + width, y + height, _r, _g, _b, _a,
    };
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2, GL_FLOAT, sizeof(float) * 6, &rt[0]);
    glColorPointer(4, GL_FLOAT, sizeof(float) * 6, &rt[2]);

    uint8 indices[] = { 0, 1, 2,  2, 1, 3 };
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}
void fillRect(float x, float y, float width, float height, float radius)
{
    static uint32 programID = iShader::buildFromPath("assets/shader/gdi.vert", "assets/shader/rect.frag");
    glUseProgram(programID);

    float position[] = {
        x, y,          0, 1,   x + width, y,          0, 1,
        x, y + height, 0, 1,   x + width, y + height, 0, 1,
    };
    glm::mat4 projMatrix = glm::ortho(0.0f, devSize.width, devSize.height, 0.0f, -1000.0f, 1000.0f);
    glm::mat4 viewMatrix(1.0f);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);// 1
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, position);
    uint32 pAttr = glGetAttribLocation(programID, "position");
    glEnableVertexAttribArray(pAttr);// 2
    glVertexAttribPointer(pAttr, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)0);

    uint32 pID = glGetUniformLocation(programID, "projMatrix");
    glUniformMatrix4fv(pID, 1, false, (float*)&projMatrix);
    uint32 vID = glGetUniformLocation(programID, "viewMatrix");
    glUniformMatrix4fv(vID, 1, false, (float*)&viewMatrix);

    // x, y, width, height in 640x480
    //float r = viewport.size.width / devSize.width;
    //x = viewport.origin.x + x * r;
    //y = devSize.height - y;
    //y = viewport.origin.y + y * r;
    //width *= r;
    //height *= r;
    //radius *= r;
    glUniform4f(glGetUniformLocation(programID, "u_rect"), x + width / 2, y + height / 2, width / 2, height / 2);
    glUniform4f(glGetUniformLocation(programID, "u_color"), _r, _g, _b, _a);
    glUniform1f(glGetUniformLocation(programID, "u_radius"), radius);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);// 3
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);// 3

    glDisableVertexAttribArray(pAttr);// 2
    glBindBuffer(GL_ARRAY_BUFFER, 0);// 1
}
void fillRect(iRect rt, float radius)
{
    fillRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}

char* stringName = NULL;
float stringSize = 25.0f, stringLineHeight = 30.0f;
float sr = 1.0f, sg = 1.0f, sb = 1.0f, sa = 1.0f;
const char* getStringName()
{
    return stringName;
}
void setStringName(const char* name)
{
    if (stringName)
    {
        if (strcmp(stringName, name) == 0)
            return;
        delete stringName;
    }
    stringName = iString::copy(name);
}

float getStringSize()
{
    return stringSize;
}
void setStringSize(float size)
{
    stringSize = size;
}
float getStringLineHeight()
{
    return stringLineHeight;
}
void setStringLineHeight(float height)
{
    stringLineHeight = height;
}

void getStringRGBA(float& r, float& g, float& b, float& a)
{
    r = sr; g = sg; b = sb; a = sa;
}
void setStringRGBA(float r, float g, float b, float a)
{
    sr = r; sg = g; sb = b; sa = a;
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

uint8* bmp2rgba(Bitmap* bmp, int& width, int& height)
{
    Rect rt;
    rt.X = 0, rt.Y = 0, rt.Width = bmp->GetWidth(),
        rt.Height = bmp->GetHeight();
    BitmapData bmpData;
    bmp->LockBits(&rt, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);

    uint8* bgra = (uint8*)bmpData.Scan0;
    int stride = bmpData.Stride;

    int pw = nextPot(rt.Width), ph = nextPot(rt.Height);
    uint8* rgba = new uint8[pw * ph * 4];
    memset(rgba, 0x00, sizeof(uint8) * pw * ph * 4);
    for (int j = 0; j < rt.Height; j++)
    {
        for (int i = 0; i < rt.Width; i++)
        {
            uint8* s = &bgra[stride * j + 4 * i];
            uint8* d = &rgba[pw * 4 * j + 4 * i];
            d[0] = s[2];
            d[1] = s[1];
            d[2] = s[0];
            d[3] = s[3];
        }
    }

    bmp->UnlockBits(&bmpData);

    width = rt.Width;
    height = rt.Height;
    return rgba;
}

Texture* createImageWithRGBA(uint8* rgba, int width, int height)
{
    uint32 texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// GL_REPEAT
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST

    int pw = nextPot(width), ph = nextPot(height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pw, ph, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

    glBindTexture(GL_TEXTURE_2D, 0);

    Texture* tex = new Texture;
    tex->texID = texID;
    tex->width = width;
    tex->height = height;
    tex->potWidth = pw;
    tex->potHeight = ph;
    tex->retainCount = 1;

    return tex;
}

static MethodImageFilter method = NULL;
void setImageFilter(MethodImageFilter method)
{
    ::method = method;
}

Texture* createImageFilter(const char* szFormat, ...)
{
    char szText[512];
    va_start_end(szFormat, szText);

    wchar_t* path = utf8_to_utf16(szText);
    Bitmap* bmp = Bitmap::FromFile(path);
    delete path;
    int width, height;
    uint8* rgba = bmp2rgba(bmp, width, height);
    delete bmp;

    if (method)
        method(rgba, width, height, nextPot(width));

    Texture* tex = createImageWithRGBA(rgba, width, height);
    delete rgba;

    return tex;
}

void imageFilterGrey(uint8* bgra, int width, int height, int stride)
{
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            uint8* c = &bgra[stride * 4 * j + 4 * i];
            uint8 grey = c[0] * 0.3f + c[1] * 0.4f + c[2] * 0.3f;
            c[0] = grey;
            c[1] = grey;
            c[2] = grey;
        }
    }
}

void imageFilterMirror(uint8* bgra, int width, int height, int stride)
{
    float rateHeight = 0.5f;
    int h = height * rateHeight;

    int* pixels = (int*)bgra;
    for (int j = 0; j < h; j++)
        memcpy(&pixels[stride * j], &pixels[stride * (int)(j / rateHeight)], sizeof(int) * stride);
    for (int j = h; j < height; j++)
        memset(&pixels[stride * j], 0x00, sizeof(int) * stride);

    uint8 t[4];
    int len = sizeof(uint8) * 4;
    for (int j = 0; j < h / 2; j++)
    {
        for (int i = 0; i < width; i++)
        {
            uint8* s = &bgra[stride * 4 * j + 4 * i];
            uint8* d = &bgra[stride * 4 * (h - 1 - j) + 4 * i];

            memcpy(t, s, len);
            memcpy(s, d, len);
            memcpy(d, t, len);

            s[3] = linear(0xFF, 0x00, (float)j / h);
            d[3] = 0xFF - s[3];

            //uint8 grey = c[0] * 0.3f + c[1] * 0.4f + c[2] * 0.3f;
            //c[0] = grey;
            //c[1] = grey;
            //c[2] = grey;
        }
    }
}
static TextureWrap wrap = TextureWrapClamp;
static TextureFilter filter = TextureFilterLinear;

void setImage(TextureWrap w, TextureFilter f)
{
    wrap = w;
    filter = f;
}

void applyImage()
{
    if (wrap == TextureWrapClamp)
    {    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else if (wrap == TextureWrapRepeat)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    if (filter == TextureFilterLinear)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else if (filter == TextureFilterNearest)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

}

void setImage(Texture* tex, TextureWrap w, TextureFilter f)
{
    
    TextureWrap _w = wrap;
    TextureFilter _f = filter;
    setImage(w, f);

    glBindTexture(GL_TEXTURE_2D,tex->texID);
    applyImage();
    glBindTexture(GL_TEXTURE_2D, 0);
  
    setImage(_w, _f);
}

Texture* createImage(const char* szFormat, ...)
{
    char szText[512];
    va_start_end(szFormat, szText);

    wchar_t* path = utf8_to_utf16(szText);
    Bitmap* bmp = Bitmap::FromFile(path);
    delete path;
    int width, height;
    uint8* rgba = bmp2rgba(bmp, width, height);
    delete bmp;

    Texture* tex = createImageWithRGBA(rgba, width, height);
    delete rgba;

    return tex;
}

Texture** createImage(int wNum, int hNum, const char* szFormat, ...)
{
    char szText[512];
    va_start_end(szFormat, szText);

    wchar_t* path = utf8_to_utf16(szText);
    Bitmap* bmp = Bitmap::FromFile(path);
    delete path;
    int width, height;
    uint8* rgba = bmp2rgba(bmp, width, height);
    delete bmp;
    int potWidth = nextPot(width);

    int num = wNum * hNum;
    Texture** texs = new Texture * [num];
    int w = width / wNum;
    int h = height / hNum;
    int pw = nextPot(w), ph = nextPot(h);
    uint8* tmp = new uint8[pw * ph * 4];
    memset(tmp, 0x00, sizeof(uint8) * pw * ph * 4);
    for (int j = 0; j < hNum; j++)
    {
        for (int i = 0; i < wNum; i++)
        {
            // tmp << rgba (i, j)
            for (int n = 0; n < h; n++)
            {
                memcpy(&tmp[pw * 4 * n], &rgba[potWidth * 4 * (h * j + n) + w * 4 * i], pw * 4);
            }

#if 0//for testing
            for (int q = 0; q < h; q++)
            {
                for (int p = 0; p < w; p++)
                {
                    uint8* c = &tmp[pw * 4 * q + 4 * p];
                    if (c[0] == 0xFF &&
                        c[1] == 0xFF &&
                        c[2] == 0xFF)
                        c[3] = 0;
                }
            }
#endif
            texs[wNum * j + i] = createImageWithRGBA(tmp, w, h);
        }
    }

    delete rgba;
    return texs;
}

void freeImage(Texture* tex)
{
    if (tex->retainCount > 1)
    {
        tex->retainCount--;
        return;
    }
    glDeleteTextures(1, &tex->texID);
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
    switch (anc) {
    case TOP | LEFT:                               break;
    case TOP | HCENTER:     x -= w / 2;            break;
    case TOP | RIGHT:       x -= w;                break;
    case VCENTER | LEFT:    x;         y -= h / 2; break;
    case VCENTER | HCENTER: x -= w / 2; y -= h / 2; break;
    case VCENTER | RIGHT:   x -= w;    y -= h / 2; break;
    case BOTTOM | LEFT:                y -= h;     break;
    case BOTTOM | HCENTER:  x -= w / 2; y -= h;     break;
    case BOTTOM | RIGHT:    x -= w;    y -= h;     break;
    }
    iPoint p[4] = { {-w / 2, -h / 2}, {w / 2, -h / 2},
                    {-w / 2,  h / 2}, {w / 2,  h / 2} };
    if (reverse & REVERSE_WIDTH)
    {
        for (int i = 0; i < 4; i++)
            p[i].x *= -1;
    }
    if (reverse & REVERSE_HEIGHT)
    {
        for (int i = 0; i < 4; i++)
            p[i].y *= -1;
    }

    glm::mat4 projMatrix = glm::ortho(0.0f, devSize.width, devSize.height, 0.0f, -1000.0f, 1000.0f);
    glm::mat4 viewMatrix(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(x + w / 2, y + h / 2, 0));
    if (degree)
        viewMatrix = glm::rotate(viewMatrix,
            glm::radians(degree), glm::vec3(xyz == 0, xyz == 1, xyz == 2));

    float pw = tex->potWidth, ph = tex->potHeight;
    float texCoord[] = {
        sx / pw, sy / ph,         (sx + sw) / pw, sy / ph,
        sx / pw, (sy + sh) / ph,  (sx + sw) / pw, (sy + sh) / ph,
    };

    float color[] = {
        _r, _g, _b, _a,     _r, _g, _b, _a,
        _r, _g, _b, _a,     _r, _g, _b, _a,
    };

    static uint32 programID = iShader::buildFromPath("assets/shader/std.vert", "assets/shader/alpha.frag");
    glUseProgram(programID);

    uint32 pID = glGetUniformLocation(programID, "projMatrix");
    glUniformMatrix4fv(pID, 1, false, (float*)&projMatrix);
    uint32 vID = glGetUniformLocation(programID, "viewMatrix");
    glUniformMatrix4fv(vID, 1, false, (float*)&viewMatrix);

    for (int i = 0; i < 4; i++)
    {
        Vertex* v = &vertex[i];
        memcpy(v->position, &p[i], sizeof(iPoint));
        v->position[2] = 0;
        v->position[3] = 1;
        memcpy(v->texCoord, &texCoord[2 * i], sizeof(float) * 2);
        memcpy(v->color, &color[4 * i], sizeof(float) * 4);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);// 1
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 4, vertex);

    uint32 pAttr = glGetAttribLocation(programID, "position");
    uint32 cAttr = glGetAttribLocation(programID, "color");
    uint32 tAttr = glGetAttribLocation(programID, "texCoord");
    glEnableVertexAttribArray(pAttr);// 2
    glEnableVertexAttribArray(cAttr);// 2
    glEnableVertexAttribArray(tAttr);// 2
    glVertexAttribPointer(pAttr, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 0));
    glVertexAttribPointer(cAttr, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 4));
    glVertexAttribPointer(tAttr, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(float) * 8));
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //glVertexPointer(2, GL_FLOAT, 0, p);
    //glColorPointer(4, GL_FLOAT, 0, color);
    //glTexCoordPointer(2, GL_FLOAT, 0, texCoord);
    //glEnable(GL_TEXTURE_2D);
    glUniform1i(glGetUniformLocation(programID, "tex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->texID);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);// 3
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);// 3

    //glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_COLOR_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    //glDisable(GL_TEXTURE_2D);
    glDisableVertexAttribArray(pAttr);// 2
    glDisableVertexAttribArray(cAttr);// 2
    glDisableVertexAttribArray(tAttr);// 2
    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawShadertoy(float dt)
{
    float position[] = {
        0, 0, 0, 1,                   devSize.width, 0, 0, 1,
        0, devSize.height, 0, 1,      devSize.width, devSize.height, 0, 1,
    };
    glm::mat4 projMatrix = glm::ortho(0.0f, devSize.width, devSize.height, 0.0f, -1000.0f, 1000.0f);
    glm::mat4 viewMatrix(1.0f);

    static uint32 programID = iShader::buildShaderToy("assets/shader/gdi.vert",
        "assets/shader/st.frag");
    glUseProgram(programID);

    uint32 pID = glGetUniformLocation(programID, "projMatrix");
    glUniformMatrix4fv(pID, 1, false, (float*)&projMatrix);
    uint32 vID = glGetUniformLocation(programID, "viewMatrix");
    glUniformMatrix4fv(vID, 1, false, (float*)&viewMatrix);

#define uniform3f(id, x, y, z)  glUniform3f(glGetUniformLocation(programID, id), x, y, z)
#define uniform1f(id, x)        glUniform1f(glGetUniformLocation(programID, id), x)
#define uniform1i(id, x)        glUniform1i(glGetUniformLocation(programID, id), x)
    uniform3f("iResolution", devSize.width, devSize.height, 0);
    static float iTime = 0.0f;
    uniform1f("iTime", iTime);
    iTime += dt;
    uniform1f("iTimeDelta", dt);
    uniform1f("iFrameRate", 0);
    uniform1f("iFrameRate", 0);
    static int iFrame = 0;
    uniform1i("iFrame", iFrame);
    iFrame++;
#if 0
    uniform float     iChannelTime[4];
    uniform vec3      iChannelResolution[4];
    uniform sampler2D iChannel0;
    uniform sampler2D iChannel1;
    uniform sampler2D iChannel2;
    uniform sampler2D iChannel3;
    uniform vec4      iMouse;
    uniform vec4      iDate;
    uniform float     iSampleRate;
#endif
    static Texture** texs = NULL;
    if (texs == NULL)
    {
        texs = new Texture * [4];
        for (int i = 0; i < 4; i++)
            texs[i] = createImage("assets/shader/%d.%s", i, i == 2 ? "png" : "jpg");
    }
    for (int i = 0; i < 4; i++)
    {
        char id[16];
        sprintf(id, "iChannel%d", i);
        glUniform1i(glGetUniformLocation(programID, id), i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texs[i]->texID);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);// 1
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16, position);

    uint32 pAttr = glGetAttribLocation(programID, "position");
    glEnableVertexAttribArray(pAttr);// 2
    glVertexAttribPointer(pAttr, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 0));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbe);// 3
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);// 3

    glDisableVertexAttribArray(pAttr);// 2
    glBindBuffer(GL_ARRAY_BUFFER, vbo);// 1
    for (int i = 0; i < 4; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void drawString(float x, float y, int anc, const char* szFormat, ...)
{
    printf("직접 사용하지 말고, iString을 권장\n");
}

class iText
{
public:
    iText()
    {
        bmp = new Bitmap(devSize.width, devSize.height);
        g = Graphics::FromImage(bmp);
    }

    virtual ~iText()
    {
        delete bmp;
        delete g;
    }

    iRect rectOfString(const char* str)
    {
        float bkR, bkG, bkB, bkA;
        float bkFR, bkFG, bkFB, bkFA;
        getRGBA(bkR, bkG, bkB, bkA);
        getStringRGBA(bkFR, bkFG, bkFB, bkFA);

        setStringRGBA(1, 1, 1, 1);
        iGraphics::clear(g, 0, 0, 0, 0);
        iGraphics::drawString(g, 0, 0, str);

        setRGBA(bkR, bkG, bkB, bkA);
        setStringRGBA(bkFR, bkFG, bkFB, bkFA);

        Rect rt;
        rt.X = 0; rt.Y = 0;
        rt.Width = bmp->GetWidth(); rt.Height = bmp->GetHeight();
        BitmapData bmpData;
        bmp->LockBits(&rt, ImageLockModeRead,
            PixelFormat32bppARGB, &bmpData);

        iRect rect = rectOfString((uint8*)bmpData.Scan0,
            bmpData.Stride, rt.Width, rt.Height);

        bmp->UnlockBits(&bmpData);

        return rect;
    }

    iRect rectOfString(uint8* bgra, int stride, int w, int h)
    {
        int left = w;
        for (int i = 0; i < w; i++)
        {
            bool found = false;
            for (int j = 0; j < h; j++)
            {
                if (bgra[stride * j + 4 * i + 3])
                {
                    found = true;
                    left = i;
                    break;
                }
            }
            if (found)
                break;
        }
        if (left == w)
            return iRectMake(0, 0, 0, 0);

        int right = -1;
        for (int i = w - 1; i > -1; i--)
        {
            bool found = false;
            for (int j = 0; j < h; j++)
            {
                if (bgra[stride * j + 4 * i + 3])
                {
                    found = true;
                    right = i;
                    break;
                }
            }
            if (found)
                break;
        }
        if (right == -1)
            return iRectMake(0, 0, 0, 0);

        int top = h;
        for (int j = 0; j < h; j++)
        {
            bool found = false;
            for (int i = 0; i < w; i++)
            {
                if (bgra[stride * j + 4 * i + 3])
                {
                    found = true;
                    top = j;
                    break;
                }
            }
            if (found)
                break;
        }
        if (top == h)
            return iRectMake(0, 0, 0, 0);

        int bottom = -1;
        for (int j = h - 1; j > -1; j--)
        {
            bool found = false;
            for (int i = 0; i < w; i++)
            {
                if (bgra[stride * j + 4 * i + 3])
                {
                    found = true;
                    bottom = j;
                    break;
                }
            }
            if (found)
                break;
        }
        if (bottom == -1)
            return iRectMake(0, 0, 0, 0);

        return iRectMake(left, top, right - left + 1, bottom - top + 1);
    }

    Bitmap* bmp;
    Graphics* g;
};

static iText* txt = NULL;

iRect rectOfString(const char* szFormat, ...)
{
    char szText[512];
    va_start_end(szFormat, szText);

    //graphics->MeasureString()
    if (txt == NULL)
        txt = new iText();

    return txt->rectOfString(szText);
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

//#define clamp(a, b, c) min(max(a, b), c) 
float clamp(float f, float min, float max)
{
    if (f < min)
        f = min;
    else if (f > max)
        f = max;
    return f;
}

bool move(iPoint* cp, const iPoint* tp, const iPoint& mp)
{
    if (cp->x < tp->x)
    {
        cp->x += mp.x;
        if (cp->x > tp->x)
            cp->x = tp->x;
    }
    else if (cp->x > tp->x)
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
    else if (cp->y > tp->y)
    {
        cp->y += mp.y;
        if (cp->y < tp->y)
            cp->y = tp->y;
    }
    return (cp->x == tp->x && cp->y == tp->y);
}

char* loadFile(int& len, const char* szFormat, ...)
{
    FILE* pf = fopen(szFormat, "rb");

    fseek(pf, 0, SEEK_END);// 파일 끝
    len = ftell(pf);// where
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

