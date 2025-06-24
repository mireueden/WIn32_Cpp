#pragma once

#include "iStd.h"

void loadDTProc();
void freeDTProc();
void drawDTProc(float dt);
void keyDTProc(iKeyStat stat, iPoint point);

// ======================================
// DTfield
// ======================================

#define FieldWidth 1280
#define FieldHeight 960
extern iPoint offField;

void loadDTfield();
void freeDTfield();
void drawDTfield(float dt);
void keyDTProcfield(iKeyStat stat, iPoint point);