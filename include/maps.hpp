#ifndef MAPS_H
#define MAPS_H

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

namespace top_gear{

    enum Map {
        NONE = -1,
        LAS_VEGAS,
        BORDEAUX,
        HIROSHIMA,
        FRANKFURT
    };

    namespace maps {

        std::string get_map_song_title(Map map);
        ALLEGRO_AUDIO_STREAM* get_map_music(Map map);
        ALLEGRO_BITMAP* get_map_landscape(Map map);
        ALLEGRO_COLOR get_map_color(Map map);

    }

}

#endif
