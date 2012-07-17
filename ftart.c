#include <ft2build.h>
#include FT_FREETYPE_H
#include <strings.h>

int next_color()
{
    static int red = 3;
    static int green = 2;
    static int blue = 0;
    int color;

    color = 18 + (red * 36) + (green * 6) + blue;

    ++blue;
    if (blue > 5) {
	++green;
	blue = 0;
    }

    if (green > 5) {
	++red;
	green = 0;
    }

    if (red > 5) {
	red = 0;
    }

    return color;
}

char next_char()
{
    static const char fuck[] = "Fuck";
    static int index = 0;

    if (index > 3)
	index = 0;

    return fuck[index++];

}

int main(int argc, char **argv)
{
    int psize;
    FT_Library library;
    FT_Face face;
    unsigned int ucode;
    FT_UInt glyph_index;
    int row, pixel;
    char *fp;

    if (argc != 4) {
        return 10;
    }

    ucode = strtol(argv[2], NULL, 16);
    psize = strtol(argv[3], NULL, 10);

    //    printf("unicode +%X size %d \n", ucode, psize);
    //    printf("font %s\n", (fp = rindex(argv[1], '/')) != NULL ?
    //                 fp  + 1 : argv[1]);
    if (FT_Init_FreeType(&library) ||
        FT_New_Face(library, argv[1], 0, &face) ||
        FT_Set_Pixel_Sizes(face, psize, 0)) {
        return 1;
    }

    glyph_index = FT_Get_Char_Index(face, ucode);
    if (glyph_index == 0) {
        return 2;
    }
    if (FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT)) {
        return 3;
    }
    if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO)) {
        return 4;
    }

    /* FIXME: we shall reserve space for low length
     * (face->glyph->bitmap.rows - face->glyph->bitmap_top)
     */
    int color = next_color();
    for (row = 0; row < face->glyph->bitmap.rows; ++row) {
        for (pixel = 0; pixel < face->glyph->bitmap_left; ++pixel)
            printf(" ");
        for (pixel = 0; pixel < face->glyph->bitmap.width; ++pixel){
            if (face->glyph->bitmap.buffer
                    [row * face->glyph->bitmap.pitch +
                     pixel / 8] & (0xC0 >> (pixel % 8))) {
                //printf("\033[44;37m" " " "\033[m");
                printf("\033[48;5;%dm%c\033[m", color, next_char());
            } else
                printf(" ");
        }
        printf("\n");
	color = next_color();
    }
    printf("\033[m");
    return 0;
}
