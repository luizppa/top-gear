#include "../include/maps.hpp"
#include "../include/sounds.hpp"
#include "../include/display.hpp"
#include "../include/colors.hpp"

#include <allegro5/allegro5.h>
#include <allegro5/events.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

namespace top_gear {

  namespace maps {

    // Returns the map music asset
    ALLEGRO_AUDIO_STREAM* get_map_music(Map map){
      switch (map) {
        case Map::LAS_VEGAS: return sounds::set_music(LAS_VEGAS_MUSIC);
        case Map::BORDEAUX: return sounds::set_music(BORDEAUX_MUSIC);
        case Map::HIROSHIMA: return sounds::set_music(HIROSHIMA_MUSIC);
        case Map::FRANKFURT: return sounds::set_music(FRANKFURT_MUSIC);
      }
    }

    // Returns the file name to the map music asset
    char* get_map_music_title(Map map){
      switch (map) {
        case Map::LAS_VEGAS: return LAS_VEGAS_MUSIC;
        case Map::BORDEAUX: return BORDEAUX_MUSIC;
        case Map::HIROSHIMA: return HIROSHIMA_MUSIC;
        case Map::FRANKFURT: return FRANKFURT_MUSIC;
      }
    }

    // Returns the map landscape asset
    ALLEGRO_BITMAP* get_map_landscape(Map map){
      switch (map) {
        case Map::LAS_VEGAS: return display::LAS_VEGAS_LANDSCAPE_BITMAP;
        case Map::BORDEAUX: return display::BORDEAUX_LANDSCAPE_BITMAP;
        case Map::HIROSHIMA: return display::HIROSHIMA_LANDSCAPE_BITMAP;
        case Map::FRANKFURT: return display::FRANKFURT_LANDSCAPE_BITMAP;
      }
    }

    // Returns the map soil color
    ALLEGRO_COLOR get_map_color(Map map){
      switch (map) {
        case Map::LAS_VEGAS: return colors::LAS_VEGAS_SOIL;
        case Map::BORDEAUX: return colors::GREEN;
        case Map::HIROSHIMA: return colors::HIROSHIMA_SOIL;
        case Map::FRANKFURT: return colors::FRANKFURT_SOIL;
      }
    }

  }

}
