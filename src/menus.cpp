#include <string>
#include <iostream>

#include "../include/menus.hpp"
#include "../include/game.hpp"
#include "../include/sounds.hpp"
#include "../include/colors.hpp"
#include "../include/display.hpp"
#include "../include/fonts.hpp"
#include "../include/environment.hpp"

namespace top_gear {

  namespace menus {

    int mode_selection();
    int car_selection(int mode);
    int color_selection(int mode, int car);
    int map_selection(int mode, ALLEGRO_BITMAP* car);

    ALLEGRO_COLOR get_color(int option_number, int selected_option){
      return option_number == selected_option ? colors::WHITE : colors::YELLOW;
    }

    /**
     * Main menu region
    */
    int render_main_menu(int selected_op){
      display::clear_display(colors::BLUE, false);
      al_draw_bitmap(display::GAME_TITLE, (SCREEN_WIDTH/2)-203, (SCREEN_HEIGHT/2)-250, 0);
      display::draw_text(fonts::PIXEL_28, get_color(0, selected_op), SCREEN_WIDTH/2, (SCREEN_HEIGHT/2)-35, ALLEGRO_ALIGN_CENTRE, "PLAY", false);
      display::draw_text(fonts::PIXEL_28, get_color(1, selected_op), SCREEN_WIDTH/2, (SCREEN_HEIGHT/2)+5, ALLEGRO_ALIGN_CENTRE, "OPTIONS", false);
      display::draw_text(fonts::PIXEL_28, get_color(2, selected_op), SCREEN_WIDTH/2, (SCREEN_HEIGHT/2)+45, ALLEGRO_ALIGN_CENTRE, "CREDITS", false);
      display::draw_text(fonts::PIXEL_28, get_color(3, selected_op), SCREEN_WIDTH/2, (SCREEN_HEIGHT/2)+85, ALLEGRO_ALIGN_CENTRE, "EXIT", true);
    }

    int main_menu_loop(){
      int selected_op = 0, result;
      ALLEGRO_EVENT ev;

      al_flush_event_queue(environment::input_event_queue);
      while(true){
        al_wait_for_event(environment::input_event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
          return MENU_ACTION_QUIT_GAME;
        }

        if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
          switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
              sounds::play_sample(sounds::MENU_BACK_SOUND);
              return MENU_ACTION_GO_BACK;

            case ALLEGRO_KEY_UP:
            case ALLEGRO_KEY_W:
              if(selected_op > 0){
                sounds::play_sample(sounds::MENU_MOVE_SOUND);
                selected_op--;
              }
              break;

            case ALLEGRO_KEY_DOWN:
            case ALLEGRO_KEY_S:
              if(selected_op < 3){
                sounds::play_sample(sounds::MENU_MOVE_SOUND);
                selected_op++;
              }
              break;

            case ALLEGRO_KEY_ENTER:
              sounds::play_sample(sounds::MENU_SELECT_SOUND);
              switch (selected_op) {
                case 0: // Play
                  result = mode_selection();
                  break;

                case 1: // Settings
                  result = options_menu();
                  sounds::set_music(TITLE_MUSIC);
                  break;
              }

              switch (result) {
                case MENU_ACTION_GO_BACK:
                  selected_op = 0;
                  break;
                case MENU_ACTION_QUIT_GAME:
                  return MENU_ACTION_QUIT_GAME;
              }
          }
          render_main_menu(selected_op);
        }
      }
      return MENU_ACTION_DONE;
    }

    /**
     * Main menu entry point
    */
    int main_menu(){
      int result;

      render_main_menu(0);
      result = main_menu_loop();
      display::draw_title();

      return result;
    }

    /**
     * Options region
    */
    void render_options_menu(int selected_op){
      display::clear_display(colors::BLUE, false);
      std::string music_status = environment::music_on ? "ON" : "OFF";
      std::string sounds_status = environment::sounds_on ? "ON" : "OFF";
      std::string collisions_status = environment::collisions ? "ON" : "OFF";
      std::string debug_status = environment::debug ? "ON" : "OFF";

      // Music
      display::draw_text(fonts::PIXEL_28, get_color(0, selected_op), (SCREEN_WIDTH/2)-80, 200+(1*35), ALLEGRO_ALIGN_RIGHT, "Music", false);
      display::draw_text(fonts::PIXEL_28, get_color(0, selected_op), (SCREEN_WIDTH/2)+80, 200+(1*35), ALLEGRO_ALIGN_LEFT, music_status, false);

      // Sounds
      display::draw_text(fonts::PIXEL_28, get_color(1, selected_op), (SCREEN_WIDTH/2)-80, 200+(2*35), ALLEGRO_ALIGN_RIGHT, "Sounds", false);
      display::draw_text(fonts::PIXEL_28, get_color(1, selected_op), (SCREEN_WIDTH/2)+80, 200+(2*35), ALLEGRO_ALIGN_LEFT, sounds_status, false);

      // Collisions
      display::draw_text(fonts::PIXEL_28, get_color(2, selected_op), (SCREEN_WIDTH/2)-80, 200+(3*35), ALLEGRO_ALIGN_RIGHT, "Collisions", false);
      display::draw_text(fonts::PIXEL_28, get_color(2, selected_op), (SCREEN_WIDTH/2)+80, 200+(3*35), ALLEGRO_ALIGN_LEFT, collisions_status, false);

      // Debug
      display::draw_text(fonts::PIXEL_28, get_color(3, selected_op), (SCREEN_WIDTH/2)-80, 200+(4*35), ALLEGRO_ALIGN_RIGHT, "Debug", false);
      display::draw_text(fonts::PIXEL_28, get_color(3, selected_op), (SCREEN_WIDTH/2)+80, 200+(4*35), ALLEGRO_ALIGN_LEFT, debug_status, false);

      al_flip_display();
    }

    int options_menu_loop(){
      int selected_op = 0;
      ALLEGRO_EVENT ev;

      while (true) {
        al_wait_for_event(environment::input_event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
          return MENU_ACTION_QUIT_GAME;
        }

        if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
          if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            sounds::play_sample(sounds::MENU_BACK_SOUND);
            return MENU_ACTION_GO_BACK;
          }

          switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
            case ALLEGRO_KEY_W:
              if(selected_op > 0){
                selected_op--;
                sounds::play_sample(sounds::MENU_MOVE_SOUND);
              }
              break;

            case ALLEGRO_KEY_DOWN:
            case ALLEGRO_KEY_S:
              if(selected_op < 3){
                selected_op++;
                sounds::play_sample(sounds::MENU_MOVE_SOUND);
              }
              break;

            case ALLEGRO_KEY_ENTER:
              sounds::play_sample(sounds::MENU_SELECT_SOUND);
              switch (selected_op) {
                case 0:
                  if(environment::music_on){
                    sounds::turn_of_music(sounds::music);
                  }
                  else{
                    sounds::turn_on_music(sounds::music);
                  }
                  break;

                case 1:
                  environment::sounds_on = !environment::sounds_on;
                  break;

                case 2:
                  environment::collisions = !environment::collisions;
                  break;

                case 3:
                  environment::debug = !environment::debug;
                  break;
              }
              break;
          }
          render_options_menu(selected_op);
        }
      }
      return MENU_ACTION_DONE;
    }

    /**
     * Options menu entry point
    */
    int options_menu(){
      int res;

      render_options_menu(0);
      al_flush_event_queue(environment::input_event_queue);
      res = options_menu_loop();
      return res;
    }

    /**
     * Mode selection region
    */
    void render_mode_selection(int selected_op){
      float square_size = 250.0;

      display::clear_display(colors::BLUE, false);
      display::draw_text(fonts::PIXEL_32, colors::YELLOW, SCREEN_WIDTH/2, 35, ALLEGRO_ALIGN_CENTRE, "GAME MODE", false);

      // Quick play
      al_draw_rounded_rectangle((SCREEN_WIDTH/2)-25-square_size, (SCREEN_HEIGHT/2)-(square_size/2), (SCREEN_WIDTH/2)-25, (SCREEN_HEIGHT/2)+(square_size/2), 0, 0, get_color(0, selected_op), 5);
      display::draw_text(fonts::PIXEL_28, get_color(0, selected_op), (SCREEN_WIDTH/2)-25-(square_size/2), (SCREEN_HEIGHT/2)-14, ALLEGRO_ALIGN_CENTRE, "QUICK PLAY", false);
      
      // Tournament
      al_draw_rounded_rectangle((SCREEN_WIDTH/2)+25, (SCREEN_HEIGHT/2)-(square_size/2), (SCREEN_WIDTH/2)+25+square_size, (SCREEN_HEIGHT/2)+(square_size/2), 0, 0, get_color(1, selected_op), 5);
      display::draw_text(fonts::PIXEL_28, get_color(1, selected_op), (SCREEN_WIDTH/2)+25+(square_size/2), (SCREEN_HEIGHT/2)-14, ALLEGRO_ALIGN_CENTRE, "TOURNAMENT", false);
      switch (selected_op) {
        case 0:
          display::draw_text(fonts::PIXEL_22, colors::WHITE, 30, SCREEN_HEIGHT-37, ALLEGRO_ALIGN_LEFT, "A single race to test your skills", true);
          break;
        case 1:
          display::draw_text(fonts::PIXEL_22, colors::WHITE, 30, SCREEN_HEIGHT-37, ALLEGRO_ALIGN_LEFT, "Join a series of races and make your way to the top!", true);
          break;
      }
    }

    /**
     * Mode selection menu entry point
    */
    int mode_selection(){
      int selected_op = 0, *car, map, result;
      ALLEGRO_EVENT ev;

      render_mode_selection(selected_op);
      al_flush_event_queue(environment::input_event_queue);
      while (true) {
        al_wait_for_event(environment::input_event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
          return MENU_ACTION_QUIT_GAME;
        }
        
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
          if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            sounds::play_sample(sounds::MENU_BACK_SOUND);
            return MENU_ACTION_GO_BACK;
          }

          switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
            case ALLEGRO_KEY_A:
              if(selected_op > 0){
                selected_op--;
                sounds::play_sample(sounds::MENU_MOVE_SOUND);
              }
              break;

            case ALLEGRO_KEY_RIGHT:
            case ALLEGRO_KEY_D:
              if(selected_op < 1){
                selected_op++;
                sounds::play_sample(sounds::MENU_MOVE_SOUND);
              }
              break;

            case ALLEGRO_KEY_ENTER:
              sounds::play_sample(sounds::MENU_SELECT_SOUND);

              switch (selected_op) {

                case 0: // Single race
                  result = car_selection(GAME_MODE_QUICK_PLAY);
                  break;

                case 2: // Tournament
                  result = car_selection(GAME_MODE_TOURNAMENT);
                  break;
              }
              if(result != MENU_ACTION_GO_BACK){
                return result;
              }
          }
          render_mode_selection(selected_op);
        }
      }
      return MENU_ACTION_DONE;
    }

    /**
     * Car selection region
    */
    void render_car_selection(int selected_op){
      float square_width = 340.0, square_height = 220.0, border_tickness = 5;
      float dominus_w = display::get_bitmap_width(display::DOMINUS_GT_ICON_BITMAP), dominus_h = display::get_bitmap_height(display::DOMINUS_GT_ICON_BITMAP);
      float octane_w = display::get_bitmap_width(display::OCTANE_ZSR_ICON_BITMAP), octane_h = display::get_bitmap_height(display::OCTANE_ZSR_ICON_BITMAP);
      float roadhog_w = display::get_bitmap_width(display::ROADHOG_ICON_BITMAP), roadhog_h = display::get_bitmap_height(display::ROADHOG_ICON_BITMAP);
      float maverick_w = display::get_bitmap_width(display::MAVERICK_ICON_BITMAP), maverick_h = display::get_bitmap_height(display::MAVERICK_ICON_BITMAP);
      display::clear_display(colors::BLUE, false);

      // Dominus
      al_draw_rectangle((SCREEN_WIDTH/2)-25-square_width, (SCREEN_HEIGHT/2)-25-square_height, (SCREEN_WIDTH/2)-25, (SCREEN_HEIGHT/2)-25, get_color(0, selected_op), border_tickness);
      display::draw_text(fonts::PIXEL_22, get_color(0, selected_op), (SCREEN_WIDTH/2)-25-(square_width/2), (SCREEN_HEIGHT/2)-25-square_height+12, ALLEGRO_ALIGN_CENTRE, "Dominus GT", false);
      al_draw_bitmap(display::DOMINUS_GT_ICON_BITMAP, (SCREEN_WIDTH/2)-25-((square_width-dominus_w)/2)-dominus_w, (SCREEN_HEIGHT/2)-25-35-dominus_h, 0);

      // Octane
      al_draw_rectangle((SCREEN_WIDTH/2)+25, (SCREEN_HEIGHT/2)-25-square_height, (SCREEN_WIDTH/2)+25+square_width, (SCREEN_HEIGHT/2)-25, get_color(1, selected_op), border_tickness);
      display::draw_text(fonts::PIXEL_28, get_color(1, selected_op), (SCREEN_WIDTH/2)+25+(square_width/2), (SCREEN_HEIGHT/2)-25-square_height+12, ALLEGRO_ALIGN_CENTRE, "Octane ZSR", false);
      al_draw_bitmap(display::OCTANE_ZSR_ICON_BITMAP, (SCREEN_WIDTH/2)+25+((square_width-octane_w)/2), (SCREEN_HEIGHT/2)-25-35-octane_h, 0);

      // Roadhog
      al_draw_rectangle((SCREEN_WIDTH/2)-25-square_width, (SCREEN_HEIGHT/2)+25, (SCREEN_WIDTH/2)-25, (SCREEN_HEIGHT/2)+25+square_height, get_color(2, selected_op), border_tickness);
      display::draw_text(fonts::PIXEL_28, get_color(2, selected_op), (SCREEN_WIDTH/2)-25-(square_width/2), (SCREEN_HEIGHT/2)+25+12, ALLEGRO_ALIGN_CENTRE, "Roadhog", false);
      al_draw_bitmap(display::ROADHOG_ICON_BITMAP, (SCREEN_WIDTH/2)-25-(square_width/2)-(roadhog_w/2), (SCREEN_HEIGHT/2)+25+square_height-35-roadhog_h, 0);

      // Maverick
      al_draw_rectangle((SCREEN_WIDTH/2)+25, (SCREEN_HEIGHT/2)+25, (SCREEN_WIDTH/2)+25+square_width, (SCREEN_HEIGHT/2)+25+square_height, get_color(3, selected_op), border_tickness);
      display::draw_text(fonts::PIXEL_28, get_color(3, selected_op), (SCREEN_WIDTH/2)+25+(square_width/2), (SCREEN_HEIGHT/2)+25+12, ALLEGRO_ALIGN_CENTRE, "Maverick", false);
      al_draw_bitmap(display::MAVERICK_ICON_BITMAP, (SCREEN_WIDTH/2)+25+(square_width/2)-(maverick_w/2), (SCREEN_HEIGHT/2)+25+square_height-35-maverick_h, 0);

      display::draw_text(fonts::PIXEL_32, colors::YELLOW, (SCREEN_WIDTH/2), 30, ALLEGRO_ALIGN_CENTRE, "SELECT YOUR VEICHLE", true);
    }

    /**
     * Car selection menu entry point
    */
    int car_selection(int mode){
      int selected_op = 0, result;
      ALLEGRO_EVENT ev;

      render_car_selection(selected_op);
      al_flush_event_queue(environment::input_event_queue);
      while (true) {
        al_wait_for_event(environment::input_event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
          return MENU_ACTION_QUIT_GAME;
        }

        if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
          switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
              sounds::play_sample(sounds::MENU_BACK_SOUND);
              return MENU_ACTION_GO_BACK;

            case ALLEGRO_KEY_RIGHT:
            case ALLEGRO_KEY_D:
              if(selected_op == 0 || selected_op == 2){
                selected_op++;
                sounds::play_sample(sounds::MENU_MOVE_SOUND);
              }
              break;

            case ALLEGRO_KEY_DOWN:
            case ALLEGRO_KEY_S:
              if(selected_op < 2){
                selected_op += 2;
                sounds::play_sample(sounds::MENU_MOVE_SOUND);
              }
              break;

            case ALLEGRO_KEY_LEFT:
            case ALLEGRO_KEY_A:
              if(selected_op == 1 || selected_op == 3){
                selected_op--;
                sounds::play_sample(sounds::MENU_MOVE_SOUND);
              }
              break;

            case ALLEGRO_KEY_UP:
            case ALLEGRO_KEY_W:
              if(selected_op > 1){
                selected_op -= 2;
                sounds::play_sample(sounds::MENU_MOVE_SOUND);
              }
              break;

            case ALLEGRO_KEY_ENTER:
              sounds::play_sample(sounds::MENU_SELECT_SOUND);
              int result = color_selection(mode, selected_op);
              if(result != MENU_ACTION_GO_BACK){
                return MENU_ACTION_DONE;
              }
              break;
          }
          render_car_selection(selected_op);
        }
      }
      return MENU_ACTION_DONE;
    }

    /**
     * Color selection region
    */
    void render_color_selection(int selected_op){
      float block_size = 150.0;

      ALLEGRO_COLOR color_options[7] = {
        colors::rgb(255, 0, 0),
        colors::rgb(0, 0, 255),
        colors::rgb(0, 255, 0),
        colors::rgb(100, 100, 100),
        colors::rgb(170, 0, 240),
        colors::rgb(247, 165, 0),
        colors::rgb(0, 0, 0)
      };

      display::clear_display(colors::BLUE, false);
      display::draw_text(fonts::PIXEL_32, colors::YELLOW, (SCREEN_WIDTH/2), 35, ALLEGRO_ALIGN_CENTRE, "SELECT YOUR COLOR", false);

      for(int i = -3; i < 4; i++){
        al_draw_filled_rounded_rectangle((SCREEN_WIDTH/2)+(i*(block_size+25.0))-(block_size/2), (SCREEN_HEIGHT/2)-(block_size/2), (SCREEN_WIDTH/2)+(i*(block_size+25))+(block_size/2), (SCREEN_HEIGHT/2)+(block_size/2), 0, 0, color_options[i+3]);
        al_draw_rounded_rectangle((SCREEN_WIDTH/2)+(i*(block_size+25.0))-(block_size/2), (SCREEN_HEIGHT/2)-(block_size/2), (SCREEN_WIDTH/2)+(i*(block_size+25))+(block_size/2), (SCREEN_HEIGHT/2)+(block_size/2), 0, 0, get_color(i+3, selected_op), 5);
      }
      al_flip_display();
    }

    /**
     * Color selection menu entry point
    */
    int color_selection(int mode, int car){
      int selected_op = 0, result;
      ALLEGRO_EVENT ev;

      render_color_selection(selected_op);
      al_flush_event_queue(environment::input_event_queue);
      while (true) {
        al_wait_for_event(environment::input_event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
          return MENU_ACTION_QUIT_GAME;
        }

        if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
          switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
              sounds::play_sample(sounds::MENU_BACK_SOUND);
              return MENU_ACTION_GO_BACK;

            case ALLEGRO_KEY_RIGHT:
            case ALLEGRO_KEY_D:
              if(selected_op < 6){
                selected_op++;
                sounds::play_sample(sounds::MENU_MOVE_SOUND);
              }
              break;

            case ALLEGRO_KEY_LEFT:
            case ALLEGRO_KEY_A:
              if(selected_op > 0){
                selected_op--;
                sounds::play_sample(sounds::MENU_MOVE_SOUND);
              }
              break;

            case ALLEGRO_KEY_ENTER:
              sounds::play_sample(sounds::MENU_SELECT_SOUND);
              result = map_selection(mode, display::get_car_bitmap((CarsTypes)car, selected_op));

              if(result != MENU_ACTION_GO_BACK){
                return result;
              }
              break;
          }
          render_color_selection(selected_op);
        }
      }
      return MENU_ACTION_DONE;
    }

    /**
     * Map selection region
    */
    void render_map_selection(int selected_op){
      int square_w = 500, square_h = 200;
      display::clear_display(colors::BLUE, false);

      display::draw_text(fonts::PIXEL_28, colors::YELLOW, SCREEN_WIDTH/2, 20, ALLEGRO_ALIGN_CENTRE, "MAP SELECTION", false);

      display::draw_text(fonts::PIXEL_22, colors::YELLOW, (SCREEN_WIDTH/2), 100, ALLEGRO_ALIGN_CENTRE, "LAS VEGAS", false);
      al_draw_bitmap(display::LAS_VEGAS_ICON_BITMAP, (SCREEN_WIDTH/2)-(square_w/2), (SCREEN_HEIGHT/2)-(square_h/2), 0);
      al_draw_rounded_rectangle((SCREEN_WIDTH/2)-(square_w/2), (SCREEN_HEIGHT/2)-(square_h/2), (SCREEN_WIDTH/2)+(square_w/2), (SCREEN_HEIGHT/2)+(square_h/2), 0, 0, get_color(0, selected_op), 5);
      al_flip_display();
    }

    void flip_map_selection(int selected_op, bool go_right){
      int square_w = 500, square_h = 200;
      float positions[4], slide_speed = 1.3;

      float frame_offset = (SCREEN_WIDTH*slide_speed)/environment::fps;

      if(go_right) {
        positions[0] = (SCREEN_WIDTH*(1-selected_op));
        positions[1] = (SCREEN_WIDTH*(2-selected_op));
        positions[2] = (SCREEN_WIDTH*(3-selected_op));
        positions[3] = (SCREEN_WIDTH*(4-selected_op));
      }

      else {
        positions[0] = (SCREEN_WIDTH*(-1-selected_op));
        positions[1] = (SCREEN_WIDTH*(-selected_op));
        positions[2] = (SCREEN_WIDTH*(1-selected_op));
        positions[3] = (SCREEN_WIDTH*(2-selected_op));
      }

      al_flush_event_queue(environment::timer_event_queue);
      while((go_right && positions[selected_op] > 0) || (!go_right && positions[selected_op] < 0)){
        ALLEGRO_EVENT ev;
        al_wait_for_event(environment::timer_event_queue, &ev);

        for (int i = 0; i < 4; i++) {
          if(go_right) positions[i] -= frame_offset;
          else positions[i] += frame_offset;
        }

        display::clear_display(colors::BLUE, false);
        display::draw_text(fonts::PIXEL_28, colors::YELLOW, SCREEN_WIDTH/2, 20, ALLEGRO_ALIGN_CENTRE, "MAP SELECTION", false);

        // Lass Vegas
        display::draw_text(fonts::PIXEL_22, colors::YELLOW, positions[0]+(SCREEN_WIDTH/2), 100, ALLEGRO_ALIGN_CENTRE, "LAS VEGAS", false);
        al_draw_bitmap(display::LAS_VEGAS_ICON_BITMAP, positions[0]+(SCREEN_WIDTH/2)-(square_w/2), (SCREEN_HEIGHT/2)-(square_h/2), 0);
        al_draw_rounded_rectangle(positions[0]+(SCREEN_WIDTH/2)-(square_w/2), (SCREEN_HEIGHT/2)-(square_h/2), positions[0]+(SCREEN_WIDTH/2)+(square_w/2), (SCREEN_HEIGHT/2)+(square_h/2), 0, 0, get_color(0, selected_op), 5);
        
        // Bordeaux
        display::draw_text(fonts::PIXEL_22, colors::YELLOW, positions[1]+(SCREEN_WIDTH/2), 100, ALLEGRO_ALIGN_CENTRE, "BORDEAUX", false);
        al_draw_bitmap(display::BORDEAUX_ICON_BITMAP, positions[1]+(SCREEN_WIDTH/2)-(square_w/2), (SCREEN_HEIGHT/2)-(square_h/2), 0);
        al_draw_rounded_rectangle(positions[1]+(SCREEN_WIDTH/2)-(square_w/2), (SCREEN_HEIGHT/2)-(square_h/2), positions[1]+(SCREEN_WIDTH/2)+(square_w/2), (SCREEN_HEIGHT/2)+(square_h/2), 0, 0, get_color(1, selected_op), 5);
        
        // Hiroshima
        display::draw_text(fonts::PIXEL_22, colors::YELLOW, positions[2]+(SCREEN_WIDTH/2), 100, ALLEGRO_ALIGN_CENTRE, "Hiroshima", false);
        al_draw_bitmap(display::HIROSHIMA_ICON_BITMAP, positions[2]+(SCREEN_WIDTH/2)-(square_w/2), (SCREEN_HEIGHT/2)-(square_h/2), 0);
        al_draw_rounded_rectangle(positions[2]+(SCREEN_WIDTH/2)-(square_w/2), (SCREEN_HEIGHT/2)-(square_h/2), positions[2]+(SCREEN_WIDTH/2)+(square_w/2), (SCREEN_HEIGHT/2)+(square_h/2), 0, 0, get_color(2, selected_op), 5);
        
        // Frankfurt
        display::draw_text(fonts::PIXEL_22, colors::YELLOW, positions[3]+(SCREEN_WIDTH/2), 100, ALLEGRO_ALIGN_CENTRE, "FRANKFURT", false);
        al_draw_bitmap(display::FRANKFURT_ICON_BITMAP, positions[3]+(SCREEN_WIDTH/2)-(square_w/2), (SCREEN_HEIGHT/2)-(square_h/2), 0);
        al_draw_rounded_rectangle(positions[3]+(SCREEN_WIDTH/2)-(square_w/2), (SCREEN_HEIGHT/2)-(square_h/2), positions[3]+(SCREEN_WIDTH/2)+(square_w/2), (SCREEN_HEIGHT/2)+(square_h/2), 0, 0, get_color(3, selected_op), 5);
        al_flip_display();
      }
    }

    int map_selection(int mode, ALLEGRO_BITMAP* car){
      ALLEGRO_EVENT ev;
      int selected_op = 0, result;

      render_map_selection(selected_op);
      al_flush_event_queue(environment::input_event_queue);
      while (true) {
        al_wait_for_event(environment::input_event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
          return MENU_ACTION_QUIT_GAME;
        }

        if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
          switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
              sounds::play_sample(sounds::MENU_BACK_SOUND);
              return MENU_ACTION_GO_BACK;

            case ALLEGRO_KEY_RIGHT:
            case ALLEGRO_KEY_D:
              if(selected_op < 3){
                selected_op++;
                sounds::play_sample(sounds::MENU_MOVE_SOUND);
                flip_map_selection(selected_op, true);
              }
              break;

            case ALLEGRO_KEY_LEFT:
            case ALLEGRO_KEY_A:
              if(selected_op > 0){
                selected_op--;
                sounds::play_sample(sounds::MENU_MOVE_SOUND);
                flip_map_selection(selected_op, false);
              }
              break;

            case ALLEGRO_KEY_ENTER:
              sounds::play_sample(sounds::MENU_SELECT_SOUND);
              switch (mode) {
              case GAME_MODE_QUICK_PLAY:
                result = top_gear::play(car, nullptr, 11, (Map)selected_op, true);
                break;
              
              default:
                result = top_gear::tournament(car, 11);
                break;
              }
              sounds::stop_music(sounds::music);
              sounds::music = sounds::set_music(TITLE_MUSIC);
              sounds::start_music(sounds::music, true);
              return result;
          }
        }
      }
      return MENU_ACTION_DONE;
    }

  }

}
