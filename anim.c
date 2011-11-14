#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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

void sge_UpdateRect(SDL_Surface *screen, Sint16 x, Sint16 y, Uint16 w, Uint16 h)
{
//FIXME
//    if(_sge_update!=1 || screen != SDL_GetVideoSurface()){return;}

    if(x>=screen->w || y>=screen->h){return;}

    Sint16 a,b;

    a=w; b=h;

    if(x < 0){x=0;}
    if(y < 0){y=0;}

    if(a+x > screen->w){a=screen->w-x;}
    if(b+y > screen->h){b=screen->h-y;}

    SDL_UpdateRect(screen,x,y,a,b);
}

void _HLine(SDL_Surface *Surface, Sint16 x1, Sint16 x2, Sint16 y, Uint32 Color)
{
    if(x1>x2){Sint16 tmp=x1; x1=x2; x2=tmp;}

#if 0
    //Do the clipping
    #if SDL_VERSIONNUM(SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL) < \
    SDL_VERSIONNUM(1, 1, 5)
    if(y<Surface->clip_miny || y>Surface->clip_maxy || x1>Surface->clip_maxx || x2<Surface->clip_minx)
        return;
    if(x1<Surface->clip_minx)
        x1=Surface->clip_minx;
    if(x2>Surface->clip_maxx)
        x2=Surface->clip_maxx;
    #endif
#endif

    SDL_Rect l;
    l.x=x1; l.y=y; l.w=x2-x1+1; l.h=1;

    SDL_FillRect(Surface, &l, Color);
}

void _VLine(SDL_Surface *Surface, Sint16 x, Sint16 y1, Sint16 y2, Uint32 Color)
{
    if(y1>y2){Sint16 tmp=y1; y1=y2; y2=tmp;}

#if 0
    //Do the clipping
    #if SDL_VERSIONNUM(SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL) < \
    SDL_VERSIONNUM(1, 1, 5)
    if(x<Surface->clip_minx || x>Surface->clip_maxx || y1>Surface->clip_maxy || y2<Surface->clip_miny)
        return;
    if(y1<Surface->clip_miny)
        y1=Surface->clip_miny;
    if(y2>Surface->clip_maxy)
        y2=Surface->clip_maxy;
    #endif
#endif

    SDL_Rect l;
    l.x=x; l.y=y1; l.w=1; l.h=y2-y1+1;

    SDL_FillRect(Surface, &l, Color);
}

void sge_Rect(SDL_Surface *Surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
{
    _HLine(Surface,x1,x2,y1,color);
    _HLine(Surface,x1,x2,y2,color);
    _VLine(Surface,x1,y1,y2,color);
    _VLine(Surface,x2,y1,y2,color);

    sge_UpdateRect(Surface, x1, y1, x2-x1, 1);
    sge_UpdateRect(Surface, x1, y2, x2-x1+1, 1); /* Hmm? */
    sge_UpdateRect(Surface, x1, y1, 1, y2-y1);
    sge_UpdateRect(Surface, x2, y1, 1, y2-y1);
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
