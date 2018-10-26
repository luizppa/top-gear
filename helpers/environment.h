#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

struct ENV {
  ALLEGRO_DISPLAY_MODE disp_data;
  ALLEGRO_DISPLAY* display;
  ALLEGRO_EVENT_QUEUE* queue;
  ALLEGRO_TIMER* timer;
  ALLEGRO_AUDIO_STREAM** musics;
  ALLEGRO_FONT** fonts;
  int musics_count;
  int fonts_count;
};

typedef struct ENV ENVIRONMENT;

extern int sw;
extern int sh;

extern ALLEGRO_DISPLAY_MODE disp_data;
extern ALLEGRO_DISPLAY* display;
extern ALLEGRO_EVENT_QUEUE* queue;
extern ALLEGRO_TIMER* timer;
extern ALLEGRO_AUDIO_STREAM* music;
