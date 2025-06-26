#include "DTProc.h"

#include "DTLoading.h"
#include "DTLogin.h"

#include "DTObject.h"
#include "DTProcUI.h"
#include "DTProcPop.h"

void loadDTProc()
{
	printf("loadDTProc()");
	loadDTfield();
	loadDTObject();
	loadDTProcUI();
	loadDTProcPop();
}

void freeDTProc()
{
	printf("freeDTProc()");

	freeDTfield();
	freeDTObject();
	freeDTProcUI();
	freeDTProcPop();
}

void drawDTProc(float dt)
{
	drawDTfield(dt);
	drawDTObject(dt, offField);
	drawDTProcUI(dt);
	drawDTProcPop(dt);
}

void keyDTProc(iKeyStat stat, iPoint point)
{

	if (keyDTProcPop(stat, point) ||
		keyDTProcUI(stat, point) ||
		keyDTObject(stat, point))
		return;

	keyDTProcfield(stat, point);

	//if (stat == iKeyStatBegan)
	//	setLoading(DTStateLogin, freeDTProc, loadDTLogin);
}

// ======================================
// DTfield
// ======================================
iPoint offField;
static iPoint offMin, offMax;

Texture* texField;
void loadDTfield()
{
	offField = iPointZero;
	offMax = iPointZero;
	offMin = iPointMake(devSize.width - FieldWidth,
						devSize.height - FieldHeight);

	texField = createImage("assets/download2.jpg");

}

void freeDTfield()
{
	freeImage(texField);
}

void drawDTfield(float dt)
{
	//setRGBA(0, 0, 1, 1);
	//fillRect(0, 0, devSize.width, devSize.height);
	setRGBA(1, 1, 1, 1);
	Texture* t = texField;
	drawImage(t, offField.x, offField.y,
		0, 0, t->width, t->height,
		(float)FieldWidth / t->width, (float)FieldHeight / t->height,
		2, 0, TOP | LEFT);
}
bool dragField = false;
iPoint prevField;

void keyDTProcfield(iKeyStat stat, iPoint point)
{
	switch (stat)
	{
	case iKeyStatBegan:
		dragField = true;
		prevField = point;
		break;

	case iKeyStatMoved:
		if (dragField == false) break;
		showDTProcUI(false);

		offField += point - prevField;
		prevField = point;
		if (offField.x < offMin.x)			offField.x = offMin.x;
		else if (offField.x > offMax.x)		offField.x = offMax.x;
		if (offField.y < offMin.y)			offField.y = offMin.y;
		else if (offField.y > offMax.y)		offField.y = offMax.y;

		break;
	case iKeyStatEnded:
		if (dragField == false) break;
		showDTProcUI(true);

		dragField = false;
		break;
	}

}