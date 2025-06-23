#include "DT.h"

#include "DTLoading.h"
#include "DTLogin.h"
#include "DTProc.h"

void loadDT()
{
	loadDTLogin();
	ds = DTStateLogin;
}

void freeDT()
{
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
