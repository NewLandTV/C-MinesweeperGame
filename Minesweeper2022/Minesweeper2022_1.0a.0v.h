#ifndef __Minesweeper_2022__
#define __Minesweeper_2022__

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#define MAX_SIZE_X 70
#define MAX_SIZE_Y 40
#define LAND_MINE_MAX_SIZE_X 8
#define LAND_MINE_MAX_SIZE_Y 8
#define LAND_MINE_MAX 5
#define FALSE 0
#define TRUE 1

int NumLen(int num);

void FillMap(char str[], char str_s,  int max_value);
void EditMap(short x, short y, char str);

void DrawSprite(short x, short y, short size_x, short size_y, const char spr[]);
void DrawNumber(short x, short y, int num);

void DrawLandMineBoard();
void EditLandMineBoard(short x, short y, char str);

void CheckGameState();

void ControlUI();
void ControlInput();

void CreateRandomLandMineBoard();

void StartGame();
void UpdateGame();

void SetConsole();

#endif
