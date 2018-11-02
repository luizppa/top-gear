#include "display.h"

// Screen dimensions
int sw = 1300;
int sh = 700;

ALLEGRO_BITMAP *GAME_TITLE = NULL;
ALLEGRO_BITMAP *GAME_ICON = NULL;
ALLEGRO_BITMAP *GAME_CAR_BITMAP = NULL;
ALLEGRO_BITMAP *OPONENT_CAR_BITMAP = NULL;
ALLEGRO_BITMAP *LAS_VEGAS_LANDSCAPE_BITMAP = NULL;
ALLEGRO_BITMAP *LAS_VEGAS_ROAD_BITMAP = NULL;

// Initialize bitmaps
void init_bitmaps(){
  GAME_TITLE = al_load_bitmap("assets/images/logo.png");
  GAME_ICON = al_load_bitmap("assets/icons/top-gear-icon.jpg");
  GAME_CAR_BITMAP = al_load_bitmap("assets/images/car.png");
  OPONENT_CAR_BITMAP = al_load_bitmap("assets/images/oponent_car.png");
  LAS_VEGAS_LANDSCAPE_BITMAP = al_load_bitmap("assets/images/las_vegas_landscape.png");
  LAS_VEGAS_ROAD_BITMAP = al_load_bitmap("assets/images/road.png");
}

// Draw a message to the screen
void draw_text(char* font_name, int size, ALLEGRO_COLOR color, int position_x, int position_y, int align, char* text, bool flip){
  ALLEGRO_FONT *selected_font = font(font_name, size);
  al_draw_text(selected_font, color, position_x, position_y, align, text);
  if(flip) al_flip_display();
  al_destroy_font(selected_font);
}

// Clear the display to a given color
void clear_display(ALLEGRO_COLOR color, bool flip){
  al_clear_to_color(color);
  if(flip) al_flip_display();
}

// Draw title screen
void draw_title(){
  clear_display(BLUE, false);
  al_draw_bitmap(GAME_TITLE, (sw/2)-203, (sh/2)-182, 0);
  draw_text(PIXEL_FONT, 22, YELLOW, sw/2, sh/2+30, ALLEGRO_ALIGN_CENTRE, "Press enter to continue...", false);
  draw_text(PIXEL_FONT, 22, WHITE, sw/2, sh/2+85, ALLEGRO_ALIGN_CENTRE, "Licensed by... no one actually", false);
  draw_text(PIXEL_FONT, 22, WHITE, sw/2, sh/2+110, ALLEGRO_ALIGN_CENTRE, "i may be fined for inappropriate use of image.", false);
  draw_text(PIXEL_FONT, 16, WHITE, 10, sh-26, ALLEGRO_ALIGN_LEFT, "I'm sorry for skipping all those classes.", true);
}

// Free pointers
void destroy_bitmaps(){
  al_destroy_bitmap(GAME_TITLE);
  al_destroy_bitmap(GAME_ICON);
  al_destroy_bitmap(GAME_CAR_BITMAP);
  al_destroy_bitmap(OPONENT_CAR_BITMAP);
  al_destroy_bitmap(LAS_VEGAS_LANDSCAPE_BITMAP);
  al_destroy_bitmap(LAS_VEGAS_ROAD_BITMAP);
}
