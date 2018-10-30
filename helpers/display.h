#include "fonts.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

extern int sw;
extern int sh;

extern ALLEGRO_BITMAP *GAME_TITLE; // 406x182
extern ALLEGRO_BITMAP *GAME_CAR_BITMAP; // 166x120
extern ALLEGRO_BITMAP *OPONENT_CAR_BITMAP; // 166x120
extern ALLEGRO_BITMAP *GAME_ICON;

void init_bitmaps();
void draw_text(char* font_name, int size, ALLEGRO_COLOR color, int position_x, int position_y, int align, char* text, bool flip);
void clear_display(ALLEGRO_COLOR color, bool flip);
void draw_title();
void destroy_bitmaps();
