#include "DTProc.h"

#include "DTLoading.h"
#include "DTLogin.h"

#include "DTObject.h"
#include "DTProcUI.h"
#include "DTProcPop.h"

void loadDTProc()
{
	printf("loadDTProc()\n");
	loadDTField();
	loadDTObject();
	loadDTProcUI();
	loadDTProcPop();
}

void freeDTProc()
{
	printf("freeDTProc()\n");
	freeDTField();
	freeDTObject();
	freeDTProcUI();
	freeDTProcPop();
}

void drawDTProc(float dt)
{
	drawDTField(dt);
	drawDTObject(dt * 1, offField);
	drawDTProcUI(dt);
	drawDTProcPop(dt);
}

void keyDTProc(iKeyStat stat, iPoint point)
{
	if (keyDTProcPop(stat, point) ||
		keyDTProcUI(stat, point) ||
		keyDTObject(stat, point))
		return;

	keyDTField(stat, point);
	//if (stat == iKeyStatBegan)
	//	setLoading(DTStateLogin, freeDTProc, loadDTLogin);
}

// ====================================== 
// DTField
// ====================================== 
iPoint offField;
static iPoint offMin, offMax;

Texture* texField;

bool dragField;
iPoint prevField;

//int fieldTile[32 * 24];
int fieldX, fieldY, fieldW, fieldH;

void loadDTField()
{
	offField = iPointZero;
	offMax = iPointZero;
	offMin = iPointMake(devSize.width - FieldWidth,
						devSize.height - FieldHeight);

	texField = createImage("assets/download2.jpg");

	dragField = false;

	fieldW = 40, fieldH = 40;
	fieldX = FieldWidth / fieldW;
	fieldY = FieldHeight / fieldH;
}

void freeDTField()
{
	freeImage(texField);
}

void drawDTField(float dt)
{
	//setRGBA(0, 0, 1, 1);
	//fillRect(0, 0, devSize.width, devSize.height);
	setRGBA(1, 1, 1, 1);
	Texture* t = texField;
	drawImage(t, offField.x, offField.y, 
		0, 0, t->width, t->height,
		(float)FieldWidth / t->width, (float)FieldHeight / t->height,
		2, 0, TOP | LEFT);

	setRGBA(0xD2 / 255.f, 0x69 / 255.f, 0x1E / 255.f, 0.8f);
	fillRect(0, 0, devSize.width, devSize.height);

#if displayFieldTile
	int num = 32 * 24;
	iColor4f color[10] = {
		{1, 1, 1, 0.5f},
		{}, {}, {}, {}, {}, {}, {}, {},
		{1, 0, 0, 0.8f}
	};
	for (int i = 0; i < num; i++)
	{
		int ft = fieldTile[i];
		iColor4f* c = &color[ft];
		setRGBA(c->r, c->g, c->b, c->a);
		int x = i % fieldX;
		int y = i / fieldX;
		fillRect(	fieldW * x + offField.x,
					fieldH * y + offField.y, fieldW, fieldH);
	}
	setRGBA(1, 1, 1, 1);
#endif
}

void keyDTField(iKeyStat stat, iPoint point)
{
#if 0
	if (stat == iKeyStatBegan)
	{
		dtsp->set(fieldTile, fieldX, fieldY, fieldW, fieldH);
		DTUnitSida* u = (DTUnitSida*)unit[5];
		dtsp->run(u->position, point - offField, u->path, u->pathNum);
		u->pathIndex = 0;
	}
#else
	switch (stat) {

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
#endif
}

uint8 fieldTile[32 * 24] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 9, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 9, 1, 9, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
