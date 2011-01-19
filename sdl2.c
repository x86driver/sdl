#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>

const int SCREEN_WIDTH=768;
const int SCREEN_HEIGHT=1024;
SDL_Surface* g_pDisplaySurface = NULL;
SDL_Event g_Event;
SDL_Rect g_Rect;
Uint8 g_Red, g_Green, g_Blue;
Uint32 g_Color;

FILE *stdfont, *spcfont, *ascfont;


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

long big5(long Code)
{
    long Offset, LowCode;
    LowCode = Code & 0xff;
    if(LowCode > 0x80) LowCode = (0x7e -0x40) + (LowCode -0xA1+1);
    else LowCode -= 0x40;

    if (Code >= 0x0A440) {
	Offset = (((Code>>8) - 0xA4) * (0x7E - 0x40 + 1 + 0xfe - 0xa1 + 1) + LowCode)*30;
    } else {
	Offset = (((Code>>8) - 0xA1) * (0x7E - 0x40 + 1 + 0xfe - 0xa1 + 1) + LowCode)*30;
    }
    return(Offset);
}

void draw_16x15(int x, int y, unsigned char buf[30])
{
    int i, len;
    int orig_x = x;
    for (len = 0; len < 15; ++len) {
	x = orig_x;
	for (i = 0; i < 8; ++i) {
	    if ((buf[len*2] & 0x80) != 0) {
		draw_pixel(x, y, 255, 255, 255);
	    }
	    ++x;
	    buf[len*2] <<= 1;
	}
	
	for (i = 0; i < 8; ++i) {
	    if ((buf[len*2+1] & 0x80) != 0) {
		draw_pixel(x, y, 255, 255, 255);
	    }
	    ++x;
	    buf[len*2+1] <<= 1;
	}
	++y;
    }
}

void draw_8x15(int x, int y, unsigned char *buf)
{
    int i, len;
    int orig_x = x;
    for (len = 0; len < 15; ++len) {
	x = orig_x;
	for (i = 0; i < 8; ++i) {
	    if ((buf[len] & 0x80) != 0) {
		draw_pixel(x, y, 255, 255, 255);
	    }
	    ++x;
	    buf[len] <<= 1;
	}
	++y;
    }
}

void draw_text(int x, int y, unsigned char *text, int span, int space)
{
    union {
      unsigned char byte[2];
      unsigned short big5;
    } Code;
    unsigned char buf[30];
    int i;
    int orig_x = x;
    for (i = 0; i < strlen((char*)text); ++i) {
	if (text[i] >= 0x0A4) {
	    Code.byte[1] = text[i++];
	    Code.byte[0] = text[i];
	    fseek(stdfont, big5(Code.big5), SEEK_SET);
	    fread(&buf, 30, 1, stdfont);
	    draw_16x15(x, y, buf);
	    x += span;
	} else if (text[i] >= 0x0A0) {
	    Code.byte[1] = text[i++];
	    Code.byte[0] = text[i];
	    fseek(spcfont, big5(Code.big5), SEEK_SET);
	    fread(&buf, 30, 1, spcfont);
	    draw_16x15(x, y, buf);
	    x += span;
	} else if (text[i] < 128) {
	    if (text[i] == '\n') {
		x = orig_x;
		y += space;
	    } else {
		fseek(ascfont, ((int)text[i] * 15), SEEK_SET);
		fread(&buf, 15, 1, ascfont);
		draw_8x15(x, y, buf);
		x += span;
	    }
	}
    }
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    g_pDisplaySurface =
      SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,0,SDL_ANYFORMAT);

    if (argc != 2) {
	printf("Usage: %s Text file\n", argv[0]);
	return 0;
    }

    stdfont = fopen("stdfont.15", "rb");
    if (!stdfont) {
	printf("Can't open stdfont.15\n");
	return 0;
    }
    spcfont = fopen("SPCFONT.15", "rb");
    if (!spcfont) {
	printf("Can't open spcfont.15\n");
	return 0;
    }
    ascfont = fopen("ASCFONT.15", "rb");
    if (!ascfont) {
	printf("Can't open ascfont.15\n");
	return 0;
    }

    FILE *textfp = fopen(argv[1], "rb");
    if (!textfp) {
	printf("Can't open %s\n", argv[1]);
	return 0;
    }
    
    unsigned char text[100];
    fseek(textfp, 0, SEEK_END);
    int textlen = ftell(textfp);
    fseek(textfp, 0, SEEK_SET);
    
    //unsigned char text[] = {0xa7, 0x41, 0xac, 0x4f, '\n', 'D', 0xbd, 0xde, 0};
    fread(&text, textlen, 1, textfp);
    text[textlen] = 0;

    
    draw_text(0, 0, text, 20, 20);

    fclose(stdfont);
    fclose(spcfont);
    fclose(ascfont);
    fclose(textfp);

    /*
    int x,y,i;
    for (i = 0; i < 100; ++i) {
	x = rand() % SCREEN_WIDTH;
	y = rand() % SCREEN_HEIGHT;
	g_Red=rand()%256;
	g_Green=rand()%256;
	g_Blue=rand()%256;
	draw_pixel(x, y, 255, 255, 255);
    }
    */

    for(;;) {
	if(SDL_PollEvent(&g_Event)==0) {
	    SDL_UpdateRect(g_pDisplaySurface,0,0,0,0);
	} else {
	    if(g_Event.type==SDL_QUIT) break;
	}
    }
    return(0);
}
