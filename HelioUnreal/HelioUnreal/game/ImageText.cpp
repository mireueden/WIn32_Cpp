#include "ImageText.h"

ImageText** it;

void loadImageText()
{
	// 글씨체로 만듦
	it = new ImageText * [2];
	for (int i = 0; i < 2; i++)
	{
		setStringName(i == 0 ? "궁서체" : "굴림체");
		it[i] = new ImageText();
		it[i]->loadFont("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ");
	}

	// 이미지로 만듦
	const char* strName[5] = {
		"abc.png", "def.png", "download0.png",
		"download1.png", "download2.jpg"
	}; // 'a', 'b', 'c', 'd', 'e'
	Texture** texs = new Texture * [256];
	memset(texs, 0x00, sizeof(Texture*) * 256);
	for (int i = 0; i < 5; i++)
	{
		texs['a' + i] = createImage("assets/%s", strName[i]);
		Texture* t = createImage("assets/%s", strName[i]);
		t->width /= 5.0f;
		t->height /= 5.0f;
		t->potWidth /= 5.0f;
		t->potHeight /= 5.0f;
		texs['a' + i] = t;
	}
	it[2] = new ImageText();
	it[2]->loadImage(texs);
}

void freeImageText()
{
	for (int i = 0; i < 3; i++)
		delete it[i];
	delete it;
}

void drawImageText(float dt)
{
	for(int i = 0; i<2;i++)
		it[i]->paint(0, 300 + 40*i, TOP | LEFT, "HELLO MY WORLD 1818");
	it[2]->paint(0, 200, TOP | LEFT, "abcde");
	it[2]->paint(0, 400, TOP | LEFT, "cabde");

	int fps = iFPS::share()->framePerSec();
	it[0]->paint(devSize.width / 2, 5, TOP | RIGHT, "FPS %d", fps);
	

}

void keyImageText(iKeyStat stat, iPoint point)
{
}



ImageText::ImageText()
{
	texs = new Texture * [256];
	memset(texs, 0x00, sizeof(Texture*) * 256);

	interval = 1;
}

ImageText::~ImageText()
{
	for (int i = 0; i < 256; i++)
	{
		if (texs[i])
			freeImage(texs[i]);
	}
	delete texs;
}

void ImageText::loadFont(const char* s)
{
	iGraphics* g = iGraphics::share();

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

void ImageText::loadImage(Texture** texs)
{
	// 레퍼런스 카운팅을 위해 이렇게 작성
	for (int i = 0; i < 256; i++)
	{
		if (texs[i] == NULL) continue;

		this->texs[i] = texs[i];
		texs[i]->retainCount++;
	}

}

void ImageText::paint(float x, float y, int anc, const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	for (int i = 0; szText[i]; i++)
	{
		Texture* t = texs[szText[i]];
		drawImage(t, x, y, TOP | LEFT);

		x += t->width + interval;
	}
}
