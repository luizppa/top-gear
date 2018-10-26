#include <stdio.h>
#include "helpers/environment.h"
#include "helpers/sounds.h"
#include "helpers/display.h"
#include "helpers/colors.h"

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
  while (true) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(queue, &ev);
    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      break;
    }
    else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
      if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) break;
      else if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
        clear_display(BLUE);
      }
    }
  }
  deinit();
  return 0;
}

int init() {

  // Setings
  if (!al_init()) {
    printf("Allegro init failed.\n");
    return 1;
  }
  if (!al_install_audio() || !al_install_keyboard() || !al_install_mouse() || !al_reserve_samples(3)) {
    printf("Install failed.\n");
    return 2;
  }
  if (!al_init_font_addon() || !al_init_ttf_addon() || !al_init_acodec_addon() || !al_init_video_addon() || !al_init_image_addon() || !al_init_primitives_addon()) {
    printf("Addon init failed.\n");
    return 5;
  }

  ALLEGRO_PATH* p = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
  al_change_directory(al_path_cstr(p, '\\'));
  al_destroy_path(p);
  p = 0;

  // Music setings
  music = set_music(TITLE_MUSIC);
  al_attach_audio_stream_to_mixer(music, al_get_default_mixer());
  start_music(music, true);

  // Display settings
  init_colors();
  // windowed
  al_set_new_display_flags(ALLEGRO_WINDOWED);
  display = al_create_display(sw, sh);
  // fullscreen
  // TODO: fix fullscreen renderization
  // al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
  // al_set_new_display_flags(ALLEGRO_FULLSCREEN);
  // display = al_create_display(disp_data.width, disp_data.height);
  al_set_window_title(display, "Top Gear");

  queue = al_create_event_queue();
  timer = al_create_timer(1.0 / 60.0);
  if (!display || !queue || !timer || !music) {
    printf("Failed setup.\n");
    return -1;
  }

  // Presentation screen
  draw_text(DISKUN_FONT, 56, BLUE, sw/2, (sh/2)-56, ALLEGRO_ALIGN_CENTRE, "LUIZ PHILIPPE");
  draw_text(PIXEL_FONT, 26, WHITE, sw/2, sh/2, ALLEGRO_ALIGN_CENTRE, "presents");
  al_rest(4.5);

  // Title screen
  clear_display(rgb(0, 0, 0));
  draw_text(NINTENDO_FONT, 56, ORANGE, sw/2, (sh/2)-28, ALLEGRO_ALIGN_CENTRE, "Top Gear");
  draw_text(PIXEL_FONT, 26, RED, sw/2, sh/2+70, ALLEGRO_ALIGN_CENTRE, "Press enter to continue...");


  al_register_event_source(queue, al_get_display_event_source(display));
  al_register_event_source(queue, al_get_timer_event_source(timer));
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_reserve_samples(2);

}

void deinit() {
  al_destroy_display(display);
  al_destroy_audio_stream(music);
  clearenv();
}
