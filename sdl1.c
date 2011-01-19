#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH 1800
#define SCREEN_HEIGHT 600

SDL_Surface* g_pDisplaySurface = NULL;
SDL_Event g_Event;
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
    g_Color=SDL_MapRGB(g_pDisplaySurface->format, r, g, b);
    pData = (char*)g_pDisplaySurface->pixels;
    pData += (y*g_pDisplaySurface->pitch);
    pData += (x*g_pDisplaySurface->format->BytesPerPixel);
    memcpy(pData, &g_Color, g_pDisplaySurface->format->BytesPerPixel);
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

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    g_pDisplaySurface =
      SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,0,SDL_ANYFORMAT);

    draw_sine();

    for(;;)
    {
	if(SDL_PollEvent(&g_Event)==0)
	{
/*
	    int x,y;
	    x = rand() % SCREEN_WIDTH;
	    y = rand() % SCREEN_HEIGHT;
	    g_Red=rand()%256;
	    g_Green=rand()%256;
	    g_Blue=rand()%256;
	    draw_pixel(x, y, g_Red, g_Green, g_Blue);
*/
	    SDL_UpdateRect(g_pDisplaySurface,0,0,0,0);
	}
	else
	{
	    if(g_Event.type==SDL_QUIT) break;
	}
    }
    return(0);
}
