#include "display.h"

// Screen dimensions
int sw = 1300;
int sh = 700;

ALLEGRO_BITMAP *GAME_TITLE = NULL;
ALLEGRO_BITMAP *GAME_ICON = NULL;
ALLEGRO_BITMAP *LAS_VEGAS_LANDSCAPE_BITMAP = NULL;
ALLEGRO_BITMAP *LAS_VEGAS_ROAD_BITMAP = NULL;
ALLEGRO_BITMAP *FINISH_LINE = NULL;
ALLEGRO_BITMAP *ROAD_SIGN_LEFT = NULL;

// Octane assets
ALLEGRO_BITMAP *OCTANE_ZSR_ICON_BITMAP = NULL;
ALLEGRO_BITMAP *OCTANE_ZSR_BLUE_BITMAP = NULL;
ALLEGRO_BITMAP *OCTANE_ZSR_RED_BITMAP = NULL;
ALLEGRO_BITMAP *OCTANE_ZSR_GREEN_BITMAP = NULL;
ALLEGRO_BITMAP *OCTANE_ZSR_PURPLE_BITMAP = NULL;
ALLEGRO_BITMAP *OCTANE_ZSR_GREY_BITMAP = NULL;
ALLEGRO_BITMAP *OCTANE_ZSR_BLACK_BITMAP = NULL;
ALLEGRO_BITMAP *OCTANE_ZSR_YELLOW_BITMAP = NULL;
// Dominus assets
ALLEGRO_BITMAP *DOMINUS_GT_ICON_BITMAP = NULL;
ALLEGRO_BITMAP *DOMINUS_GT_BLUE_BITMAP = NULL;
ALLEGRO_BITMAP *DOMINUS_GT_RED_BITMAP = NULL;
ALLEGRO_BITMAP *DOMINUS_GT_GREEN_BITMAP = NULL;
ALLEGRO_BITMAP *DOMINUS_GT_PURPLE_BITMAP = NULL;
ALLEGRO_BITMAP *DOMINUS_GT_GREY_BITMAP = NULL;
ALLEGRO_BITMAP *DOMINUS_GT_BLACK_BITMAP = NULL;
ALLEGRO_BITMAP *DOMINUS_GT_YELLOW_BITMAP = NULL;
// Roadhog assets
ALLEGRO_BITMAP *ROADHOG_ICON_BITMAP = NULL;
ALLEGRO_BITMAP *ROADHOG_BLUE_BITMAP = NULL;
ALLEGRO_BITMAP *ROADHOG_RED_BITMAP = NULL;
ALLEGRO_BITMAP *ROADHOG_GREEN_BITMAP = NULL;
ALLEGRO_BITMAP *ROADHOG_PURPLE_BITMAP = NULL;
ALLEGRO_BITMAP *ROADHOG_GREY_BITMAP = NULL;
ALLEGRO_BITMAP *ROADHOG_BLACK_BITMAP = NULL;
ALLEGRO_BITMAP *ROADHOG_YELLOW_BITMAP = NULL;

// Initialize bitmaps
void init_bitmaps(){
  GAME_TITLE = al_load_bitmap("assets/images/misc/logo.png");
  GAME_ICON = al_load_bitmap("assets/icons/top-gear-icon.jpg");
  LAS_VEGAS_LANDSCAPE_BITMAP = al_load_bitmap("assets/images/landscapes/las_vegas_landscape.png");
  FINISH_LINE = al_load_bitmap("assets/images/misc/finish.png");
  ROAD_SIGN_LEFT = al_load_bitmap("assets/images/misc/road_sign_left.png");
  LAS_VEGAS_ROAD_BITMAP = al_load_bitmap("assets/images/misc/road.png");
  OCTANE_ZSR_ICON_BITMAP = al_load_bitmap("assets/images/cars/octane_icon.png");
  OCTANE_ZSR_BLUE_BITMAP = al_load_bitmap("assets/images/cars/octane_zsr_blue.png");
  OCTANE_ZSR_RED_BITMAP = al_load_bitmap("assets/images/cars/octane_zsr_red.png");
  OCTANE_ZSR_GREEN_BITMAP = al_load_bitmap("assets/images/cars/octane_zsr_green.png");
  OCTANE_ZSR_PURPLE_BITMAP = al_load_bitmap("assets/images/cars/octane_zsr_purple.png");
  OCTANE_ZSR_GREY_BITMAP = al_load_bitmap("assets/images/cars/octane_zsr_grey.png");
  OCTANE_ZSR_BLACK_BITMAP = al_load_bitmap("assets/images/cars/octane_zsr_black.png");
  OCTANE_ZSR_YELLOW_BITMAP = al_load_bitmap("assets/images/cars/octane_zsr_yellow.png");
  DOMINUS_GT_ICON_BITMAP = al_load_bitmap("assets/images/cars/dominus_icon.png");
  DOMINUS_GT_BLUE_BITMAP = al_load_bitmap("assets/images/cars/dominus_gt_blue.png");
  DOMINUS_GT_RED_BITMAP = al_load_bitmap("assets/images/cars/dominus_gt_red.png");
  DOMINUS_GT_GREEN_BITMAP = al_load_bitmap("assets/images/cars/dominus_gt_green.png");
  DOMINUS_GT_PURPLE_BITMAP = al_load_bitmap("assets/images/cars/dominus_gt_purple.png");
  DOMINUS_GT_GREY_BITMAP = al_load_bitmap("assets/images/cars/dominus_gt_grey.png");
  DOMINUS_GT_BLACK_BITMAP = al_load_bitmap("assets/images/cars/dominus_gt_black.png");
  DOMINUS_GT_YELLOW_BITMAP = al_load_bitmap("assets/images/cars/dominus_gt_yellow.png");
  ROADHOG_ICON_BITMAP = al_load_bitmap("assets/images/cars/roadhog_icon.png");
  ROADHOG_BLUE_BITMAP = al_load_bitmap("assets/images/cars/roadhog_blue.png");
  ROADHOG_RED_BITMAP = al_load_bitmap("assets/images/cars/roadhog_red.png");
  ROADHOG_GREEN_BITMAP = al_load_bitmap("assets/images/cars/roadhog_green.png");
  ROADHOG_PURPLE_BITMAP = al_load_bitmap("assets/images/cars/roadhog_purple.png");
  ROADHOG_GREY_BITMAP = al_load_bitmap("assets/images/cars/roadhog_grey.png");
  ROADHOG_BLACK_BITMAP = al_load_bitmap("assets/images/cars/roadhog_black.png");
  ROADHOG_YELLOW_BITMAP = al_load_bitmap("assets/images/cars/roadhog_yellow.png");
}

// Returns the width of a car bitmap
float get_bitmap_width(ALLEGRO_BITMAP* bitmap){
  if(bitmap == OCTANE_ZSR_BLUE_BITMAP || bitmap == OCTANE_ZSR_RED_BITMAP || bitmap == OCTANE_ZSR_GREEN_BITMAP || bitmap == OCTANE_ZSR_PURPLE_BITMAP || bitmap == OCTANE_ZSR_GREY_BITMAP || bitmap == OCTANE_ZSR_BLACK_BITMAP || bitmap == OCTANE_ZSR_YELLOW_BITMAP) return 166.0;
  else if(bitmap == DOMINUS_GT_BLUE_BITMAP || bitmap == DOMINUS_GT_RED_BITMAP || bitmap == DOMINUS_GT_GREEN_BITMAP || bitmap == DOMINUS_GT_PURPLE_BITMAP || bitmap == DOMINUS_GT_GREY_BITMAP || bitmap == DOMINUS_GT_BLACK_BITMAP || bitmap == DOMINUS_GT_YELLOW_BITMAP) return 166.0;
  else if(bitmap == ROADHOG_BLUE_BITMAP || bitmap == ROADHOG_RED_BITMAP || bitmap == ROADHOG_GREEN_BITMAP || bitmap == ROADHOG_PURPLE_BITMAP || bitmap == ROADHOG_GREY_BITMAP || bitmap == ROADHOG_BLACK_BITMAP || bitmap == ROADHOG_YELLOW_BITMAP) return 166.0;
  else return 0.0;
}

// Returns the height of a car bitmap
float get_bitmap_height(ALLEGRO_BITMAP* bitmap){
  if(bitmap == OCTANE_ZSR_BLUE_BITMAP || bitmap == OCTANE_ZSR_RED_BITMAP || bitmap == OCTANE_ZSR_GREEN_BITMAP || bitmap == OCTANE_ZSR_PURPLE_BITMAP || bitmap == OCTANE_ZSR_GREY_BITMAP || bitmap == OCTANE_ZSR_BLACK_BITMAP || bitmap == OCTANE_ZSR_YELLOW_BITMAP) return 120.0;
  else if(bitmap == DOMINUS_GT_BLUE_BITMAP || bitmap == DOMINUS_GT_RED_BITMAP || bitmap == DOMINUS_GT_GREEN_BITMAP || bitmap == DOMINUS_GT_PURPLE_BITMAP || bitmap == DOMINUS_GT_GREY_BITMAP || bitmap == DOMINUS_GT_BLACK_BITMAP || bitmap == DOMINUS_GT_YELLOW_BITMAP) return 103.0;
  else if(bitmap == ROADHOG_BLUE_BITMAP || bitmap == ROADHOG_RED_BITMAP || bitmap == ROADHOG_GREEN_BITMAP || bitmap == ROADHOG_PURPLE_BITMAP || bitmap == ROADHOG_GREY_BITMAP || bitmap == ROADHOG_BLACK_BITMAP || bitmap == ROADHOG_YELLOW_BITMAP) return 131.0;
  else return 0.0;
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

// Get car bitmap
/*
  Cars:
    0 = Octane
    1 = Dominus
    2 = Roadhog
  Colors:
    0 = Red
    1 = Blue
    2 = Green
    3 = Grey
    4 = Purple
    5 = Yellow
    6 = Black
*/
ALLEGRO_BITMAP* get_car(int car, int color){
  // return DOMINUS_GT_PURPLE_BITMAP;
  switch (car) {
    case 1:
      switch (color) {
        case 0:
          return DOMINUS_GT_RED_BITMAP;
        case 1:
          return DOMINUS_GT_BLUE_BITMAP;
        case 2:
          return DOMINUS_GT_GREEN_BITMAP;
        case 3:
          return DOMINUS_GT_GREY_BITMAP;
        case 4:
          return DOMINUS_GT_PURPLE_BITMAP;
        case 5:
          return DOMINUS_GT_YELLOW_BITMAP;
        case 6:
          return DOMINUS_GT_BLACK_BITMAP;
      }
      break;
    case 2:
      switch (color) {
        case 0:
          return OCTANE_ZSR_RED_BITMAP;
        case 1:
          return OCTANE_ZSR_BLUE_BITMAP;
        case 2:
          return OCTANE_ZSR_GREEN_BITMAP;
        case 3:
          return OCTANE_ZSR_GREY_BITMAP;
        case 4:
          return OCTANE_ZSR_PURPLE_BITMAP;
        case 5:
          return OCTANE_ZSR_YELLOW_BITMAP;
        case 6:
          return OCTANE_ZSR_BLACK_BITMAP;
      }
      break;
    case 3:
      switch (color) {
        case 0:
          return ROADHOG_RED_BITMAP;
        case 1:
          return ROADHOG_BLUE_BITMAP;
        case 2:
          return ROADHOG_GREEN_BITMAP;
        case 3:
          return ROADHOG_GREY_BITMAP;
        case 4:
          return ROADHOG_PURPLE_BITMAP;
        case 5:
          return ROADHOG_YELLOW_BITMAP;
        case 6:
          return ROADHOG_BLACK_BITMAP;
      }
      break;
  }
}

// Free pointers
void destroy_bitmaps(){
  al_destroy_bitmap(GAME_TITLE);
  al_destroy_bitmap(GAME_ICON);
  al_destroy_bitmap(LAS_VEGAS_LANDSCAPE_BITMAP);
  al_destroy_bitmap(LAS_VEGAS_ROAD_BITMAP);
  al_destroy_bitmap(FINISH_LINE);
  al_destroy_bitmap(ROAD_SIGN_LEFT);
  al_destroy_bitmap(OCTANE_ZSR_ICON_BITMAP);
  al_destroy_bitmap(OCTANE_ZSR_RED_BITMAP);
  al_destroy_bitmap(OCTANE_ZSR_BLUE_BITMAP);
  al_destroy_bitmap(OCTANE_ZSR_GREEN_BITMAP);
  al_destroy_bitmap(OCTANE_ZSR_GREY_BITMAP);
  al_destroy_bitmap(OCTANE_ZSR_PURPLE_BITMAP);
  al_destroy_bitmap(OCTANE_ZSR_BLACK_BITMAP);
  al_destroy_bitmap(OCTANE_ZSR_YELLOW_BITMAP);
  al_destroy_bitmap(DOMINUS_GT_ICON_BITMAP);
  al_destroy_bitmap(DOMINUS_GT_RED_BITMAP);
  al_destroy_bitmap(DOMINUS_GT_BLUE_BITMAP);
  al_destroy_bitmap(DOMINUS_GT_GREEN_BITMAP);
  al_destroy_bitmap(DOMINUS_GT_GREY_BITMAP);
  al_destroy_bitmap(DOMINUS_GT_PURPLE_BITMAP);
  al_destroy_bitmap(DOMINUS_GT_BLACK_BITMAP);
  al_destroy_bitmap(DOMINUS_GT_YELLOW_BITMAP);
  al_destroy_bitmap(ROADHOG_ICON_BITMAP);
  al_destroy_bitmap(ROADHOG_BLUE_BITMAP);
  al_destroy_bitmap(ROADHOG_RED_BITMAP);
  al_destroy_bitmap(ROADHOG_GREEN_BITMAP);
  al_destroy_bitmap(ROADHOG_PURPLE_BITMAP);
  al_destroy_bitmap(ROADHOG_GREY_BITMAP);
  al_destroy_bitmap(ROADHOG_BLACK_BITMAP);
  al_destroy_bitmap(ROADHOG_YELLOW_BITMAP);
}
