#include "../include/display.hpp"
#include "../include/car.hpp"
#include "../include/fonts.hpp"
#include "../include/environment.hpp"

namespace top_gear {

  namespace display {

    ALLEGRO_DISPLAY_MODE disp_data;

    // Miscelanious
    ALLEGRO_BITMAP *GAME_TITLE = nullptr;
    ALLEGRO_BITMAP *GAME_ICON = nullptr;
    ALLEGRO_BITMAP *LAS_VEGAS_ROAD_BITMAP = nullptr;
    ALLEGRO_BITMAP *FINISH_LINE = nullptr;
    ALLEGRO_BITMAP *ROAD_SIGN = nullptr;
    ALLEGRO_BITMAP *ROCK_BITMAP = nullptr;
    ALLEGRO_BITMAP *TIRES_1_BITMAP = nullptr;
    ALLEGRO_BITMAP *TIRES_2_BITMAP = nullptr;
    ALLEGRO_BITMAP *BOX_1_BITMAP = nullptr;
    ALLEGRO_BITMAP *BOX_2_BITMAP = nullptr;

    // landscapes
    ALLEGRO_BITMAP *LAS_VEGAS_LANDSCAPE_BITMAP = nullptr;
    ALLEGRO_BITMAP *LAS_VEGAS_ICON_BITMAP = nullptr;
    ALLEGRO_BITMAP *HIROSHIMA_LANDSCAPE_BITMAP = nullptr;
    ALLEGRO_BITMAP *HIROSHIMA_ICON_BITMAP = nullptr;
    ALLEGRO_BITMAP *BORDEAUX_LANDSCAPE_BITMAP = nullptr;
    ALLEGRO_BITMAP *BORDEAUX_ICON_BITMAP = nullptr;
    ALLEGRO_BITMAP *FRANKFURT_LANDSCAPE_BITMAP = nullptr;
    ALLEGRO_BITMAP *FRANKFURT_ICON_BITMAP = nullptr;

    // Octane assets
    ALLEGRO_BITMAP *OCTANE_ZSR_ICON_BITMAP = nullptr;
    ALLEGRO_BITMAP *OCTANE_ZSR_BLUE_BITMAP = nullptr;
    ALLEGRO_BITMAP *OCTANE_ZSR_RED_BITMAP = nullptr;
    ALLEGRO_BITMAP *OCTANE_ZSR_GREEN_BITMAP = nullptr;
    ALLEGRO_BITMAP *OCTANE_ZSR_PURPLE_BITMAP = nullptr;
    ALLEGRO_BITMAP *OCTANE_ZSR_GREY_BITMAP = nullptr;
    ALLEGRO_BITMAP *OCTANE_ZSR_BLACK_BITMAP = nullptr;
    ALLEGRO_BITMAP *OCTANE_ZSR_YELLOW_BITMAP = nullptr;
    // Dominus assets
    ALLEGRO_BITMAP *DOMINUS_GT_ICON_BITMAP = nullptr;
    ALLEGRO_BITMAP *DOMINUS_GT_BLUE_BITMAP = nullptr;
    ALLEGRO_BITMAP *DOMINUS_GT_RED_BITMAP = nullptr;
    ALLEGRO_BITMAP *DOMINUS_GT_GREEN_BITMAP = nullptr;
    ALLEGRO_BITMAP *DOMINUS_GT_PURPLE_BITMAP = nullptr;
    ALLEGRO_BITMAP *DOMINUS_GT_GREY_BITMAP = nullptr;
    ALLEGRO_BITMAP *DOMINUS_GT_BLACK_BITMAP = nullptr;
    ALLEGRO_BITMAP *DOMINUS_GT_YELLOW_BITMAP = nullptr;
    // Roadhog assets
    ALLEGRO_BITMAP *ROADHOG_ICON_BITMAP = nullptr;
    ALLEGRO_BITMAP *ROADHOG_BLUE_BITMAP = nullptr;
    ALLEGRO_BITMAP *ROADHOG_RED_BITMAP = nullptr;
    ALLEGRO_BITMAP *ROADHOG_GREEN_BITMAP = nullptr;
    ALLEGRO_BITMAP *ROADHOG_PURPLE_BITMAP = nullptr;
    ALLEGRO_BITMAP *ROADHOG_GREY_BITMAP = nullptr;
    ALLEGRO_BITMAP *ROADHOG_BLACK_BITMAP = nullptr;
    ALLEGRO_BITMAP *ROADHOG_YELLOW_BITMAP = nullptr;
    // Maverick assets
    ALLEGRO_BITMAP *MAVERICK_ICON_BITMAP = nullptr;
    ALLEGRO_BITMAP *MAVERICK_BLUE_BITMAP = nullptr;
    ALLEGRO_BITMAP *MAVERICK_RED_BITMAP = nullptr;
    ALLEGRO_BITMAP *MAVERICK_GREEN_BITMAP = nullptr;
    ALLEGRO_BITMAP *MAVERICK_PURPLE_BITMAP = nullptr;
    ALLEGRO_BITMAP *MAVERICK_GREY_BITMAP = nullptr;
    ALLEGRO_BITMAP *MAVERICK_BLACK_BITMAP = nullptr;
    ALLEGRO_BITMAP *MAVERICK_YELLOW_BITMAP = nullptr;

    // Initialize bitmaps
    void init_bitmaps(){
      // Miscelanious
      GAME_TITLE = al_load_bitmap("../assets/images/misc/logo.png");
      GAME_ICON = al_load_bitmap("../assets/icons/top-gear-icon.jpg");
      FINISH_LINE = al_load_bitmap("../assets/images/misc/finish.png");
      ROAD_SIGN = al_load_bitmap("../assets/images/misc/road_sign_left.png");
      LAS_VEGAS_ROAD_BITMAP = al_load_bitmap("../assets/images/misc/road.png");
      ROCK_BITMAP = al_load_bitmap("../assets/images/misc/rock_1.png");
      TIRES_1_BITMAP = al_load_bitmap("../assets/images/misc/tires_1.png");
      TIRES_2_BITMAP = al_load_bitmap("../assets/images/misc/tires_2.png");
      BOX_1_BITMAP = al_load_bitmap("../assets/images/misc/box_1.png");
      BOX_2_BITMAP = al_load_bitmap("../assets/images/misc/box_2.png");

      // Landscapes
      LAS_VEGAS_LANDSCAPE_BITMAP = al_load_bitmap("../assets/images/landscapes/las_vegas_landscape.png");
      LAS_VEGAS_ICON_BITMAP = al_load_bitmap("../assets/images/landscapes/las_vegas_icon.png");
      HIROSHIMA_LANDSCAPE_BITMAP = al_load_bitmap("../assets/images/landscapes/hiroshima_landscape.png");
      HIROSHIMA_ICON_BITMAP = al_load_bitmap("../assets/images/landscapes/hiroshima_icon.png");
      BORDEAUX_LANDSCAPE_BITMAP = al_load_bitmap("../assets/images/landscapes/bordeaux_landscape.png");
      BORDEAUX_ICON_BITMAP = al_load_bitmap("../assets/images/landscapes/bordeaux_icon.png");
      FRANKFURT_LANDSCAPE_BITMAP = al_load_bitmap("../assets/images/landscapes/frankfurt_landscape.png");
      FRANKFURT_ICON_BITMAP = al_load_bitmap("../assets/images/landscapes/frankfurt_icon.png");

      // Cars
      // Octane
      OCTANE_ZSR_ICON_BITMAP = al_load_bitmap("../assets/images/cars/octane_icon.png");
      OCTANE_ZSR_BLUE_BITMAP = al_load_bitmap("../assets/images/cars/octane_zsr_blue.png");
      OCTANE_ZSR_RED_BITMAP = al_load_bitmap("../assets/images/cars/octane_zsr_red.png");
      OCTANE_ZSR_GREEN_BITMAP = al_load_bitmap("../assets/images/cars/octane_zsr_green.png");
      OCTANE_ZSR_PURPLE_BITMAP = al_load_bitmap("../assets/images/cars/octane_zsr_purple.png");
      OCTANE_ZSR_GREY_BITMAP = al_load_bitmap("../assets/images/cars/octane_zsr_grey.png");
      OCTANE_ZSR_BLACK_BITMAP = al_load_bitmap("../assets/images/cars/octane_zsr_black.png");
      OCTANE_ZSR_YELLOW_BITMAP = al_load_bitmap("../assets/images/cars/octane_zsr_yellow.png");
      // Dominus
      DOMINUS_GT_ICON_BITMAP = al_load_bitmap("../assets/images/cars/dominus_icon.png");
      DOMINUS_GT_BLUE_BITMAP = al_load_bitmap("../assets/images/cars/dominus_gt_blue.png");
      DOMINUS_GT_RED_BITMAP = al_load_bitmap("../assets/images/cars/dominus_gt_red.png");
      DOMINUS_GT_GREEN_BITMAP = al_load_bitmap("../assets/images/cars/dominus_gt_green.png");
      DOMINUS_GT_PURPLE_BITMAP = al_load_bitmap("../assets/images/cars/dominus_gt_purple.png");
      DOMINUS_GT_GREY_BITMAP = al_load_bitmap("../assets/images/cars/dominus_gt_grey.png");
      DOMINUS_GT_BLACK_BITMAP = al_load_bitmap("../assets/images/cars/dominus_gt_black.png");
      DOMINUS_GT_YELLOW_BITMAP = al_load_bitmap("../assets/images/cars/dominus_gt_yellow.png");
      // Roadhog
      ROADHOG_ICON_BITMAP = al_load_bitmap("../assets/images/cars/roadhog_icon.png");
      ROADHOG_BLUE_BITMAP = al_load_bitmap("../assets/images/cars/roadhog_blue.png");
      ROADHOG_RED_BITMAP = al_load_bitmap("../assets/images/cars/roadhog_red.png");
      ROADHOG_GREEN_BITMAP = al_load_bitmap("../assets/images/cars/roadhog_green.png");
      ROADHOG_PURPLE_BITMAP = al_load_bitmap("../assets/images/cars/roadhog_purple.png");
      ROADHOG_GREY_BITMAP = al_load_bitmap("../assets/images/cars/roadhog_grey.png");
      ROADHOG_BLACK_BITMAP = al_load_bitmap("../assets/images/cars/roadhog_black.png");
      ROADHOG_YELLOW_BITMAP = al_load_bitmap("../assets/images/cars/roadhog_yellow.png");
      // Maverick
      MAVERICK_ICON_BITMAP = al_load_bitmap("../assets/images/cars/maverick_icon.png");
      MAVERICK_BLUE_BITMAP = al_load_bitmap("../assets/images/cars/maverick_blue.png");
      MAVERICK_RED_BITMAP = al_load_bitmap("../assets/images/cars/maverick_red.png");
      MAVERICK_GREEN_BITMAP = al_load_bitmap("../assets/images/cars/maverick_green.png");
      MAVERICK_PURPLE_BITMAP = al_load_bitmap("../assets/images/cars/maverick_purple.png");
      MAVERICK_GREY_BITMAP = al_load_bitmap("../assets/images/cars/maverick_grey.png");
      MAVERICK_BLACK_BITMAP = al_load_bitmap("../assets/images/cars/maverick_black.png");
      MAVERICK_YELLOW_BITMAP = al_load_bitmap("../assets/images/cars/maverick_yellow.png");
    }

    // Returns the width of a car bitmap
    float get_bitmap_width(ALLEGRO_BITMAP* bitmap){
      return al_get_bitmap_width(bitmap);
    }

    // Returns the height of a car bitmap
    float get_bitmap_height(ALLEGRO_BITMAP* bitmap){
      return al_get_bitmap_height(bitmap);
    }

    // Draw a message to the screen
    void draw_text(ALLEGRO_FONT *selected_font, ALLEGRO_COLOR color, int position_x, int position_y, int align, char* text, bool flip){
      al_draw_text(selected_font, color, position_x, position_y, align, text);
      if(flip) al_flip_display();
    }

    void draw_text(ALLEGRO_FONT *selected_font, ALLEGRO_COLOR color, int position_x, int position_y, int align, std::string text, bool flip){
      al_draw_text(selected_font, color, position_x, position_y, align, text.c_str());
      if(flip) al_flip_display();
    }

    // Clear the display to a given color
    void clear_display(ALLEGRO_COLOR color, bool flip){
      al_clear_to_color(color);
      if(flip) al_flip_display();
    }

    void draw_splash(){
        /* Presentation screen */
        draw_text(fonts::DISKUN_60, colors::BLUE, SCREEN_WIDTH/2, (SCREEN_HEIGHT/2)-60, ALLEGRO_ALIGN_CENTRE, "LUIZ PHILIPPE", false);
        draw_text(fonts::PIXEL_28, colors::WHITE, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, ALLEGRO_ALIGN_CENTRE, "presents", true);
        al_rest(3);
        al_flush_event_queue(environment::event_queue);
        top_gear::display::draw_title();
    }

    // Draw title screen
    void draw_title(){
      clear_display(colors::BLUE, false);
      al_draw_bitmap(GAME_TITLE, (SCREEN_WIDTH/2)-203, (SCREEN_HEIGHT/2)-182, 0);
      draw_text(fonts::PIXEL_22, colors::YELLOW, SCREEN_WIDTH/2, SCREEN_HEIGHT/2+30, ALLEGRO_ALIGN_CENTRE, "Press enter to continue...", false);
      draw_text(fonts::PIXEL_22, colors::WHITE, SCREEN_WIDTH/2, SCREEN_HEIGHT/2+85, ALLEGRO_ALIGN_CENTRE, "Licensed by... no one actually", false);
      draw_text(fonts::PIXEL_22, colors::WHITE, SCREEN_WIDTH/2, SCREEN_HEIGHT/2+110, ALLEGRO_ALIGN_CENTRE, "i may be fined for inappropriate use of image.", false);
      draw_text(fonts::PIXEL_14, colors::WHITE, 10, SCREEN_HEIGHT-24, ALLEGRO_ALIGN_LEFT, "I'm sorry for skipping all those classes.", true);
    }

    // TODO: Use enum
    /*
      Colors:
        0 = Red
        1 = Blue
        2 = Green
        3 = Grey
        4 = Purple
        5 = Yellow
        6 = Black
    */
    ALLEGRO_BITMAP* get_car_bitmap(CarsTypes car, int color){
      switch (car) {
        case CarsTypes::DOMINUS:
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
        case CarsTypes::OCTANE:
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
        case CarsTypes::ROADHOG:
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
        case CarsTypes::MAVERICK:
          switch (color) {
            case 0:
              return MAVERICK_RED_BITMAP;
            case 1:
              return MAVERICK_BLUE_BITMAP;
            case 2:
              return MAVERICK_GREEN_BITMAP;
            case 3:
              return MAVERICK_GREY_BITMAP;
            case 4:
              return MAVERICK_PURPLE_BITMAP;
            case 5:
              return MAVERICK_YELLOW_BITMAP;
            case 6:
              return MAVERICK_BLACK_BITMAP;
          }
          break;
      }
      return MAVERICK_BLACK_BITMAP;
    }

    // Free pointers
    void destroy_bitmaps(){
      al_destroy_bitmap(GAME_TITLE);
      al_destroy_bitmap(GAME_ICON);
      al_destroy_bitmap(LAS_VEGAS_ROAD_BITMAP);
      al_destroy_bitmap(FINISH_LINE);
      al_destroy_bitmap(ROAD_SIGN);
      al_destroy_bitmap(ROCK_BITMAP);
      al_destroy_bitmap(TIRES_1_BITMAP);
      al_destroy_bitmap(TIRES_2_BITMAP);
      al_destroy_bitmap(BOX_1_BITMAP);
      al_destroy_bitmap(BOX_2_BITMAP);

      al_destroy_bitmap(LAS_VEGAS_LANDSCAPE_BITMAP);
      al_destroy_bitmap(LAS_VEGAS_ICON_BITMAP);
      al_destroy_bitmap(BORDEAUX_LANDSCAPE_BITMAP);
      al_destroy_bitmap(BORDEAUX_ICON_BITMAP);
      al_destroy_bitmap(HIROSHIMA_LANDSCAPE_BITMAP);
      al_destroy_bitmap(HIROSHIMA_ICON_BITMAP);
      al_destroy_bitmap(FRANKFURT_LANDSCAPE_BITMAP);
      al_destroy_bitmap(FRANKFURT_ICON_BITMAP);

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

      al_destroy_bitmap(MAVERICK_ICON_BITMAP);
      al_destroy_bitmap(MAVERICK_BLUE_BITMAP);
      al_destroy_bitmap(MAVERICK_RED_BITMAP);
      al_destroy_bitmap(MAVERICK_GREEN_BITMAP);
      al_destroy_bitmap(MAVERICK_PURPLE_BITMAP);
      al_destroy_bitmap(MAVERICK_GREY_BITMAP);
      al_destroy_bitmap(MAVERICK_BLACK_BITMAP);
      al_destroy_bitmap(MAVERICK_YELLOW_BITMAP);
    }

  }

}
