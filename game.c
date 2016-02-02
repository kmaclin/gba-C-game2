//Kelsey Maclin

#include "mylib.h"
#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include "beginning.h"
#include "gameover.h"
#include "speedup.h"

//create enemy struct
typedef struct  
{
	volatile int r;
	volatile int c;
	volatile int oldR;
	volatile int oldC;
	volatile u16 color;

} Enemy;

//creat main player struct
typedef struct  
{
	volatile int r;
	volatile int c;
	volatile int oldR;
	volatile int oldC;
	volatile u16 color;

} Hero;

typedef struct
{
	volatile int r;
	volatile int c;
	volatile int rate;
	volatile int total;
} Life;

//prototypes
int inboundsHero(int r, int c);
int inboundsEnemy(int r, int c);
int isColliding(Enemy enemy, Hero hero);
void drawHearts(Life life);
int countDistance();
Enemy movement(Enemy red);
int checkEnemyGap(int row, int col);
int copyUpValues(Enemy enemies[], int i, int enemCount);
void drawWhite(Hero white, u16 color);
int generateEnemies(Enemy enemies[], int enemCount, int seed, u16 colors[]);


void intro() {
	drawImage3(0, 0, 240, 160, beginning);
	drawString(140, 50, "Start Game: Press Start", BLACK);
	while(!KEY_DOWN_NOW(START));
	delay(30);
	drawRect(0,0,240,160,BLACK);
	delay(60);
	drawString(10, 10, "Rules:", WHITE);
	delay(60);
	drawString(20, 10, "Move left to right to avoid the cars", WHITE);
	delay(60);
	drawString(30, 10, "If you make 200 moves without dying", WHITE);
	drawString(40, 10, "you win!", WHITE);
	delay(60);
	drawString(50, 10, "Try to beat your previous scores!", WHITE);
	delay(60);
	drawString(60, 10, "The red bar is your health.", WHITE);
	drawString(70, 10, "watch out for it.", WHITE);
	delay(60);
	drawString(80, 10, "Press START to play", BLUE);
	while(!KEY_DOWN_NOW(START));
	drawRect(0,0,240,160,BLACK);
	delay(60);
}

void game()
{
	drawRect(0, 79, 80, 160, RGB(5, 5, 8));
	drawRect(0, 59, 20, 160, RGB(3, 10, 3));
	drawRect(0, 159, 20, 160, RGB(3, 10, 3));
	for (int i = 1; i <= 3; i++)
	{
		int k = 20 * i;
		for (int j = 0; j < 160; j++)
		{
			drawRect(0 + j, 79 + k, 1, 4, YELLOW);
			j = j + 7;
		}
	}

	Hero white;
	white.r = 140;
	white.c = 84;
	white.oldR = 140;
	white.oldC = 84;
	white.color = WHITE;
	drawWhite(white, WHITE);
	int countleft = 8;
	int countright = 8;

	Enemy enemies[10];
	int enemCount = 0;
	int seed = 0;

	u16 color[4];
	color[0] = BLUE;
	color[1] = YELLOW;
	color[2] = RED;
	color[3] = GREEN;
	int passes = 0;
	int inv = 0;

	Life life;
	life.r = 1;
	life.c = 1;
	life.total = 10;
	life.rate = 5;
	drawHearts(life);

	int enemyRate = 1;
	int countSteps = 0;
	int increaseRate = 0;
	int increaseRate2 = 0;

	char message[10];
	sprintf(message, "Moves:%d", countSteps);
	drawString(110, 180, message, WHITE);
	while(1)
	{
		drawString(110, 180, message, BLACK);
		sprintf(message, "Moves:%d", countSteps);
		drawString(110, 180, message, WHITE);
		countright++;
		countleft++;
		seed = seed + 1;
		passes++;
		inv++;
		
		if (passes > 10)
		{
			passes = 0;
			enemCount = generateEnemies(enemies, enemCount, seed, color);
		}

		if (countSteps > 50 && !increaseRate)
		{
			seed = 0;
			enemyRate = enemyRate + 1;
			increaseRate = 1;
			drawImage3(10, 10, 220, 140, speedup);
			delay(50);
			drawRect(0,0,240,160,BLACK);
			drawRect(0, 79, 80, 160, RGB(5, 5, 8));
			drawRect(0, 59, 20, 160, RGB(3, 10, 3));
			drawRect(0, 159, 20, 160, RGB(3, 10, 3));
			for (int i = 1; i <= 3; i++)
			{
				int k = 20 * i;
				for (int j = 0; j < 160; j++)
				{
					drawRect(0 + j, 79 + k, 1, 4, YELLOW);
					j = j + 7;
				}
			}
			drawWhite(white, white.color);
			drawRect(life.r, life.c, (life.total * life.rate), 10, RED);
			drawString(110, 180, message, WHITE);
		}

		if (countSteps > 150 && !increaseRate2)
		{
			seed = 0;
			enemyRate = enemyRate + 1;
			increaseRate2 = 1;
			drawImage3(10, 10, 220, 140, speedup);
			delay(50);
			drawRect(0,0,240,160,BLACK);
			drawRect(0, 79, 80, 160, RGB(5, 5, 8));
			drawRect(0, 59, 20, 160, RGB(3, 10, 3));
			drawRect(0, 159, 20, 160, RGB(3, 10, 3));
			for (int i = 1; i <= 3; i++)
			{
				int k = 20 * i;
				for (int j = 0; j < 160; j++)
				{
					drawRect(0 + j, 79 + k, 1, 4, YELLOW);
					j = j + 7;
				}
			}
			drawWhite(white, white.color);
			drawRect(life.r, life.c, (life.total * life.rate), 10, RED);
			drawString(110, 180, message, WHITE);
		}

		if (inv > 16)
		{
			inv = 0;
			for (int i = 0; i < enemCount; i++)
			{
				Enemy enemy = enemies[i];
				if (isColliding(enemy, white))
				{
					drawRect(life.r, life.c, (life.total * life.rate), 10, BLACK);
					life.total = life.total - 1;
					drawRect(life.r, life.c, (life.total * life.rate), 10, RED);
				}
			}
		}
		
		if (life.total == 0)
		{
			break;
		}
		
		if(KEY_DOWN_NOW(LEFT))
		{
			int r = white.r;
			int c = white.c - 21;
			if(inboundsHero(r,c) && (countleft > 7)) 
			{
				white.c = c;
				countleft = 0;
				countSteps++;
			}
		}

		if(KEY_DOWN_NOW(RIGHT)) 
		{
			int r = white.r;
			int c = white.c + 21;
			if(inboundsHero(r,c) && (countright > 7)) 
			{
				white.c = c;
				countright = 0;
				countSteps++;
			}
		}

		if (countSteps == 200)
		{
			break;
		}	

		waitForVblank();

		drawWhite(white, RGB(5, 5, 8));
		drawWhite(white, white.color);
		white.oldR = white.r;
		white.oldC = white.c;


		for (int i = 0; i < enemCount; i++)
		{

			Enemy enemy = enemies[i];
			if ((enemy.r + 15) >= 160)
			{
				drawRect(enemy.r, enemy.c, 9, 15, RGB(5, 5, 8));
				enemCount = copyUpValues(enemies, i, enemCount);
			}
			else
			{
				drawRect(enemy.oldR, enemy.oldC, 9, 15, RGB(5, 5, 8));
				enemy.r = enemy.r + enemyRate;
				drawRect(enemy.r, enemy.c, 9, 15, enemy.color);
				enemy.oldR = enemy.r;
				enemy.oldC = enemy.c;
				enemies[i] = enemy;
			}
		}
	}
	if (countSteps == 200)
	{
		drawRect(0,0,240,160,BLACK);
		delay(60);
		drawString(10, 10, "YOU WIN!", GREEN);
		while(1);
	}
	if (life.total == 0)
	{
		drawRect(0,0,240,160,BLACK);
		delay(60);
		drawImage3(0,0,240,160,gameover);
		while(1);
	}
}
//srand
//int seed = 0;
//seed++;
//srand(seed);
//use bmp files (a multiple of 4 wide)
int copyUpValues(Enemy enemies[], int i, int enemCount)
{
	for (int j = i; j < (enemCount - i); j++)
	{
		enemies[j] = enemies[j + 1];
	}
	enemCount = enemCount - 1;
	return enemCount;
}

int generateEnemies(Enemy enemies[], int enemCount, int seed, u16 colors[])
{
	//int count = 0;
	int arr[4];
	int seedr = seed;

	for(int j = 0; j < 4; j++)
	{
		seedr++;
		srand(seedr);
		int k = rand() % 4;
		if (j == 1)
		{
			while (arr[0] == k)
			{
				k = rand() % 4;
			}
			arr[j] = k;
		}
		else if (j == 2)
		{
			while ((arr[0] == k) || (arr[1] == k))
			{
				k = rand() % 4;
			}
			arr[j] = k;
		}
		else if (j == 3)
		{
			while ((arr[0] == k) || (arr[1] == k) || (arr[2] == k))
			{
				k = rand() % 4;
			}
			arr[j] = k;
		}
		else
		{
			arr[j] = k;
		}
	}
	int iter = 0;
	seedr++;
	srand(seedr);
	int randgen = rand() % 2;
	seedr++;
	srand(seedr);
	int count = 0;
	while (randgen >= 0)
	{
		int chance = rand() % 5;
		int startR = 1;
		int startC = 0;
		int choose = arr[iter];
		Enemy enemy;
		u16 ranCol;

		if ((chance == 2))
		{
			seedr++;

			if (choose == 0)
			{
				startC = 84;
			}
			else if (choose == 1)
			{
				startC = 105;
			}
			else if (choose == 2)
			{
				startC = 126;
			}
			else if (choose == 3)
			{
				startC = 147;
			}

			if (!checkEnemyGap(startR, startC))
			{
				
				if ((enemCount < 10) && (count < 3))
				{
					seedr++;
					srand(seedr);
					ranCol = colors[rand() % 4];
					enemy.r = startR;
					enemy.oldR = startR;
					enemy.c = startC;
					enemy.oldC = startC;
					enemy.color = ranCol;
					enemies[enemCount] = enemy;
					enemCount++;
					count++;
				}
			}
		}
		randgen--;
		iter++;
	}
	return enemCount;
}

void drawWhite(Hero white, u16 color)
{
	if (color == RGB(5, 5, 8))
	{
		drawRect(white.oldR, white.oldC, 9, 15, color);
	}
	else
	{
		drawRect(white.r, white.c, 9, 15, color);
	}
}

void drawHearts(Life life)
{
	drawRect(life.r, life.c, (life.total * life.rate), 10, RED);
}

//checks if all coins have been collected
int countDistance()
{
	return 0;
}

//enemy movement up and down
Enemy movement(Enemy red)
{
	return red;
}
//enemy movement right to left

//checks for enemy collision
int isColliding(Enemy enemy, Hero hero) 
{
	if ((hero.r + 14) < enemy.r)
	{
		return 0;
	}
	if ((enemy.r + 14) < hero.r)
	{
		return 0;
	}
	if (hero.c > (enemy.c + 8))
	{
		return 0;
	}
	if (enemy.c > (hero.c + 8))
	{
		return 0;
	}
	
	return 1;
}

//checks if point is within level boundaries
int inboundsHero(int r, int c) {
	if ((r > 139 && r < 160) && (c > 79 && c < 160))
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

int checkEnemyGap(int row, int col)
{
	int r;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 60; j++)
		{
			r = row + j;
			if (videoBuffer[OFFSET(row + j, col + i, 240)] != RGB(5, 5, 8))
			{
				return r;
			}
		}
	}
	return 0;
}

int inboundsEnemy(int r, int c) {
	if ((r > 0 && r < 160) && (c > 79 && c < 160))
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

//called part2, but is actually part
