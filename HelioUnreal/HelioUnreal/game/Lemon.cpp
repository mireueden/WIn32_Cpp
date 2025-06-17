#include "Lemon.h"

#include "LemonMenu.h"
#include "LemonProc.h"

GameState gameState;

void loadLemon()
{
	gameState = GameStateMenu;
	loadLemonMenu();
}

void freeLemon()
{
	switch (gameState) {
	case GameStateMenu: freeLemonMenu(); break;
	case GameStateProc: freeLemonProc(); break;
	}
}

void drawLemon(float dt)
{
	switch (gameState) {
	case GameStateMenu: drawLemonMenu(dt); break;
	case GameStateProc: drawLemonProc(dt); break;
	}
}

void keyLemon(iKeyStat stat, iPoint point)
{
	switch (gameState) {
	case GameStateMenu: keyLemonMenu(stat, point); break;
	case GameStateProc: keyLemonProc(stat, point); break;
	}
}

