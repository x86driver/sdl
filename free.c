#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>          /* for videodev2.h */
#include <ft2build.h>
#include <freetype2/freetype/freetype.h>
#include <locale.h>
#include <wchar.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600
#define FONT_SIZE 64
#define FONT_FILE "/usr/share/fonts/truetype/ttf-droid/DroidSans.ttf"
#define START_X 0
#define START_Y (FONT_SIZE + 4)

FT_Library library;
FT_Face face;
FT_GlyphSlot slot;
FT_Vector pen;

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

static inline void draw_xy(int x, int y,
            unsigned char r,
            unsigned char g,
            unsigned char b)
{
    draw_pixel(x, y, r, g, b);
}

void draw_bitmap( FT_Bitmap*  bitmap,
		  FT_Int      x,
		  FT_Int      y)
{
	FT_Int  i, j, p, q;
	FT_Int  x_max = x + bitmap->width;
	FT_Int  y_max = y + bitmap->rows;


	for ( i = x, p = 0; i < x_max; i++, p++ )
	{
		for ( j = y, q = 0; j < y_max; j++, q++ )
		{
			if ( i >= WIDTH || j >= HEIGHT )
			  continue;

            if (bitmap->buffer[q * bitmap->width + p])
                draw_xy(i, j, 255, 0, 0);
		}
	}
}

void drawtext(wchar_t *text)
{
	setlocale(LC_CTYPE, "zh_TW.UTF-8");

	FT_Init_FreeType( &library );
	FT_New_Face( library, FONT_FILE, 0, &face );
	FT_Set_Char_Size( face, 0, FONT_SIZE * 64,
			  100, 100 );

	slot = face->glyph;

	pen.x = START_X * 64;
	pen.y = ( HEIGHT - START_Y ) * 64;

	FT_Error error = FT_Select_Charmap( face, FT_ENCODING_UNICODE);
	if ( error != 0 ) {
		printf("select font error");
		perror("select font");
		exit(1);
	}

	int num_chars, n;
	num_chars = wcslen( text );

	for ( n = 0; n < num_chars; ++n )
	{
		FT_Set_Transform( face, NULL, &pen );
		FT_Load_Char( face, text[n], FT_LOAD_RENDER );

		draw_bitmap( &slot->bitmap,
			     slot->bitmap_left,
			     HEIGHT - slot->bitmap_top );

		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
	}

	// move to new line
	pen.y -= FONT_SIZE *2 * 64;
	pen.x  = 50 * 64;


	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

void draw()
{
	wchar_t text[80];
	swprintf(text, sizeof(text)/sizeof(wchar_t), L"Guck");
	drawtext(text);
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    g_pDisplaySurface =
      SDL_SetVideoMode(WIDTH,HEIGHT,0,SDL_ANYFORMAT);

    draw();

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
