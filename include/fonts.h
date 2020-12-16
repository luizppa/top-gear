#include "colors.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define NINTENDO_FONT     "assets/fonts/nintendo.ttf"
#define PRESS_START_FONT  "assets/fonts/press_start.ttf"
#define PIXEL_FONT        "assets/fonts/pixel.otf"
#define DISKUN_FONT       "assets/fonts/diskun.ttf"

extern ALLEGRO_FONT* DISKUN_60;
extern ALLEGRO_FONT* DISKUN_80;
extern ALLEGRO_FONT* DISKUN_100;
extern ALLEGRO_FONT* PIXEL_14;
extern ALLEGRO_FONT* PIXEL_22;
extern ALLEGRO_FONT* PIXEL_28;
extern ALLEGRO_FONT* PIXEL_32;

void init_fonts();
ALLEGRO_FONT* font(char* font_name, int size);
void destroy_fonts();
