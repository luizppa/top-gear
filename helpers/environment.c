#include "environment.h"

int sw = 1300;
int sh = 700;
float fps = 60.0;

ALLEGRO_DISPLAY_MODE disp_data;
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE* queue = NULL;
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_AUDIO_STREAM *music = NULL;
ALLEGRO_KEYBOARD_STATE* key_state = NULL;

void init_environment(){
  if (!al_init()) {
    printf("Allegro init failed.\n");
    exit(1);
  }
  if (!al_install_audio() || !al_install_keyboard() || !al_install_mouse() || !al_reserve_samples(3)) {
    printf("Install failed.\n");
    exit(2);
  }
  if (!al_init_font_addon() || !al_init_ttf_addon() || !al_init_acodec_addon() || !al_init_video_addon() || !al_init_image_addon() || !al_init_primitives_addon()) {
    printf("Addon init failed.\n");
    exit(5);
  }
}

void setup_display(){
  // windowed
  al_set_new_display_flags(ALLEGRO_WINDOWED);
  display = al_create_display(sw, sh);
  // fullscreen
  // TODO: fix fullscreen renderization
  // al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
  // al_set_new_display_flags(ALLEGRO_FULLSCREEN);
  // display = al_create_display(disp_data.width, disp_data.height);
  al_set_window_title(display, "Top Gear");
}

void setup_events(){
  queue = al_create_event_queue();
  timer = al_create_timer(1.0 / fps);
  al_register_event_source(queue, al_get_display_event_source(display));
  al_register_event_source(queue, al_get_timer_event_source(timer));
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_reserve_samples(2);
}
