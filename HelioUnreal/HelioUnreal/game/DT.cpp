#include "DT.h"

#include "DTCommon.h"
#include "DTLoading.h"
#include "DTLogin.h"
#include "DTProc.h"

#include "NSLib.h"
#pragma comment(lib, "NSLib.lib")

void loadDT()
{
	int result =  NS::nsAdd(3, 2);
	printf("nsAdd = %d\n", result);

	loadDTLogin();
	ds = DTStateLogin;

	loadAudio();
	playAudio(2);
}

void freeDT()
{
	freeAudio();

	switch (ds) {
	case DTStateLogin: freeDTLogin(); break;
	case DTStateProc: freeDTProc(); break;
	}
}

void drawDT(float dt)
{
	switch (ds) {
	case DTStateLogin: drawDTLogin(dt); break;
	case DTStateProc: drawDTProc(dt); break;
	}

	drawDTLoading(dt);
}

void keyDT(iKeyStat stat, iPoint point)
{
	if (keyDTLoading(stat, point))
		return;

	switch (ds) {
	case DTStateLogin: keyDTLogin(stat, point); break;
	case DTStateProc: keyDTProc(stat, point); break;
	}
}
