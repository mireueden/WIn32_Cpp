#pragma once

#include "iStd.h"

void loadDTProcUI();
void freeDTProcUI();
void drawDTProcUI(float dt);
bool keyDTProcUI(iKeyStat stat, iPoint point);

// DT의 디스플레이 되는 모든 정보
struct ProcData
{
	float playtimeTotal;
	float playtimeCurr;
	float playtimeLast; // 마지막 접속

	int unitMake;		// 공장에 들어온 개수
	int unitBroken;		// 소모된 개수
	int unitRun;		// 운행중인 개수 
	int unitRepairing;	// 수리중인 개수
	int unitRepaired;	// 수리 완료된 개수
	int unitReplace;

	int target;			// 생산 목표 개수(수주)
	int made;			// 현재 진행 개수

	float unitMakeTime[10];	// 10개라고 가정
};