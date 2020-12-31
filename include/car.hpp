#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#include "object.h"

#ifndef CAR_H 
#define CAR_H

// Environmental values
#define GRASS_SLOW_EFFECT 0.13333333333
#define NO_ACCELERATE_EFFECT 0.05433333333
#define BREAK_EFFECT 0.73
#define WRONG_GEAR_EFFECT 0.00166666666
#define DISTANCE_VARIATION 0.147778
#define STARTING_DISTANCE 130
#define COLLISION_DISTANCE 50
#define COLLISION_CRITICAL_DISTACE 30

#define STANDARD_Car_WIDTH 166.0
#define STANDARD_Car_HEIGHT 120.0

// Car code
#define OCTANE 0
#define DOMINUS 1

class Car {
  private:
    int lvl;
    char *name;
    int points;
    float width;
    float apparent_width;
    float height;
    float apparent_height;
    float speed;
    float position_x; // The Car center x coordinate (on the road, where 0 is the center)
    float screen_position_x; // The Car center x coordinate (on the screen, where 0 is the left border of the window)
    float position_y; // The Car bottom y coordinate (on the road, a virtual environment)
    float screen_position_y; // The Car center y coordinate (on the screen, where 0 is the top of the window)
    float nitrox;
    int gear;
    int max_gear;
    bool will_colide;
    bool going_right;
    ALLEGRO_BITMAP* texture;
    
    bool is_aligned_to(OBJECT* object);
    bool is_aligned_to(Car* car);
    float ai_gear_up_point();
    static int Car::partition(Car** cars, int first, int last);
    static Car** quick_sort_helper(Car** cars, int first, int last);

  public:
    Car(ALLEGRO_BITMAP* texture);
    Car(int lvl, ALLEGRO_BITMAP *texture);
    void gear_up();
    void gear_down();
    void set_gear(int gear);
    void control_ia(Car** cars, OBJECT** objects, int car_count, int object_count, bool play_sounds);
    int get_gear_progress();
    bool car_collided(Car** cars, OBJECT** objects, int car_count, int object_count);
    static void restart_positions(Car** Cars, int count);
    static float max_speed(int gear);
    static float speed_increase(int gear, float speed);
    static Car** quick_sort_cars(Car** cars, int size);
};

#endif