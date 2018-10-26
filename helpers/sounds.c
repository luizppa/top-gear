#include "sounds.h"

// Play music from the beginning
/*  TODO: modify start_music() to recieve a boolean
    that tells if the music should be played in loop  */
void start_music(ALLEGRO_AUDIO_STREAM* music, int loop){
  if(loop) al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);
  else al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_ONCE);
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
