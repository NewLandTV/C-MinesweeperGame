////////////////////////////////////////////////////
// made by 장경혁tv
// Name : Minesweeper 2022 
// Version : 1.0a.0v
// Youtube : https://www.youtube.com/channel/UC0lR1aYGgOhFioC3IsazUtw
//
//
#include "Minesweeper2022_1.0a.0v.h"

#define LAND_MINE_MAX_SIZE_X 8
#define LAND_MINE_MAX_SIZE_Y 8

typedef struct _Cursor
{
	short x;
	short y;
	
	short boardX;
	short boardY;
	
	bool isVoid[LAND_MINE_MAX_SIZE_X][LAND_MINE_MAX_SIZE_Y];
} Cursor;

typedef struct _LandMine
{
	int boardIndexX;
	int boardIndexY;
} LandMine;

// Struct variables
Cursor cursor = {MAX_SIZE_X / 2 - LAND_MINE_MAX_SIZE_X, MAX_SIZE_Y / 2 - LAND_MINE_MAX_SIZE_Y, 0, 0};
LandMine landMines;

// Global variables
const char title[17] = "Minesweeper 2022";
const char clearMsg[12] = "Game Clear!";
const char overMsg[11] = "Game Over!";
const char ground_basic_sprite = '#';
const char ground_landMine_sprite = '!';
const char ground_void_sprite = '-';

unsigned int tick;

char originLandMineBoard[LAND_MINE_MAX_SIZE_X * LAND_MINE_MAX_SIZE_Y];
char landMineBoardData[LAND_MINE_MAX_SIZE_X * LAND_MINE_MAX_SIZE_Y];
char mapData[MAX_SIZE_X * MAX_SIZE_Y];

bool gameOver;
bool gameClear;

int main(void)
{
	StartGame();
	
	while (TRUE)
	{
		if (tick + 30 < GetTickCount())
		{
			tick = GetTickCount();
			
			UpdateGame();
			
			if (tick == 0)
			{
				break;
			}
		}
	}
	
	return 0;
}

int NumLen(int num)
{
	int temp = num;
	int len = 0;
	
	if (num == 0)
	{
		return 1;
	}
	else
	{
		while (temp != 0)
		{
			temp /= 10;
			len++;
		}
	}
	
	return len;
}

void FillMap(char str[], char str_s,  int max_value)
{
	for (int i = 0; i < max_value; i++)
	{
		str[i] = str_s;
	}
}

void EditMap(short x, short y, char str)
{
	if (x > 0 && y > 0 && x - 1 < MAX_SIZE_X && y - 1 < MAX_SIZE_Y)
	{
		mapData[(y - 1) * MAX_SIZE_X + x - 1] = str;
	}
}

void DrawSprite(short x, short y, short size_x, short size_y, const char spr[])
{
	for (int i = 0; i < size_y; i++)
	{
		for (int n = 0; n < size_x; n++)
		{
			EditMap(x + n, y + i, spr[i * size_x + n]);
		}
	}
}

void DrawNumber(short x, short y, int num)
{
	int temp = num;
	int len = NumLen(temp);
	int count = len;
	char str[len];
	
	do
	{
		count--;
		str[count] = (char)(temp % 10 + 48);
		temp /= 10;
	}
	while (temp != 0);
	
	DrawSprite(x, y, len, 1, str);
}

void DrawLandMineBoard()
{
	/*for (int i = 0; i < LAND_MINE_MAX_SIZE_X; i++)
	{
		for (int j = 0; j < LAND_MINE_MAX_SIZE_Y; j++)
		{
			EditMap(MAX_SIZE_X / 2 - LAND_MINE_MAX_SIZE_X + j * 2, MAX_SIZE_Y / 2 + i - LAND_MINE_MAX_SIZE_Y, originLandMineBoard[j * i]);
		}
	}*/
	
	for (int i = 0; i < LAND_MINE_MAX_SIZE_X; i++)
	{
		for (int j = 0; j < LAND_MINE_MAX_SIZE_Y; j++)
		{
			EditMap(MAX_SIZE_X / 2 - LAND_MINE_MAX_SIZE_X + j * 2, MAX_SIZE_Y / 2 + i - LAND_MINE_MAX_SIZE_Y, landMineBoardData[j * i]);
		}
	}
}

void EditLandMineBoard(short x, short y, char str)
{
	if (x > 0 && y > 0 && x - 1 < LAND_MINE_MAX_SIZE_X && y - 1 < LAND_MINE_MAX_SIZE_Y)
	{
		landMineBoardData[(y - 1) * LAND_MINE_MAX_SIZE_X + x - 1] = str;
	}
}

void CheckGameState()
{
	if (gameClear)
	{
		tick = 0;
		
		DrawSprite(MAX_SIZE_X / 2 - strlen(clearMsg) / 2, 2, strlen(clearMsg), 1, clearMsg);
	}
	else if (gameOver)
	{
		tick = 0;
		
		DrawSprite(MAX_SIZE_X / 2 - strlen(overMsg) / 2, 2, strlen(overMsg), 1, overMsg);
	}
}

void ControlUI()
{
	DrawSprite(MAX_SIZE_X / 2 - strlen(title) / 2, 2, strlen(title), 1, title);
}

void ControlInput()
{
	// Cursor Move 커서 이동 
	if (GetAsyncKeyState(VK_LEFT) & 0x0001 && cursor.x > MAX_SIZE_X / 2 - LAND_MINE_MAX_SIZE_X)
	{
		cursor.x -= 2;
		cursor.boardX--;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x0001 && cursor.x < MAX_SIZE_X / 2 - LAND_MINE_MAX_SIZE_X + LAND_MINE_MAX_SIZE_X * 2 - 2)
	{
		cursor.x += 2;
		cursor.boardX++;
	}
	if (GetAsyncKeyState(VK_UP) & 0x0001 && cursor.y > MAX_SIZE_Y / 2 - LAND_MINE_MAX_SIZE_Y)
	{
		cursor.y--;
		cursor.boardY--;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x0001 && cursor.y < MAX_SIZE_Y / 2 - LAND_MINE_MAX_SIZE_Y + LAND_MINE_MAX_SIZE_Y - 1)
	{
		cursor.y++;
		cursor.boardY++;
	}
	
	EditMap(cursor.x, cursor.y, '*');
	
	// Edit Land Mine Board	지뢰 찾기 보드 편집 
	if (GetAsyncKeyState(0x0D) & 0x8000 && !cursor.isVoid[cursor.x][cursor.y])
	{	
		cursor.isVoid[cursor.boardX][cursor.boardY] = TRUE;
		EditLandMineBoard(cursor.boardX, cursor.boardY, rand() % 8);
	}
	
	// ESC Game Quit	게임 종료 
	if (GetAsyncKeyState(0x1B) & 0x8000)
	{
		tick = 0;
	}
}

void CreateRandomLandMineBoard()
{
	int ran = 0;
	
	for (int i = 0; i < LAND_MINE_MAX_SIZE_X; i++)
	{
		for (int j = 0; j < LAND_MINE_MAX_SIZE_Y; j++)
		{
			srand((unsigned int)time(NULL));
			ran = rand() % 3;
			
			switch (ran)
			{
				case 0:
			    case 1:
					originLandMineBoard[j * i] = ground_basic_sprite;
					EditLandMineBoard(i, j, ground_basic_sprite);
					break;
				case 2:
					originLandMineBoard[j * i] = ground_landMine_sprite;
					EditLandMineBoard(i, j, ground_landMine_sprite);
					break;
			}
		}
	}
}

void StartGame()
{
	SetConsole();
	
	CreateRandomLandMineBoard();
	
	for (int i = 0; i < LAND_MINE_MAX_SIZE_X; i++)
	{
		for (int j = 0; j < LAND_MINE_MAX_SIZE_Y; j++)
		{
			landMineBoardData[j * i] = ground_basic_sprite;
		}
	}
}

void UpdateGame()
{
	FillMap(mapData, ' ', MAX_SIZE_X * MAX_SIZE_Y);
	
	DrawLandMineBoard();
	ControlUI();
	ControlInput();
	CheckGameState();
	
	puts(mapData);
}

void SetConsole()
{
	system("title Minesweeper 2022");
	system("mode con:cols=70 lines=40");
	
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	ConsoleCursor.bVisible = 0;
	ConsoleCursor.dwSize = 1;
	
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
