#include "Comp.h"

// 배경이미지 (회색 배경 + 캡션 진한 회색 "정말" "종료할까요?")
iPopup* popComp;
iImage** imgCompBtn;

void loadComp()
{
	popComp = new iPopup();

	iGraphics* g = iGraphics::share();
	//
	// 640 x 480 => 320 x 240
	//
	iSize size = iSizeMake(320, 240);
	g->init(size.width, size.height);

	setRGBA(0.5f, 0.5f, 0.5f, 0.6f);
	g->fillRect(0, 0, size.width, size.height);

	setStringSize(30);
	setStringRGBA(1, 1, 1, 1);
	g->drawString(size.width / 2, size.height / 2, 
		VCENTER | HCENTER, "종료할까요?");
	
	setRGBA(0.3f, 0, 0, 0.6f);
	g->fillRect(0, 0, size.width, 40);
	g->drawString(10, 20, VCENTER | LEFT, "정말?");

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->add(tex);
	freeImage(tex);
	popComp->add(img);

	//
	// imgCompBtn 100 x 40
	//
	const char* strBtn[] = { "예", "아니오" };
	size = iSizeMake(100, 40);
	setStringSize(20);

	imgCompBtn = new iImage * [2];
	for (int i = 0; i < 2; i++)
	{
		img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			g->init(size.width, size.height);

			if (j == 0)
			{
				setRGBA(1, 0, 0, 0.5f);
				g->fillRect(0, 0, size.width, size.height);
				g->drawString(size.width / 2, size.height / 2,
					VCENTER | HCENTER, strBtn[i]);
			}
			else if (j == 1)
			{
				setRGBA(1, 0, 0, 0.5f);
				g->fillRect(15, 15, size.width - 30, size.height - 30);
				g->drawString(size.width / 2, size.height / 2,
					VCENTER | HCENTER, strBtn[i]);
			}

			tex = g->getTexture();
			img->add(tex);
			freeImage(tex);
		}
		img->position = iPointMake(40 + 120 * i, 190);
		popComp->add(img);
		imgCompBtn[i] = img;
	}
	//popComp->sp = iPointMake(-400, 200);
	//popComp->ep = iPointMake(200, 200);
	popComp->sp = iPointMake(200, -400);
	popComp->ep = iPointMake(200, 200);
	popComp->_aniDt = 0.5f;
}

void freeComp()
{
	delete popComp;
	delete imgCompBtn;
}

void drawComp(float dt)
{
	for (int i = 0; i < 2; i++)
		imgCompBtn[i]->index = (i == popComp->selected);

	popComp->paint(dt);

}

bool keyPopComp(iKeyStat stat, iPoint point)
{
	if (popComp->bShow == false)
		return false;
	if (popComp->stat != iPopupStatProc)
		return true;

	int i, j = -1;
	if (stat == iKeyStatBegan)
	{
		printf("imgCompBtn[%d] 눌렀네?\n", popComp->selected);
		popComp->show(false);
	}
	else if (stat == iKeyStatMoved)
	{
		for (int i = 0; i < 2; i++)
		{
			if (containPoint(point,
				imgCompBtn[i]->touchRect(popComp->ep)))
			{
				j = i;
				break;
			}
		}
		popComp->selected = j;
	}
	return true;
}

void keyComp(iKeyStat stat, iPoint point)
{
	if (keyPopComp(stat, point))
		return;

	if (stat == iKeyStatBegan)
	{
		popComp->show(true);
	}
	else if (stat == iKeyStatMoved)
	{
	}
}

