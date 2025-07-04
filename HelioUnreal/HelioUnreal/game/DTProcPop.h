#pragma once

#include "iStd.h"

void loadDTProcPop();
void freeDTProcPop();
void drawDTProcPop(float dt);
bool keyDTProcPop(iKeyStat stat, iPoint point);

void showToast(iPoint center, const char* szFormat, ...);

// ==========================================
// popCaption
// ==========================================
extern iStrTex* stCaption;

void createPopCaption();
void freePopCaption();
void drawPopCaption(float dt);
void showPopCaption(bool show, iPoint center = iPointZero, const char* str = NULL);

// 수주가 들어온 내용...
// (우선 순위에 따라) 진행여부...
// 
// notice 기계 고장
// 국가 안전 전기 소비량 경고
// 
// 창을 닫을때... 전달 메시지...
//

// ==========================================
// popUnit
// ==========================================
void createPopUnit();
void freePopUnit();
void drawPopUnit(float dt);
bool keyPopUnit(iKeyStat stat, iPoint point);

void showPopUnit(bool show, int unitIndex);

