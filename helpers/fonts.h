#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define NINTENDO_FONT     "assets/fonts/nintendo.ttf"
#define PRESS_START_FONT  "assets/fonts/press_start.ttf"
#define PIXEL_FONT        "assets/fonts/pixel.otf"
#define DISKUN_FONT        "assets/fonts/diskun.ttf"

ALLEGRO_FONT* font(char* font_name, int size);
void draw_text(char* font_name, int size, ALLEGRO_COLOR color, int position_x, int position_y, int align, char* text);
ALLEGRO_COLOR rgb(int r, int g, int b);
void clear_display(ALLEGRO_COLOR color);
