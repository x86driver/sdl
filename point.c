#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

#define SCREEN_WIDTH 800
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

void draw_point2()
{
    int i, ch, x, y;
    for(i = 0; i < 1; i++)
    {
        int mindiff = INT_MAX, minch = -1, ch;
        for(ch = 0; ch < 3; ch++)
        {
            int diff = 0;
            for(y = 0; y < 7; y++)
            {
                for(x = 0; x < 5; x++)
                {
                    int newx, newy, r2;
                    newx = 35.0 + (x + 6 * i) * 218.0 / 34.0 + y * 5.0 / 6.0 + 0.5;
                    newy = 33.0 - (x + 6 * i) * 18.0 / 34.0 + y * 42.0 / 6.0 + 0.5;
		    draw_xy(newx, newy, 255, 0, 0);
		    draw_xy(x+6*ch, y, 0, 255, 0);
//                    printf("(%d, %d) ==> (%d, %d)\n", newx, newy, x+6*ch, y);
                                        //                    getpixel(tmp, newx, newy, &r, &g, &b);
                                        //                    getpixel(font->img, x + 6 * ch, y, &r2, &g, &b);
                                        //                    diff += (r - r2) * (r - r2);
                 }
            }
        }
    }
}

void draw_point()
{
#define SSIZE 3
#define HEIGHT 3
#define WIDTH 3

    int x, y, i, j, val;
    for(y = SSIZE/2; y < HEIGHT - SSIZE/2; y++)
        for(x = SSIZE/2; x < WIDTH - SSIZE/2; x++)
        {
            val = 0;
            for(i = 0; i < SSIZE; i++)
                for(j = 0; j < SSIZE; j++)
                {
//                    getpixel(img, x + j - SSIZE/2, y + i - SSIZE/2, &r, &g, &b);
		    draw_xy(x+j-SSIZE/2, y+i-SSIZE/2, 255, 0, 0);
		    printf("(%d, %d)\n", x+j-SSIZE/2, y+i-SSIZE/2);
//                    val += r;
                }

            i = val / (SSIZE * SSIZE);
//            setpixel(dst, x, y, i, i, i);
        }
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    g_pDisplaySurface =
      SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,0,SDL_ANYFORMAT);

    draw_point();

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
