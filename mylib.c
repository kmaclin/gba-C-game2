//Kelsey Maclin
#include "mylib.h"
#include "text.h"

#include <stdio.h>
#include <stdlib.h>

u16 *videoBuffer = (u16 *) 0x6000000;

void setPixel(int r, int c, u16 color) 
{
    if ((r < 160) && (c < 240)) 
    {
        videoBuffer[OFFSET(r , c, 240)] = color;
    }
}

void drawHollowRect(int r, int c, int width, int height, u16 color)
{
    if ((width < (240 - c)) && (height < (160 - r))) 
    {
        for (int i = 0; i < width; i++) 
        {
            videoBuffer[OFFSET(r, c + i, 240)] = color;
            videoBuffer[OFFSET(r + height, c + i, 240)] = color;
        }
        for (int j = 0; j < height; j++) 
        {
            videoBuffer[OFFSET(r + j, c, 240)] = color;
            videoBuffer[OFFSET(r + j, c + width, 240)] = color;
        }
    }
}

void plotLine(int y0, int x0, int y1, int x1, u16 color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    int d = (dy << 1) - dx;
    videoBuffer[OFFSET(x0, y0, 240)] = color;
    int y = y0;

    for (int x = x0 + 1; x <= x1; x++) {
        if (d > 0) 
        {
            y = y+1;
            videoBuffer[OFFSET(x, y, 240)] = color;
            d = d + ((dy << 1) - (dx << 1));
        }
        else {
            videoBuffer[OFFSET(x, y, 240)] = color;
            d = d + (dy << 1);
        }
    }
}

void waitForVblank()
{
    while(SCANLINECOUNTER > 160);
    while(SCANLINECOUNTER < 160);
}

void delay(int n) {
    volatile int x = 0;
    for (int i = 0; i < n* 10000; i++) {
        x++;
    }
}

void drawImage3(int r, int c, int width, int height, const u16* image)
{

    for (int j = 0; j < height; j++)
    {
        DMA[3].src = &image[OFFSET(r + j, c, 240)];
        DMA[3].dst = &videoBuffer[OFFSET(r + j, c, 240)];
        DMA[3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT;
    }
}

void drawRect(int r, int c, int width, int height, u16 color)
{
    for (int j = 0; j < height; j++)
    {
        DMA[3].src = &color;
        DMA[3].dst = &videoBuffer[OFFSET(r + j, c, 240)];
        DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
    }
}