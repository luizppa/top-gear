#include "../include/car.hpp"
#include "../include/utils.hpp"
#include "../include/environment.hpp"
#include "../include/display.hpp"
#include "../include/colors.hpp"
#include "../include/sounds.hpp"

#include<iostream>

namespace top_gear {

  // Return a player car
  Car::Car(ALLEGRO_BITMAP* texture){
    this->lvl = 0;
    this->name = "Player";
    this->points = 0;
    this->width = display::get_bitmap_width(texture);
    this->height = display::get_bitmap_height(texture);
    this->speed = 0.0;
    this->x = -425;
    this->y = 0.0;
    this->screen_x = SCREEN_WIDTH/2;
    this->screen_y = 0;
    this->nitrox = 100.0;
    this->gear = 1;
    this->max_gear = 6;
    this->will_colide = false;
    this->texture = texture;
  }

  // Return a AI car
  Car::Car(int lvl, ALLEGRO_BITMAP *texture){
    this->lvl = lvl;
    this->name = utils::names[lvl];
    this->points = 0;
    this->width = display::get_bitmap_width(texture);
    this->height = display::get_bitmap_height(texture);
    this->speed = 0.0;
    this->nitrox = 100.0;
    this->gear = 1;
    this->max_gear = 6;
    this->texture = texture;
    if(lvl % 3 == 0) this->will_colide = false;
    else this->will_colide = true;
    this->going_right = false;
    if(lvl < 4){
      this->y = (3*STARTING_DISTANCE);
    }
    else if(lvl < 7){
      this->y = (2*STARTING_DISTANCE);
    }
    else if(lvl < 10){
      this->y = (1*STARTING_DISTANCE);
    }
    else if(lvl < 13){
      this->y = (0*STARTING_DISTANCE);
    }
    float x = ((lvl%3)-1);
    this->x = (x*25)+(x*400);
  }

  // Gear up
  void Car::gear_up(){
    if(this->gear < this->max_gear) this->gear++;
  }

  // Gear down
  void Car::gear_down(){
    if(this->gear > 1) this->gear--;
  }

  // Set to specifc gear
  void Car::set_gear(int gear){
    if(gear > 0 && gear <= this->max_gear) this->gear = gear;
  }

  void Car::control_ia(Car** cars, Object** objects, int car_count, int object_count, bool play_sounds){
    float skill_rate = (0.05/12.0)*this->lvl;
    float delta_speed = this->speed_increase();
    if(environment::collisions) {
      if(this->car_collided(cars, objects, car_count, object_count) && play_sounds){
        sounds::play_sample(sounds::COLLISION_SOUND);
      }
    }
    if(environment::ai_pilots){
      if (this->will_colide && environment::collisions) {
        
        if(!this->going_right){
          if(this->x > -440) this->x -= 20;
          else this->going_right = true;
        }
        
        if(this->going_right){
          if(this->x < 440) this->x += 20;
          else this->going_right = false;
        }
      }
      else {
        if(this->x >= 30) this->going_right = false;
        else if(this->x <= -30) this->going_right = true;
        else this->going_right = rand()%2;
      }
      
      if(delta_speed < 0){
        if(this->speed != (0.8 * Car::max_speed(this->max_gear-1))) this->gear_down();
        delta_speed = this->speed_increase();
        this->speed = utils::max(0, this->speed + delta_speed);
      }
      else {
        if(this->gear > 1 && this->speed < 0.8 * Car::max_speed(this->gear-1)){
          this->gear_down();
        }
        else if(this->speed >= this->ai_gear_up_point() && this->gear < this->max_gear) {
          this->gear_up();
        }
        this->speed += delta_speed*(1-skill_rate);
      }

    }
    else {
      this->speed = utils::max(0, this->speed - NO_ACCELERATE_EFFECT);
    }
    this->y += this->speed*DISTANCE_VARIATION;
  }

  int Car::get_gear_progress(){
    float progress = 10*this->speed/max_speed(this->gear);
    return (int)progress;
  }

  // Returns true if car collided with either an object or another car (also impacts speed)
  bool Car::car_collided(Car** cars, Object** objects, int car_count, int object_count){
    float relative_speed, distance;
    bool aligned;
    this->will_colide = false;

    // Collision with other cars
    for(int i = 0; i < car_count; i++){
      relative_speed = this->speed - cars[i]->speed;
      distance = cars[i]->y - this->y;
      aligned = this->is_aligned_to(cars[i]);
      // Don't verify collisions of a car with itslef
      if(cars[i] != this){
        if(aligned && relative_speed > 0){
          if (distance <= COLLISION_DISTANCE*1.8 && distance > COLLISION_DISTANCE*0.5){
            this->will_colide = true;
          }
          if (distance <= COLLISION_DISTANCE && distance > 0) {
            this->speed = this->speed-(relative_speed*0.8);
            cars[i]->speed += relative_speed*0.8;
            return true;
          }
        }
        else if(aligned && (distance <= COLLISION_CRITICAL_DISTACE && distance >= 0)){
          this->speed *= 0.4;
          return true;
        }
      }
    }

    // Collision with objects
    for(int i = 0; i < object_count; i++){
      if(objects[i]->is_collidable()){
        distance = objects[i]->get_y(); - this->y;
        aligned = this->is_aligned_to(objects[i]);
        if(aligned){
          if (distance <= COLLISION_DISTANCE*2.5 && distance >= 0){
            this->will_colide = true;
          }
          if (distance <= COLLISION_DISTANCE && distance > 0) {
            this->speed = -30.0;
            return true;
          }
        }
      }
    }

    return false;
  }

  // Returns true if the cars is horizontally aligned with the object
  bool Car::is_aligned_to(Object* object){
    float car_x0 = this->screen_x - (this->width/2);
    float car_xf = this->screen_x + (this->width/2);
    float object_x0 = object->get_screen_x() - (object->get_width()/2);
    float object_xf = object->get_screen_x() + (object->get_width()/2);
    if(environment::debug){
      al_draw_line(car_x0, 0, car_x0, SCREEN_HEIGHT, colors::RED, 1);
      al_draw_line(car_xf, 0, car_xf, SCREEN_HEIGHT, colors::YELLOW, 1);
      al_draw_line(object_x0, 0, object_x0, SCREEN_HEIGHT, colors::BLUE, 1);
      al_draw_line(object_xf, 0, object_xf, SCREEN_HEIGHT, colors::GREEN, 1);
    }
    if ((car_xf - car_x0 >= object_x0 - car_x0 && object_x0 - car_x0 >= 0) || (object_xf - object_x0 >= car_x0 - object_x0 && car_x0 - object_x0 >= 0)) {
      if(environment::debug) std::cout << "aligned" << std::endl;
      return true;
    }
    else {
      if(environment::debug) std::cout << "not aligned" << std::endl;
      return false;
    }
  }

  // Returns true if cars are horizontally aligned
  bool Car::is_aligned_to(Car* car){
    float a_x0 = this->screen_x - (this->width/2);
    float a_xf = this->screen_x + (this->width/2);
    float b_x0 = car->screen_x - (car->width/2);
    float b_xf = car->screen_x + (car->width/2);
    if(environment::debug){
      al_draw_line(a_x0, 0, a_x0, SCREEN_HEIGHT, colors::RED, 1);
      al_draw_line(a_xf, 0, a_xf, SCREEN_HEIGHT, colors::YELLOW, 1);
      al_draw_line(b_x0, 0, b_x0, SCREEN_HEIGHT, colors::BLUE, 1);
      al_draw_line(b_xf, 0, b_xf, SCREEN_HEIGHT, colors::GREEN, 1);
    }
    if ((a_xf - a_x0 >= b_x0 - a_x0 && b_x0 - a_x0 >= 0) || (b_xf - b_x0 >= a_x0 - b_x0 && a_x0 - b_x0 >= 0)) {
      if(environment::debug) std::cout << "aligned" << std::endl;
      return true;
    }
    else {
      if(environment::debug) std::cout << "not aligned" << std::endl;
      return false;
    }
  }

  // Returns the speed in which the AI controlled car sould raise its gear
  float Car::ai_gear_up_point(){
    float skill_rate = (0.1/12.0)*this->lvl;
    float precision;
    switch (this->gear) {
      case 1:
        precision = 0.65+skill_rate;
        break;
      case 2:
        precision = 0.66+skill_rate;
        break;
      case 3:
        precision = 0.67+skill_rate;
        break;
      case 4:
        precision = 0.685+skill_rate;
        break;
      case 5:
        precision = 0.69+skill_rate;
        break;
    }
    return Car::max_speed(gear)*precision;
  }

  /*
    Returns the variation rate of the car speed for each 1/fps seconds.
    If the current speed is higher than the current gear's max speed, returns the
    diference between them times a constant.
    If the gear is higher than 80% of the bellow gear's max speed, returns the best performance variation.
    Else, returns 30% to 40% of the bellow gear's max speed.
  */
  float Car::speed_increase(){
    if(this->speed > Car::max_speed(gear)) return (Car::max_speed(this->gear)-this->speed)*WRONG_GEAR_EFFECT;
    switch (gear) {
      case 1:
        return 16.0/environment::fps;
      case 2:
        if(this->speed > 0.8*Car::max_speed(this->gear-1)) return 6.4/environment::fps;
        else return 0.3*Car::speed_increase();
      case 3:
        if(this->speed > 0.8*Car::max_speed(this->gear-1)) return 7.0/environment::fps;
        else return 0.4*Car::speed_increase();
      case 4:
        if(this->speed > 0.8*Car::max_speed(this->gear-1)) return 7.7/environment::fps;
        else return 0.4*Car::speed_increase();
      case 5:
        if(this->speed > 0.8*Car::max_speed(this->gear-1)) return 8.0/environment::fps;
        else return 0.4*Car::speed_increase();
      case 6:
        if(this->speed > 0.8*Car::max_speed(this->gear-1)) return 8.6/environment::fps;
        else return 0.4*Car::speed_increase();
      default:
        return 0.0;
    }
  }

  // Place the cars at the begginig of the road based on their position on the last match
  void Car::restart_positions(Car** cars, int count){
    for (int i = 0; i < count; i++) {
      cars[i]->speed = 0.0;
      cars[i]->gear = 1;
      if(i+1 < 4){
        cars[i]->y = (3*STARTING_DISTANCE);
      }
      else if(i+1 < 7){
        cars[i]->y = (2*STARTING_DISTANCE);
      }
      else if(i+1 < 10){
        cars[i]->y = (1*STARTING_DISTANCE);
      }
      else if(i+1 < 13){
        cars[i]->y = (0*STARTING_DISTANCE);
      }
      float x = (((i+1)%3)-1);
      cars[i]->x = (x*25)+(x*400);
    }
  }

  // Return the max speed for each gear
  float Car::max_speed(int gear){
    switch (gear) {
      case 1:
        return 20.0;
      case 2:
        return 50.0;
      case 3:
        return 85.0;
      case 4:
        return 140.0;
      case 5:
        return 210.0;
      case 6:
        return 290.0;
      default:
        return 0.0;
    }
  }

  // quick_sort_cars sorting logic
  int Car::partition(Car** cars, int first, int last){
    Car *pivot = cars[first], *aux;
    int left_marker = first+1, right_marker = last;
    bool done = false;
    while (!done) {
      while (left_marker <= right_marker && cars[left_marker]->y <= pivot->y){
        left_marker++;
      }
      while(cars[right_marker]->y >= pivot->y && right_marker >= left_marker){
        right_marker--;
      }
      if (right_marker < left_marker) {
        done = true;
      }
      else{
        aux = cars[left_marker];
        cars[left_marker] = cars[right_marker];
        cars[right_marker] = aux;
      }
    }
    aux = cars[first];
    cars[first] = cars[right_marker];
    cars[right_marker] = aux;
    return right_marker;
  }

  // Recursive call of quick_sort_cars
  Car** Car::quick_sort_helper(Car** cars, int first, int last){
    if(first < last){
      int split_point = Car::partition(cars, first, last);
      Car::quick_sort_helper(cars, first, split_point-1);
      Car::quick_sort_helper(cars, split_point+1, last);
    }
    return cars;
  }

  // Sort the cars array based on their Y position
  Car** Car::quick_sort_cars(Car** cars, int size){
    Car::quick_sort_helper(cars, 0, size-1);
    if(environment::debug){
      std::cout << "----------------------" << std::endl;
      for(int i = 0; i < size; i++){
        std::cout << cars[i]->name << ": " << cars[i]->y << std::endl;
      }
    }
    return cars;
  }

}
