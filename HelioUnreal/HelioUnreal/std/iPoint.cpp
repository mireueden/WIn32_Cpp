#include "iPoint.h"

static int ttt;

bool iPoint::operator == (const iPoint& p)
{
#if 0
    return x == p.x && y == p.y;
#else
    bool same = fabsf(x - p.x) < 0.0001 && fabsf(y - p.y) < 0.0001;
    if (same)
    {
        x = p.x;
        y = p.y;
    }
    return same;
#endif
}
bool iPoint::operator != (const iPoint& p)
{
    return x != p.x || y != p.y;
}

iPoint& iPoint::operator += (const iPoint& p)
{
    x += p.x;
    y += p.y;
    return *this;
}
iPoint& iPoint::operator -= (const iPoint& p)
{
    x -= p.x;
    y -= p.y;
    return *this;
}

iPoint iPoint::operator - (const iPoint& p)
{
    iPoint r;
    r.x = x - p.x;
    r.y = y - p.y;
    return r;
}

iPoint iPoint::operator+(const iPoint& p)
{
    iPoint r;
    r.x = x + p.x;
    r.y = y + p.y;
    return r;
}


iPoint iPoint::operator*(float f)
{
    iPoint r;
    r.x = x * f;
    r.y = y * f;
    return r;
};
iPoint iPoint::operator/(float f)
{
    iPoint r;
    r.x = x * f;
    r.y = y * f;
    return r;
};

float iPoint::length()
{
    //return iPointLength(x, y);
    return sqrtf(x * x + y * y);
}

void iPoint::loadIdentity()
{
    float len = sqrtf(x * x + y * y);//length();
    if (len == 0.0f)
        return;
    x /= len;
    y /= len;
}


iPoint iPointMake(float x, float y)
{
    iPoint r;
    r.x = x;
    r.y = y;
    return r;
}

void iPointAbs(iPoint& p)
{
    p.x = fabsf(p.x);
    p.y = fabsf(p.y);
}

float iPointLength(const iPoint& p)
{
    return sqrtf(p.x * p.x + p.y * p.y);
}

float iPointDot(const iPoint& p0, const iPoint& p1)
{
    return p0.x * p1.x + p0.y * p1.y;
}


