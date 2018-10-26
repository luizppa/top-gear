#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>

#define SAMPLES_PER_BUFFER  1024

// Predefined songs
// Special thanks to https://audio.online-convert.com/convert-to-ogg for making this files possible
#define TITLE_MUSIC "assets/music/01_title.ogg"
#define LAS_VEGAS_MUSIC "assets/music/02_las_vegas.ogg"

void start_music(ALLEGRO_AUDIO_STREAM* music);
void stop_music(ALLEGRO_AUDIO_STREAM* music);
ALLEGRO_AUDIO_STREAM* set_music(char* file);
void change_music(ALLEGRO_AUDIO_STREAM* music, char* file);
