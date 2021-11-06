/*
 *  Made with
 *         ______   ___    ___
 *        /\  _  \ /\_ \  /\_ \
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *    By Elias Pschernig and Trent Gamblin.
 *
 *    Author: Luiz Philippe (https://github.com/luizppa & https://luizppa.com)
 */

#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/error.h>
#include <allegro5/events.h>
#include <allegro5/fullscreen_mode.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "include/environment.hpp"
#include "include/menus.hpp"
#include "include/sounds.hpp"
#include "include/display.hpp"
#include "include/fonts.hpp"
#include "include/colors.hpp"
#include "include/setup.hpp"

int main() {
  int result;

  if(!setup::init()){
    std::cout << "Setup failed" << std::endl;
    return 1;
  }
  
  top_gear::display::draw_splash();
  al_flush_event_queue(top_gear::environment::input_event_queue);
  while (true) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(top_gear::environment::input_event_queue, &ev);

    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      break;
    }

    else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
        break;
      }
      else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
        top_gear::sounds::play_sample(top_gear::sounds::MENU_SELECT_SOUND);
        result = top_gear::menus::main_menu();
        
        if(result == MENU_ACTION_QUIT_GAME){
          break;
        }

        else if(result == MENU_ACTION_GO_BACK){
          top_gear::display::draw_title();
        }
      }
    }
  }

  setup::finalize();
  return 0;
}