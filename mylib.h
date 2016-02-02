//Kelsey Maclin
#include <stdio.h>

#define REG_DISPCTL *(unsigned short *)0x4000000
#define BG2_ENABLE (1<<10)
#define MODE3 3

typedef unsigned short u16;
extern u16 *videoBuffer;

#define OFFSET(row, col, rLength) ((row)*(rLength)+(col))
#define RGB(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define RED RGB(31,0,0)
#define GREEN RGB(0, 31,0)
#define BLUE RGB(0, 0, 31)
#define CYAN RGB(0, 31, 31)
#define MAGENTA RGB(31, 0, 31)
#define YELLOW RGB(31, 31, 0)
#define WHITE RGB(31, 31, 31)
#define BLACK RGB(0, 0, 0)

#define WIDTH 240
#define HEIGHT 160

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)
#define BUTTONS *(volatile unsigned int *)0x4000130

#define A		(1<<0)
#define B		(1<<1)
#define SELECT	(1<<2)
#define START	(1<<3)
#define RIGHT	(1<<4)
#define LEFT	(1<<5)
#define UP		(1<<6)
#define DOWN	(1<<7)
#define R		(1<<8)
#define L		(1<<9)


#define SCANLINECOUNTER *(volatile u16 *)0x4000006

/* DMA */

#define REG_DMA0SAD         *(const volatile u32*)0x40000B0 // source address
#define REG_DMA0DAD         *(volatile u32*)0x40000B4       // destination address
#define REG_DMA0CNT         *(volatile u32*)0x40000B8       // control register

// DMA channel 1 register definitions
#define REG_DMA1SAD         *(const volatile u32*)0x40000BC // source address
#define REG_DMA1DAD         *(volatile u32*)0x40000C0       // destination address
#define REG_DMA1CNT         *(volatile u32*)0x40000C4       // control register

// DMA channel 2 register definitions
#define REG_DMA2SAD         *(const volatile u32*)0x40000C8 // source address
#define REG_DMA2DAD         *(volatile u32*)0x40000CC       // destination address
#define REG_DMA2CNT         *(volatile u32*)0x40000D0       // control register

// DMA channel 3 register definitions
#define REG_DMA3SAD         *(const volatile u32*)0x40000D4 // source address
#define REG_DMA3DAD         *(volatile u32*)0x40000D8       // destination address
#define REG_DMA3CNT         *(volatile u32*)0x40000DC       // control register

typedef struct
{
	const volatile void *src;
	const volatile void *dst;
	unsigned int cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

void drawImage3(int r, int c, int width, int height, const u16* image);
void setPixel(int row, int col, u16 color);
void drawRect(int row, int col, int height, int width, u16 color);
void drawHollowRect(int row, int col, int height, int width, u16 color);
void plotLine(int y0, int x0, int y1, int x1, u16 color);
void waitForVblank();
void delay(int n);