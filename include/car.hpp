#ifndef CAR_H 
#define CAR_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <vector>

#include "./obstacle.hpp"

// Environmental values
#define GRASS_SLOW_EFFECT 0.13333333333
#define NO_ACCELERATE_EFFECT 0.05433333333
#define BREAK_EFFECT 0.73
#define WRONG_GEAR_EFFECT 0.00166666666
#define DISTANCE_VARIATION 0.147778
#define STARTING_DISTANCE 130
#define COLLISION_DISTANCE 50
#define COLLISION_CRITICAL_DISTACE 30

#define STANDARD_CAR_WIDTH 166.0
#define STANDARD_CAR_HEIGHT 120.0

namespace top_gear {

  enum CarsTypes {
    DOMINUS = 1,
    OCTANE = 2,
    ROADHOG = 3,
    MAVERICK = 4
  };

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
      float x; // The Car center x coordinate (on the road, where 0 is the center)
      float screen_x; // The Car center x coordinate (on the screen, where 0 is the left border of the window)
      float y; // The Car bottom y coordinate (on the road, a virtual environment)
      float screen_y; // The Car center y coordinate (on the screen, where 0 is the top of the window)
      float nitrox;
      int gear;
      int max_gear;
      bool will_colide;
      bool going_right;
      bool engine_running = false;
      bool boosting = false;
      ALLEGRO_BITMAP* texture;
      ALLEGRO_SAMPLE_INSTANCE* engine_sound_instance = nullptr;
      
      bool is_aligned_to(Obstacle* object);
      bool is_aligned_to(Car* car);
      float ai_gear_up_point();
      static bool compare(Car* a, Car* b);

    public:
      Car(ALLEGRO_BITMAP* texture);
      Car(int lvl, ALLEGRO_BITMAP *texture);

      int get_lvl() { return this->lvl; }
      char* get_name() { return this->name; }
      int get_points() { return this->points; }
      float get_width() { return this->width; }
      float get_apparent_width() { return this->apparent_width; }
      float get_height() { return this->height; }
      float get_apparent_height() { return this->apparent_height; }
      float get_speed() { return this->speed; }
      float get_x() { return this->x; }
      float get_screen_x() { return this->screen_x; }
      float get_y() { return this->y; }
      float get_screen_y() { return this->screen_y; }
      float get_nitrox() { return this->nitrox; }
      float speed_increase();
      int get_gear() { return this->gear; }
      int get_max_gear() { return this->max_gear; }
      bool is_will_colide() { return this->will_colide; }
      bool is_going_right() { return this->going_right; }
      bool is_engine_running() { return this->engine_running; }
      bool is_boosting() { return this->boosting; }
      ALLEGRO_BITMAP* get_texture() { return this->texture; }
      ALLEGRO_SAMPLE_INSTANCE* get_engine_sound_instance() { return this->engine_sound_instance; };

      
      void set_apparent_width(float apparent_width) { this->apparent_width = apparent_width; }
      void set_apparent_height(float apparent_height) { this->apparent_height = apparent_height; }
      void set_speed(float speed) { this->speed = speed; }
      void set_x(float x) { this->x = x; }
      void set_screen_x(float screen_x) { this->screen_x = screen_x; }
      void set_y(float y) { this->y = y; }
      void set_screen_y(float screen_y) { this->screen_y = screen_y; }
      void set_nitrox(float nitrox) { this->nitrox = nitrox; }
      void set_engine_running(bool running) { this->engine_running = running; }
      void set_boosting(bool boosting) { this->boosting = boosting; }

      void gear_up();
      void gear_down();
      void set_gear(int gear);
      void control_ia(std::vector<Car*>& cars, std::vector<Obstacle*>& objects, bool play_sounds);
      void play_engine_sound();
      void stop_engine_sound();
      int get_gear_progress();
      bool car_collided(std::vector<Car*>& cars, std::vector<Obstacle*>& objects);
      static void restart_positions(std::vector<Car*>& cars);
      static float max_speed(int gear);
      static void sort(std::vector<Car*>& cars);
  };

}

#endif