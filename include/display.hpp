#ifndef DISPLAY_H
#define DISPLAY_H

#include "./car.hpp"

#include <string>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define SCREEN_WIDTH 1300
#define SCREEN_HEIGHT 700

namespace top_gear {

    namespace display {

        // TODO: Replace sw and sh variables with SCREEN_WIDTH and SCREEN_HEIGHT constants across all the project
        extern int sw;
        extern int sh;

        extern ALLEGRO_DISPLAY_MODE disp_data;

        // Miscelanious
        extern ALLEGRO_BITMAP *GAME_ICON;
        extern ALLEGRO_BITMAP *GAME_TITLE; // 406x182
        extern ALLEGRO_BITMAP *LAS_VEGAS_LANDSCAPE_BITMAP; // 2600x200
        extern ALLEGRO_BITMAP *LAS_VEGAS_ROAD_BITMAP; // 1300x500
        extern ALLEGRO_BITMAP *FINISH_LINE; // 1295x691
        extern ALLEGRO_BITMAP *ROAD_SIGN; // 150x150
        extern ALLEGRO_BITMAP *ROCK_BITMAP; // 140x132
        extern ALLEGRO_BITMAP *TIRES_1_BITMAP;
        extern ALLEGRO_BITMAP *TIRES_2_BITMAP;
        extern ALLEGRO_BITMAP *BOX_1_BITMAP;
        extern ALLEGRO_BITMAP *BOX_2_BITMAP;

        // Landscapes
        extern ALLEGRO_BITMAP *LAS_VEGAS_LANDSCAPE_BITMAP; // 3000x200
        extern ALLEGRO_BITMAP *LAS_VEGAS_ICON_BITMAP; // 500x200
        extern ALLEGRO_BITMAP *HIROSHIMA_LANDSCAPE_BITMAP; // 3000x200
        extern ALLEGRO_BITMAP *HIROSHIMA_ICON_BITMAP; // 500x200
        extern ALLEGRO_BITMAP *BORDEAUX_LANDSCAPE_BITMAP; // 3000x200
        extern ALLEGRO_BITMAP *BORDEAUX_ICON_BITMAP; // 500x200
        extern ALLEGRO_BITMAP *FRANKFURT_LANDSCAPE_BITMAP; // 3000x200
        extern ALLEGRO_BITMAP *FRANKFURT_ICON_BITMAP; // 500x200

        // Cars
        // Octane
        extern ALLEGRO_BITMAP *OCTANE_ZSR_ICON_BITMAP; // 187x88
        extern ALLEGRO_BITMAP *OCTANE_ZSR_BLUE_BITMAP; // 166x120
        extern ALLEGRO_BITMAP *OCTANE_ZSR_RED_BITMAP; // 166x120
        extern ALLEGRO_BITMAP *OCTANE_ZSR_GREEN_BITMAP; // 166x120
        extern ALLEGRO_BITMAP *OCTANE_ZSR_PURPLE_BITMAP; // 166x120
        extern ALLEGRO_BITMAP *OCTANE_ZSR_GREY_BITMAP; // 166x120
        extern ALLEGRO_BITMAP *OCTANE_ZSR_BLACK_BITMAP; // 166x120
        extern ALLEGRO_BITMAP *OCTANE_ZSR_YELLOW_BITMAP; // 166x120
        // Dominus
        extern ALLEGRO_BITMAP *DOMINUS_GT_ICON_BITMAP; // 239x70
        extern ALLEGRO_BITMAP *DOMINUS_GT_BLUE_BITMAP; // 166x103
        extern ALLEGRO_BITMAP *DOMINUS_GT_RED_BITMAP; // 166x103
        extern ALLEGRO_BITMAP *DOMINUS_GT_GREEN_BITMAP; // 166x103
        extern ALLEGRO_BITMAP *DOMINUS_GT_PURPLE_BITMAP; // 166x103
        extern ALLEGRO_BITMAP *DOMINUS_GT_GREY_BITMAP; // 166x103
        extern ALLEGRO_BITMAP *DOMINUS_GT_BLACK_BITMAP; // 166x103
        extern ALLEGRO_BITMAP *DOMINUS_GT_YELLOW_BITMAP; // 166x103
        // Roadhog
        extern ALLEGRO_BITMAP *ROADHOG_ICON_BITMAP; // 233x116
        extern ALLEGRO_BITMAP *ROADHOG_BLUE_BITMAP; // 166x131
        extern ALLEGRO_BITMAP *ROADHOG_RED_BITMAP; // 166x131
        extern ALLEGRO_BITMAP *ROADHOG_GREEN_BITMAP; // 166x131
        extern ALLEGRO_BITMAP *ROADHOG_PURPLE_BITMAP; // 166x131
        extern ALLEGRO_BITMAP *ROADHOG_GREY_BITMAP; // 166x131
        extern ALLEGRO_BITMAP *ROADHOG_BLACK_BITMAP; // 166x131
        extern ALLEGRO_BITMAP *ROADHOG_YELLOW_BITMAP; // 166x131
        // Maverick
        extern ALLEGRO_BITMAP *MAVERICK_ICON_BITMAP; // 269x108
        extern ALLEGRO_BITMAP *MAVERICK_BLUE_BITMAP; // 166x102
        extern ALLEGRO_BITMAP *MAVERICK_RED_BITMAP; // 166x102
        extern ALLEGRO_BITMAP *MAVERICK_GREEN_BITMAP; // 166x102
        extern ALLEGRO_BITMAP *MAVERICK_PURPLE_BITMAP; // 166x102
        extern ALLEGRO_BITMAP *MAVERICK_GREY_BITMAP; // 166x102
        extern ALLEGRO_BITMAP *MAVERICK_BLACK_BITMAP; // 166x102
        extern ALLEGRO_BITMAP *MAVERICK_YELLOW_BITMAP; // 166x102

        void init_bitmaps();
        float get_bitmap_width(ALLEGRO_BITMAP* bitmap);
        float get_bitmap_height(ALLEGRO_BITMAP* bitmap);
        void draw_text(ALLEGRO_FONT *selected_font, ALLEGRO_COLOR color, int position_x, int position_y, int align, char* text, bool flip);
        void draw_text(ALLEGRO_FONT *selected_font, ALLEGRO_COLOR color, int position_x, int position_y, int align, std::string text, bool flip);
        void clear_display(ALLEGRO_COLOR color, bool flip);
        void draw_splash();
        void draw_title();
        void destroy_bitmaps();
        ALLEGRO_BITMAP* get_car_bitmap(CarsTypes car, int color);

    }

}

#endif