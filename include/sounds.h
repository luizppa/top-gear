#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>

#define SAMPLES_PER_BUFFER  1024

// Predefined songs and sounds
/*
  Special thanks to
  Juhani Junkala,
  https://downloads.khinsider.com/game-soundtracks/album/top-gear-snes,
  https://www.sounds-resource.com/snes/topgear3000/sound/773/ and
  https://audio.online-convert.com/convert-to-ogg for making this files possible
*/
#define TITLE_MUSIC "../assets/music/01_title.ogg"
#define LAS_VEGAS_MUSIC "../assets/music/02_las_vegas.ogg"
#define HIROSHIMA_MUSIC "../assets/music/03_hiroshima.ogg"
#define BORDEAUX_MUSIC "../assets/music/04_bordeaux.ogg"
#define FRANKFURT_MUSIC "../assets/music/05_frankfurt.ogg"
#define QUALIFIED_MUSIC "../assets/music/06_qualified.ogg"
#define ENDING_MUSIC "../assets/music/07_ending_theme.ogg"
#define DISQUALIFIED_MUSIC "../assets/music/08_disqualified.ogg"

extern ALLEGRO_SAMPLE *COLLISION_SOUND;
extern ALLEGRO_SAMPLE *READY_SOUND;
extern ALLEGRO_SAMPLE *GO_SOUND;
extern ALLEGRO_SAMPLE *CAR_ENGINE_SOUND;
extern ALLEGRO_SAMPLE *CAR_BOOST_SOUND;
extern ALLEGRO_SAMPLE *MENU_MOVE_SOUND;
extern ALLEGRO_SAMPLE *MENU_SELECT_SOUND;
extern ALLEGRO_SAMPLE *MENU_BACK_SOUND;

extern ALLEGRO_AUDIO_STREAM *music;

void init_sounds();
void start_music(ALLEGRO_AUDIO_STREAM* music, bool loop);
void stop_music(ALLEGRO_AUDIO_STREAM* music);
ALLEGRO_AUDIO_STREAM* set_music(char* file);
void restart_music(ALLEGRO_AUDIO_STREAM* music);
void set_music_volume(ALLEGRO_AUDIO_STREAM* music, float gain);
void turn_of_music(ALLEGRO_AUDIO_STREAM* music);
void turn_on_music(ALLEGRO_AUDIO_STREAM* music);
void play_sample(ALLEGRO_SAMPLE *sample);
ALLEGRO_SAMPLE_INSTANCE* continuously_play_sample(ALLEGRO_SAMPLE *sample);
void stop_sample(ALLEGRO_SAMPLE_INSTANCE* instance);
void set_sample_volume(ALLEGRO_SAMPLE_INSTANCE* instance, float volume);
void destroy_sounds();
