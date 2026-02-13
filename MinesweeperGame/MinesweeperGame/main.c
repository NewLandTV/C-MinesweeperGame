#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MINE -1
#define STEPPED -2
#define MINE_CHECK -3
#define DONT_KNOW -4
#define UNMARKED -5
#define NUM_MINE 7
#define NUM_OF_ROWS 8
#define NUM_OF_COLUMNS 8

int i = 0;
int j = 0;
int numOfStepped = 0;	// Count the number of steps and use it end the game
int boardVisible[NUM_OF_ROWS][NUM_OF_COLUMNS];	// Similar to the board seen by the user
int boardKotae[NUM_OF_ROWS][NUM_OF_COLUMNS];	// Board with numbers and mines (correct answer board)
int obtainedRandoms[NUM_MINE];	// Random numbers for Mines
int elapsed_milli = 0;

clock_t startTime;
clock_t endTime;

void printBoard(int board_visible[NUM_OF_ROWS][NUM_OF_COLUMNS]);
void printUpperNum(int numOfUpper);
void printElapsedMilli();

int processNumMineRandoms();	// The process for obtaining random numbers
int processGame(int row_entered, int column_entered, int check_type);
int canGo(int i, int j);

int main(void)
{
	int checkingRow = -1;
	int checkingColumn = -1;
	int tempType = -1;
	int isContinue = 0;
	int numOfMine = NUM_MINE;
	int tempMineCount = 0;
	int minesMarked = 0;	// The number of mines I marked on the screen

	char input_type_char = '?';

	srand((unsigned int)time(NULL));

	// board initialize
	for (i = 0; i < NUM_OF_ROWS; i++)
	{
		for (j = 0; j < NUM_OF_COLUMNS; j++)
		{
			boardVisible[i][j] = UNMARKED;
			boardKotae[i][j] = UNMARKED;
		}
	}

	// mine planting
	if (processNumMineRandoms() == 1)
	{
		for (i = 0; i < NUM_MINE; i++)
		{
			int mineRow = obtainedRandoms[i] / NUM_OF_ROWS;
			int mineColumn = obtainedRandoms[i] % NUM_OF_ROWS;

			boardKotae[mineRow][mineColumn] = MINE;
		}
	}

	// Creating kotae array with numbers
	for (i = 0; i < NUM_OF_ROWS; i++)
	{
		for (j = 0; j < NUM_OF_COLUMNS; j++)
		{
			// Enter a number if it is bot a land mine
			if (boardKotae[i][j] != MINE)
			{
				tempMineCount = 0;

				// You can also use the canGo(int i, int j) function.

				// upper
				if (i - 1 >= 0)
				{
					if (j - 1 >= 0)
					{
						if (boardKotae[i - 1][j - 1] == MINE)
						{
							tempMineCount++;
						}
					}
					if (boardKotae[i - 1][j] == MINE)
					{
						tempMineCount++;
					}
					if (j + 1 >= 0)
					{
						if (boardKotae[i - 1][j + 1] == MINE)
						{
							tempMineCount++;
						}
					}
				}

				// same line
				if (j - 1 >= 0)
				{
					if (boardKotae[i][j - 1] == MINE)
					{
						tempMineCount++;
					}
				}
				if (j + 1 < NUM_OF_COLUMNS)
				{
					if (boardKotae[i][j + 1] == MINE)
					{
						tempMineCount++;
					}
				}

				// lower
				if (i + 1 < NUM_OF_ROWS)
				{
					if (j - 1 >= 0)
					{
						if (boardKotae[i + 1][j - 1] == MINE)
						{
							tempMineCount++;
						}
					}
					if (boardKotae[i + 1][j] == MINE)
					{
						tempMineCount++;
					}
					if (j + 1 < NUM_OF_COLUMNS)
					{
						if (boardKotae[i + 1][j + 1] == MINE)
						{
							tempMineCount++;
						}
					}

					boardKotae[i][j] = tempMineCount;
				}
			}
		}
	}

	// Game play(Core game progress code)

	// Game Start
	printBoard(boardVisible);	// Print Board First

	startTime = clock();	// Start time measurement

	do
	{
		printf("Total Mines : %d, Mines marked : %d\n\n", NUM_MINE, minesMarked);
		printf("Please enter the location(row, column) and type\n");
		printf("[type]    s : step    f : flag    ? : ?\n");
		printf("e.g. Row 3, column 5, step -> 3 5 s \n");

		scanf_s("%d %d", &checkingRow, &checkingColumn);
		scanf_s(" %c", &input_type_char, sizeof(input_type_char));

		switch (input_type_char)
		{
			case 's':
			case 'S':
				tempType = STEPPED;
				break;
			case 'f':
			case 'F':
				tempType = MINE_CHECK;
				minesMarked++;
				break;
			case '?':
			default:
				tempType = DONT_KNOW;
				break;
		}

		isContinue = processGame(checkingRow, checkingColumn, tempType);

		printBoard(boardVisible);

		printElapsedMilli();
	} while (isContinue == 1 && numOfStepped < (NUM_OF_ROWS * NUM_OF_COLUMNS - numOfMine));

	// When you stepped on a landmine
	if (numOfStepped < (NUM_OF_ROWS * NUM_OF_COLUMNS - numOfMine))
	{
		printBoard(boardVisible);
		printBoard(boardKotae);

		printf("Game Over!\n");

		printElapsedMilli();

		system("pause");
	}
	// When you cleared the game
	else
	{
		printf("Game clear! Congratulations! \n");
		printf("THANK YOU FOR PLAYING!!!\n");
		printf("- JkhTV -\n");

		printElapsedMilli();

		system("pause");
	}

	return 0;
}

void printBoard(int board_visible[NUM_OF_ROWS][NUM_OF_COLUMNS])
{
	system("cls");
	printUpperNum(NUM_OF_COLUMNS);
	printf("  ____________________________\n");

	for (i = 0; i < NUM_OF_ROWS; i++)
	{
		printf("   %d >", i);

		for (j = 0; j < NUM_OF_COLUMNS; j++)
		{
			switch (board_visible[i][j])
			{
				case UNMARKED:
					printf(" _ ");
					break;
				case MINE:
					printf(" * ");
					break;
				case MINE_CHECK:
					printf(" F ");
					break;
				case DONT_KNOW:
					printf(" ? ");
					break;
				case 0:
					printf("   ");
					break;
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
					printf(" %d ", board_visible[i][j]);
					break;
				default:
					printf(" D ");
					break;
			}
		}

		// printf("\n  ____________________________\n");

		printf("\n\n");
	}
}

void printUpperNum(int numOfUpper)
{
	printf("\n ");
	printf("    ");

	for (i = 0; i < numOfUpper; i++)
	{
		printf(" %d ", i);
	}

	printf("\n");
}

void printElapsedMilli()
{
	int s = 0;
	int left_elapsed_milli = 0;

	endTime = clock();
	elapsed_milli = (int)(endTime - startTime);

	s = elapsed_milli / 1000;
	left_elapsed_milli = elapsed_milli % 1000;

	printf("Elapsed time : %d.%03ds\n", s, left_elapsed_milli);
}

int processNumMineRandoms()
{
	int i = 0;
	int j = 0;
	int temp_random = 0;
	int is_duplicated = 0;

	const int num_of_want_nums = NUM_MINE;			//for different number, NUM_OF_WANT_NUMS <= TO_NUM
	const int to_num = NUM_OF_ROWS * NUM_OF_COLUMNS;	//if you want to optaion integer of 0-9,set 10

	do
	{
		is_duplicated = 0;
		temp_random = rand() % to_num;

		for (j = 0; j < i; j++)
		{
			if (temp_random == obtainedRandoms[j])
			{
				is_duplicated = 1;

				break;
			}
		}
		if (is_duplicated == 0)
		{
			obtainedRandoms[i] = temp_random;
			i++;
		}
	} while (i < num_of_want_nums);

	return 1;
}

int processGame(int row_entered, int column_entered, int check_type)
{
	int isLive = 1;

	switch (boardVisible[row_entered][column_entered])
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			return isLive;
			break;
		default:
			boardVisible[row_entered][column_entered] = check_type;
			break;
	}

	if (canGo(row_entered, column_entered) == 0)
	{
		return isLive;
	}

	switch (check_type)
	{
		case STEPPED:
			if (boardKotae[row_entered][column_entered] == MINE)
			{
				printf("You stepped on a land mine.\n");

				boardVisible[row_entered][column_entered] = MINE;

				isLive = 0;

				return isLive;
			}
			else
			{
				numOfStepped++;
				boardVisible[row_entered][column_entered] = boardKotae[row_entered][column_entered];
			}

			if (boardVisible[row_entered][column_entered] == 0)
			{
				if (canGo(row_entered - 1, column_entered - 1))
				{
					processGame(row_entered - 1, column_entered - 1, STEPPED);	//Spread to left upper diagonal ¢Ø
				}
				if (canGo(row_entered - 1, column_entered + 1))
				{
					processGame(row_entered - 1, column_entered - 1, STEPPED);	//Spread across the upper right diagonal ¢Ö
				}
				if (canGo(row_entered + 1, column_entered - 1))
				{
					processGame(row_entered + 1, column_entered - 1, STEPPED);	//Spread to left lower diagonal ¢×
				}
				if (canGo(row_entered + 1, column_entered + 1))
				{
					processGame(row_entered + 1, column_entered + 1, STEPPED);	//Spread across the lower right diagonal ¢Ù
				}

				if (canGo(row_entered - 1, column_entered))
				{
					processGame(row_entered - 1, column_entered, STEPPED);
				}
				if (canGo(row_entered + 1, column_entered))
				{
					processGame(row_entered + 1, column_entered, STEPPED);
				}
				if (canGo(row_entered, column_entered - 1))
				{
					processGame(row_entered, column_entered - 1, STEPPED);
				}
				if (canGo(row_entered, column_entered + 1))
				{
					processGame(row_entered, column_entered + 1, STEPPED);
				}
			}
			break;
		default:
			boardVisible[row_entered][column_entered] = check_type;
			break;
	}

	return isLive;
}

int canGo(int i, int j)
{
	int flag = 1;

	// minus
	if (i < 0 || j < 0)
	{
		flag = 0;
	}

	// over
	if (i >= NUM_OF_ROWS || j >= NUM_OF_COLUMNS)
	{
		flag = 0;
	}

	return flag;
}
