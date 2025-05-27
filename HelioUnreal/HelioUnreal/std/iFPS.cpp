#include "iFPS.h"

#if 1 // Windows
// later ... (framework.h)
#include <windows.h>
#include "iGraphics.h"

iFPS::iFPS()
{

    lastUpdate = GetTickCount();
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

    return delta;
}

#else 

#endif