#include "display.h"

int sw = 1300;
int sh = 700;

ALLEGRO_BITMAP *GAME_TITLE = NULL;
ALLEGRO_BITMAP *GAME_ICON = NULL;

void init_bitmaps(){
  GAME_TITLE = al_load_bitmap("assets/images/logo.png");
  GAME_ICON = al_load_bitmap("assets/icons/top-gear-icon.jpg");
}

// Draw a message to the screen
void draw_text(char* font_name, int size, ALLEGRO_COLOR color, int position_x, int position_y, int align, char* text){
  ALLEGRO_FONT *selected_font = font(font_name, size);
  al_draw_text(selected_font, color, position_x, position_y, align, text);
  al_flip_display();
  al_destroy_font(selected_font);
}

// Clear the display to a given color
void clear_display(ALLEGRO_COLOR color){
  al_clear_to_color(color);
  al_flip_display();
}

void draw_title(){
  clear_display(BLUE);
  al_draw_bitmap(GAME_TITLE, (sw/2)-203, (sh/2)-182, 0);
  draw_text(PIXEL_FONT, 26, RED, sw/2, sh/2+30, ALLEGRO_ALIGN_CENTRE, "Press enter to continue...");
  draw_text(PIXEL_FONT, 16, WHITE, 10, sh-26, ALLEGRO_ALIGN_LEFT, "I'm sorry for skipping all those classes.");
}

void destroy_bitmaps(){
  al_destroy_bitmap(GAME_TITLE);
  al_destroy_bitmap(GAME_ICON);
}
