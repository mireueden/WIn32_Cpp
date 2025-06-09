#include "ImageText.h"

Texture** texs;

void loadImageText()
{
	texs = new Texture * [256];
	memset(texs, 0x00, sizeof(Texture*) * 256);

	iGraphics* g = iGraphics::share();
	setStringSize(32);
	setStringRGBA(1, 1, 1, 1);

	const char* s = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";
	for (int i = 0; s[i]; i++) // i < strlen(s); <-이런형태는 작성X
	{
		char c = s[i];
		char str[] = { c, 0 };
		iRect rt = rectOfString(str);
		if (rt.size.width == 0 || rt.size.height == 0)
			rt.size = iSizeMake(10, 32);
		g->init(rt.size.width, rt.size.height);

		g->drawString(0, 0, TOP | LEFT, str);

		texs[c] = g->getTexture();
		g->clean();
	}
}

void freeImageText()
{
	for (int i = 0; i < 256; i++)
	{
		if (texs[i])
			freeImage(texs[i]);
	}
	delete texs;
}

void drawImageText(float dt)
{
	const char* str = "HELLO MY WORLD 1818";
	float x = 0;
	for (int i = 0; str[i]; i++)
	{
		Texture* t = texs[str[i]];
		// str값이 ' '일때 오류 발생.

		drawImage(t, x, 300, TOP | LEFT);

		x += t->width + 1;
	}
}

void keyImageText(iKeyStat stat, iPoint point)
{
}
