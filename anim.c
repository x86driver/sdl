#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "sge.h"

#define SCREEN_WIDTH    480
#define SCREEN_HEIGHT   272

#define WIDTH           SCREEN_WIDTH
#define HEIGHT          SCREEN_HEIGHT

SDL_Surface* screen = NULL;
SDL_Rect g_Rect;
Uint8 g_Red, g_Green, g_Blue;
Uint32 g_Color;

unsigned char image[SCREEN_WIDTH][SCREEN_HEIGHT];

int centerX = SCREEN_WIDTH + 50;
//int centerX = SCREEN_WIDTH / 2;
int centerY = SCREEN_HEIGHT / 2;

void draw_pixel(int x, int y,
		unsigned char r,
		unsigned char g,
		unsigned char b)
{
    char *pData;
    g_Color=SDL_MapRGB(screen->format, r, g, b);
    pData = (char*)screen->pixels;
    pData += (y*screen->pitch);
    pData += (x*screen->format->BytesPerPixel);
    memcpy(pData, &g_Color, screen->format->BytesPerPixel);
}

static inline void draw_xy(int x, int y,
			unsigned char r,
			unsigned char g,
			unsigned char b)
{
	draw_pixel(x + centerX, centerY - y, r, g, b);
}

void draw_sine()
{
	double i = 0.0;
	const double factor = 30.0;
	const double size = 16.3;
	for (; i < size; i += 1/factor) {
		draw_xy(i * factor, (int)(sin(i) * factor), 255, 0, 0);
	}	
}

void draw()
{
    int halfHeight;
    Uint32 color;

    int sBaseColor = 0xff000000;
    int sColors[] = {0x0000ff, 0x00ff00, 0xff0000};
    int sIntervals[] = {0x000001, 0x000100, 0x010000};

    static int mCount = HEIGHT / 2;
    static int mColorSelect = 0;

    int i;

//    color = SDL_MapRGB(screen->format, 255, 0, 0);
    color = sColors[mColorSelect];
    int interval = sIntervals[mColorSelect];

    halfHeight = HEIGHT / 2;

    SDL_Rect rect;
    rect.x = rect.y = 0;
    rect.w = WIDTH;
    rect.h = HEIGHT;
    SDL_FillRect(screen, &rect, 0);

    for (i = 0; i < halfHeight; ++i) {
        int diff = (((mCount - i) * 4) * interval) % color;
        sge_Rect(screen, halfHeight - i, (halfHeight - 1) - i,
             (WIDTH - halfHeight) + i, halfHeight + i, sBaseColor + diff);
//        printf("%d, %d, %d, %d, %u\n", halfHeight - i, (halfHeight - 1) - i,
//             (WIDTH - halfHeight) + i, halfHeight + i, color + diff);
    }

    ++mCount;
    mCount = mCount % 0xff;
    if (mCount < halfHeight) {
        mCount = halfHeight;

        mColorSelect++;
        if (mColorSelect > 2) {
            mColorSelect = 0;
        }
    }
}

int main(int argc, char* argv[])
{
    int done = 0;

    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    screen =
      SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,0,SDL_ANYFORMAT);
    SDL_Event ev;

//    draw_sine();
    draw();

    while (!done) {

        SDL_Delay(1);

    	if (SDL_PollEvent(&ev) == 0) {
            draw();
    	    SDL_UpdateRect(screen,0,0,0,0);
            continue;
    	}

        switch (ev.type) {
            case SDL_KEYDOWN:
            case SDL_QUIT:
                done = 1;
                break;
        }
    }

    SDL_Quit();
    return 0;
}
