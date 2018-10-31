#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#define GRASS_SLOW_EFFECT 0.13333333333
#define NO_ACCELERATE_EFFECT 0.05433333333
#define BREAK_EFFECT 0.73
#define WRONG_GEAR_EFFECT 0.00166666666
#define DISTANCE_VARIATION 0.177778
#define STARTING_DISTANCE 100

#define STANDARD_CAR_WIDTH 166.0
#define STANDARD_CAR_HEIGHT 120.0

struct CAR_TYPE {
  int lvl;
  float width;
  float height;
  float speed;
  float position_x;
  float  position_y;
  float fuel;
  int gear;
  int max_gear;
  ALLEGRO_BITMAP* texture;
};
typedef struct CAR_TYPE CAR;

CAR new_car(ALLEGRO_BITMAP *texture);
CAR new_oponent(int lvl, ALLEGRO_BITMAP *texture);
void gear_up(CAR *car);
void gear_down(CAR *car);
float max_speed(int gear);
float speed_increase(int gear, float speed);
CAR* quick_sort_cars(CAR* cars, int size);
void control_ia(CAR* car);
