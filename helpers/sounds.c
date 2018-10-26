#include "sounds.h"

void start_music(ALLEGRO_AUDIO_STREAM* music){
  al_set_audio_stream_playing(music, true);
}

void stop_music(ALLEGRO_AUDIO_STREAM* music){
  al_set_audio_stream_playing(music, false);
}

ALLEGRO_AUDIO_STREAM* set_music(char* file){
  return al_load_audio_stream(file, 4, SAMPLES_PER_BUFFER);
}

void change_music(ALLEGRO_AUDIO_STREAM* music, char* file){
  al_set_audio_stream_playing(music, false);
  music = al_load_audio_stream("assets/music/01_title.ogg", 4, SAMPLES_PER_BUFFER);
  al_set_audio_stream_playing(music, true);
}
