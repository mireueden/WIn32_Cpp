#pragma once

#include "iStd.h"

void loadDTProc();
void freeDTProc();
void drawDTProc(float dt);
void keyDTProc(iKeyStat stat, iPoint point);

// ====================================== 
// DTField
// ====================================== 
#define FieldWidth 1280
#define FieldHeight 960
extern iPoint offField;

extern bool dragField;
extern iPoint prevField;

extern uint8 fieldTile[32 * 24];
extern int fieldX, fieldY, fieldW, fieldH;
#define displayFieldTile 1

void loadDTField();
void freeDTField();
void drawDTField(float dt);
void keyDTField(iKeyStat stat, iPoint point);
