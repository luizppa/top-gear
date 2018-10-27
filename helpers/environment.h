#include "sounds.h"
#include "display.h"

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

extern int sw;
extern int sh;
extern float fps;

extern ALLEGRO_DISPLAY_MODE disp_data;
extern ALLEGRO_DISPLAY* display;
extern ALLEGRO_EVENT_QUEUE* queue;
extern ALLEGRO_TIMER* timer;
extern ALLEGRO_AUDIO_STREAM* music;
extern ALLEGRO_KEYBOARD_STATE* key_state;

void init_environment();
void setup_display();
void setup_events();
