#include "sounds.h"

// Play music from the beginning
void start_music(ALLEGRO_AUDIO_STREAM* music){
  al_attach_audio_stream_to_mixer(music, al_get_default_mixer());
  al_set_audio_stream_playing(music, true);
}

// Stop the music execution
void stop_music(ALLEGRO_AUDIO_STREAM* music){
  al_set_audio_stream_playing(music, false);
}

// Set the music to be played
ALLEGRO_AUDIO_STREAM* set_music(char* file){
  return al_load_audio_stream(file, 4, SAMPLES_PER_BUFFER);
}
