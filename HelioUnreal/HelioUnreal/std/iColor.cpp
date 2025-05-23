// HelioUnreal.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "iColor.h"
#include "iStd.h"

iColor4b& iColor4b::operator = (const iColor4f& c)
{
	r = (byte)(c.r * 0xFF);
	g = (byte)(c.g * 0xFF);
	b = (byte)(c.b * 0xFF);
	a = (byte)(c.a * 0xFF);
	return *this;
}

iColor4b iColor4b::operator *(float f)
{
	iColor4b c;
	c.r = r * f;
	c.g = g * f;
	c.b = b * f;
	c.a = a * f;
	return c;
}

iColor4b iColor4b::operator + (const iColor4b c)
{
	iColor4b t;
	t.r = clamp(r + c.r, 0x00, 0xFF);
	t.g = clamp(g + c.g, 0x00, 0xFF);
	t.b = clamp(b + c.b, 0x00, 0xFF);
	t.a = clamp(a + c.a, 0x00, 0xFF);
	return t;
}


iColor4f& iColor4f::operator = (const iColor4b& c)
{
	r = c.r / 255.f;
	g = c.g / 255.f;
	b = c.b / 255.f;
	a = c.a / 255.f;
	return *this;
}

iColor4f iColor4f::operator + (const iColor4f c)
{
	iColor4f t;
	t.r = clamp(r + c.r, 0.0f, 1.0f);
	t.g = clamp(g + c.g, 0.0f, 1.0f);
	t.b = clamp(b + c.b, 0.0f, 1.0f);
	t.a = clamp(a + c.a, 0.0f, 1.0f);
	return t;
}

iColor4f iColor4f::operator *(float f)
{
	iColor4f c;
	c.r = r * f;
	c.g = g * f;
	c.b = b * f;
	c.a = a * f;
	return c;
}


iColor4b iColor4bMake(byte r, byte g, byte b, byte a)
{
	iColor4b c;
	c.r = g;
	c.g = g;
	c.b = b;
	c.a = a;
	return c;
}

iColor4f iColor4fMake(float r, float g, float b, float a)
{
	iColor4f c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
	return c;
}