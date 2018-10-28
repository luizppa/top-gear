#include <stdio.h>
#include "helpers/environment.h"
#include "helpers/menus.h"
#include "helpers/game.h"

#include <allegro5/allegro5.h>
#include <allegro5/error.h>
#include <allegro5/events.h>
#include <allegro5/fullscreen_mode.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

int init();
void deinit();

int main() {
  init();
  int op;
  while (true) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(queue, &ev);
    // Events handling
    // Close window
    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      break;
    }
    else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      // Quit game
      if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
        deinit();
        return 0;
      }
      // Go to main menu
      else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
        while(true){
          op = main_menu();
          // Quit game
          if(op == 4){
            deinit();
            return 0;
          }
          // Back to title
          else if(op == -1){
            draw_title();
            break;
          }
          switch (op) {
            // Start race
            case 1:
              op = play();
              stop_music(music);
              music = set_music(TITLE_MUSIC);
              start_music(music, true);
              break;
          }
          // Quit game
          if(op == 4){
            deinit();
            return 0;
          }
        }
      }
    }
  }
  deinit();
  return 0;
}

int init() {

  // Instaling plugins
  init_environment();

  // Seting path
  ALLEGRO_PATH* p = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
  al_change_directory(al_path_cstr(p, '\\'));
  al_destroy_path(p);
  p = 0;

  // Music setings
  music = set_music(TITLE_MUSIC);
  al_attach_audio_stream_to_mixer(music, al_get_default_mixer());
  start_music(music, true);

  // Display settings
  setup_display();

  // Events setings
  setup_events();
  if (!display || !queue || !timer || !music) {
    printf("Failed setup.\n");
    return -1;
  }

  // Presentation screen
  draw_text(DISKUN_FONT, 56, BLUE, sw/2, (sh/2)-56, ALLEGRO_ALIGN_CENTRE, "LUIZ PHILIPPE", false);
  draw_text(PIXEL_FONT, 26, WHITE, sw/2, sh/2, ALLEGRO_ALIGN_CENTRE, "presents", true);
  al_rest(4);

  // Title screen
  draw_title();

}

void deinit() {
  destroy_bitmaps();
  al_destroy_timer(timer);
  al_destroy_display(display);
  al_destroy_event_queue(queue);
  al_destroy_audio_stream(music);
  clearenv();
}
