#pragma once

#include "iStd.h"

void loadDTProcUI();
void freeDTProcUI();
void drawDTProcUI(float dt);
bool keyDTProcUI(iKeyStat stat, iPoint point);

void showDTProcUI(bool show);

// DT의 디스플레이 되는 모든 정보
struct ProcData
{

	DWORD playtimeTotal;// 총 접속 시간
	DWORD playtimeCurr; // 현재 접속 시작한 시간
	DWORD playtimeLast; // 지난 접속 했던 시간

	int unitMake;		// 공장에 들어온 개수
	int unitBroken;		// 소모된 설비 개수
	int unitRun;		// 운행중인 개수 
	int unitRepairing;	// 수리중인 개수
	int unitRepaired;	// 수리 완료된 개수
	int unitReplace;

	int target;			// 생산 목표 개수(수주)
	int made;			// 현재 진행 개수

	float unitMakeTime[10];	// 10개라고 가정
};

extern ProcData* pd;
#define ProcDataSaveFile "./pd.sav"
void loadProcData();
void saveProcData();
