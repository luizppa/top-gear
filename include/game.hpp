#ifndef GAME_H
#define GAME_H

#include "./environment.hpp"
#include "./car.hpp"
#include "./maps.hpp"
#include "./utils.hpp"
#include "./menus.hpp"
#include "./maps.hpp"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

enum Direction {LEFT, RIGHT};
typedef enum Direction Direction;

namespace top_gear {

    extern int oponent_count;
    extern int placement;
    extern double race_time;
    extern Car** leaderboard;

    int play(ALLEGRO_BITMAP* player_texture, Car** tournament_cars, int oponents_amount, Map map, bool single_match);
    int tournament(ALLEGRO_BITMAP* player_texture, int oponents_amount);

}

#endif
