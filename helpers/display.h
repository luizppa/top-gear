#include "fonts.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

void draw_text(char* font_name, int size, ALLEGRO_COLOR color, int position_x, int position_y, int align, char* text);
ALLEGRO_COLOR rgb(int r, int g, int b);
void clear_display(ALLEGRO_COLOR color);
