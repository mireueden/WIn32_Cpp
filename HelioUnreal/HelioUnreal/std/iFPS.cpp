#include "iFPS.h"

#if 1 // Windows
// later ... (framework.h)
#include <windows.h>
#include "iGraphics.h"

iFPS::iFPS()
{
    lastUpdate = GetTickCount();
    count = 0;
    takeTime = 0.000001f;
}


iFPS::~iFPS()
{
    // to do
}

iFPS* iFPS::share()
{
    static iFPS* ptr = new iFPS();
    return ptr;
}


float iFPS::update()
{
    DWORD now = GetTickCount();
    float delta = (now - lastUpdate) / 1000.0f;
    lastUpdate = now;

    count++;
    takeTime += delta;
    while (takeTime > fpsInterval)
    {
        int m = count / takeTime; // 50 f/s
        takeTime -= fpsInterval; // 0.1

        count = m * takeTime;
    }

    return delta;
}

int iFPS::framePerSec()
{
    int fps = count / takeTime;
    return fps;
}

#else 

#endif