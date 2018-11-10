#include "maps.h"

/*
  maps:
    0 = Las Vegas
    1 = Bordeaux
    2 = Hiroshima
    3 = Frankfurt
*/

// Returns the map music asset
ALLEGRO_AUDIO_STREAM* get_map_music(int map){
  switch (map) {
    case 0: return set_music(LAS_VEGAS_MUSIC);
    case 1: return set_music(BORDEAUX_MUSIC);
    case 2: return set_music(HIROSHIMA_MUSIC);
    case 3: return set_music(FRANKFURT_MUSIC);
  }
}

// Returns the map landscape asset
ALLEGRO_BITMAP* get_map_landscape(int map){
  switch (map) {
    case 0: return LAS_VEGAS_LANDSCAPE_BITMAP;
    case 1: return BORDEAUX_LANDSCAPE_BITMAP;
    case 2: return HIROSHIMA_LANDSCAPE_BITMAP;
    case 3: return FRANKFURT_LANDSCAPE_BITMAP;
  }
}

// Returns the map soil color
ALLEGRO_COLOR get_map_color(int map){
  switch (map) {
    case 0: return LAS_VEGAS_SOIL;
    case 1: return GREEN;
    case 2: return GREEN;
    case 3: return FRANKFURT_SOIL;
  }
}
