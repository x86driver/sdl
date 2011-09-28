#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272

SDL_Surface* g_pDisplaySurface = NULL;
SDL_Event g_Event;
SDL_Rect g_Rect;
Uint8 g_Red, g_Green, g_Blue;
Uint32 g_Color;

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

static void draw_line()
{
    int i;
    for (i = 0; i < 100; ++i)
        draw_pixel(100+i, 100, 255, 0, 0);
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    g_pDisplaySurface =
      SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,0,SDL_ANYFORMAT);

    draw_line();

    for(;;)
    {
	if(SDL_PollEvent(&g_Event)==0)
	{
	    SDL_UpdateRect(g_pDisplaySurface,0,0,0,0);
	}
	else
	{
	    if(g_Event.type==SDL_QUIT) break;
	}
    }
    return(0);
}
