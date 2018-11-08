#include "sounds.h"

ALLEGRO_SAMPLE *COLLISION_SOUND = NULL;
ALLEGRO_SAMPLE *READY_SOUND = NULL;
ALLEGRO_SAMPLE *GO_SOUND = NULL;
ALLEGRO_SAMPLE *CAR_ENGINE_SOUND = NULL;
ALLEGRO_SAMPLE *MENU_MOVE_SOUND = NULL;
ALLEGRO_SAMPLE *MENU_SELECT_SOUND = NULL;
ALLEGRO_SAMPLE *MENU_BACK_SOUND = NULL;

ALLEGRO_AUDIO_STREAM *music = NULL;

// Initialize sound effects
void init_sounds(){
  COLLISION_SOUND = al_load_sample("assets/sounds/collision.ogg");
  READY_SOUND = al_load_sample("assets/sounds/ready.ogg");
  GO_SOUND = al_load_sample("assets/sounds/go.ogg");
  CAR_ENGINE_SOUND = al_load_sample("assets/sounds/car_engine.ogg");
  MENU_MOVE_SOUND = al_load_sample("assets/sounds/menu_move.ogg");
  MENU_SELECT_SOUND = al_load_sample("assets/sounds/menu_select.ogg");
  MENU_BACK_SOUND = al_load_sample("assets/sounds/menu_back.ogg");
}

// Play music from the beginning
void start_music(ALLEGRO_AUDIO_STREAM* music, bool loop){
  if(loop) al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_LOOP);
  else al_set_audio_stream_playmode(music, ALLEGRO_PLAYMODE_ONCE);
  al_attach_audio_stream_to_mixer(music, al_get_default_mixer());
  al_set_audio_stream_playing(music, true);
}

// Stop the music execution
void stop_music(ALLEGRO_AUDIO_STREAM* music){
  al_set_audio_stream_playing(music, false);
  al_destroy_audio_stream(music);
}

// Set the music to be played
ALLEGRO_AUDIO_STREAM* set_music(char* file){
  return al_load_audio_stream(file, 4, SAMPLES_PER_BUFFER);
}

// Play a sample once
void play_sample(ALLEGRO_SAMPLE *sample){
  al_play_sample(sample, 0.8, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

// Play a sample continuously and returns the sample id
ALLEGRO_SAMPLE_INSTANCE* continuously_play_sample(ALLEGRO_SAMPLE *sample){
  ALLEGRO_SAMPLE_INSTANCE* instance = al_create_sample_instance(sample);
	al_attach_sample_instance_to_mixer(instance, al_get_default_mixer());
  al_set_sample_instance_playmode(instance, ALLEGRO_PLAYMODE_LOOP);
	al_play_sample_instance(instance);
  return instance;
}

// Stop playing a sample
void stop_sample(ALLEGRO_SAMPLE_INSTANCE* instance){
  al_stop_sample_instance(instance);
  al_detach_sample_instance(instance);
}

// Set the gain value of a sample
void set_sample_volume(ALLEGRO_SAMPLE_INSTANCE* instance, float volume){
  al_set_sample_instance_gain(instance, volume);
}

// Free pointers
void destroy_sounds(){
  al_destroy_audio_stream(music);
  al_destroy_sample(COLLISION_SOUND);
  al_destroy_sample(CAR_ENGINE_SOUND);
  al_destroy_sample(READY_SOUND);
  al_destroy_sample(GO_SOUND);
  al_destroy_sample(MENU_MOVE_SOUND);
  al_destroy_sample(MENU_SELECT_SOUND);
  al_destroy_sample(MENU_BACK_SOUND);
}
