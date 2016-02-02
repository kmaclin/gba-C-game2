//Kelsey Maclin
#include "mylib.h"
#include "text.h"
#include "game.h"
#include <stdio.h>

void delay(int n);

int main() {

	REG_DISPCTL = MODE3 | BG2_ENABLE;
	//game logic in game.c

	while(1)
	{
		intro();
		game();
	}
	return 0;
}


