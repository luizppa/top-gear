#include "environment.h"
#include "car.h"
#include "utils.h"
#include "object.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

enum DIR {LEFT, RIGHT};
typedef enum DIR DIRECTION;

extern int oponent_count;
extern int placement;
extern double race_time;
extern CAR** leaderboard;

int play(ALLEGRO_BITMAP* player_texture, CAR** tournament_cars, int oponents_amount);
