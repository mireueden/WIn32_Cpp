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


// 수주가 들어온 내용
// (우선순위에 따라) 진행 여부
// 
// notice 특정 기계 고장
// 전기 소비량 경고