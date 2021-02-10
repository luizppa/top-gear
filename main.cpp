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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

int init() {
  srand(time(nullptr));
  top_gear::environment::init_environment();

  /* Seting path */
  ALLEGRO_PATH* p = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
  al_change_directory(al_path_cstr(p, '\\'));
  al_destroy_path(p);

  /* Settings */
  top_gear::environment::setup_display();
  top_gear::environment::setup_sounds();
  top_gear::sounds::music = top_gear::sounds::set_music(TITLE_MUSIC);
  al_attach_audio_stream_to_mixer(top_gear::sounds::music, al_get_default_mixer());
  top_gear::sounds::start_music(top_gear::sounds::music, true);
  top_gear::environment::setup_events();
  if (!top_gear::environment::display || !top_gear::environment::queue || !top_gear::environment::timer || !top_gear::sounds::music) {
    printf("Failed setup.\n");
    return -1;
  }

  /* Presentation screen */
  top_gear::display::draw_text(top_gear::fonts::DISKUN_60, top_gear::colors::BLUE, SCREEN_WIDTH/2, (SCREEN_HEIGHT/2)-60, ALLEGRO_ALIGN_CENTRE, "LUIZ PHILIPPE", false);
  top_gear::display::draw_text(top_gear::fonts::PIXEL_28, top_gear::colors::WHITE, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, ALLEGRO_ALIGN_CENTRE, "presents", true);
  al_rest(3);
  al_flush_event_queue(top_gear::environment::queue);
  top_gear::display::draw_title();
  return 0;
}

/* Clear environment */
void deinit() {
  top_gear::display::destroy_bitmaps();
  top_gear::sounds::destroy_sounds();
  top_gear::environment::destroy_environment();
}

int main() {
  init();
  int op;
  while (true) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(top_gear::environment::queue, &ev);
    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      break;
    }
    else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
        deinit();
        return 0;
      }
      else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
        top_gear::sounds::play_sample(top_gear::sounds::MENU_SELECT_SOUND);
        while(true){
          op = top_gear::menus::main_menu();
          if(op == 4){
            deinit();
            return 0;
          }
          else if(op == -1){
            top_gear::display::draw_title();
            break;
          }
        }
      }
    }
  }
  deinit();
  return 0;
}