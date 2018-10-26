#include "environment.h"

int sw = 1300;
int sh = 700;

ALLEGRO_DISPLAY_MODE disp_data;
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE* queue = NULL;
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_AUDIO_STREAM *music = NULL;
