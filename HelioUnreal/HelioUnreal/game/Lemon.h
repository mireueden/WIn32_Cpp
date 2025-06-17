#pragma once

#include "iStd.h"

void loadLemon();
void freeLemon();
void drawLemon(float dt);
void keyLemon(iKeyStat stat, iPoint point);

enum GameState
{
	GameStateMenu,
	GameStateProc,
	GameStateResult
};

extern GameState gameState;
