#include "car.h"
#include "utils.h"
#include "environment.h"

// Return a player car
CAR new_car(ALLEGRO_BITMAP* texture){
  CAR car;
  car.lvl = 0;
  car.name = "Player";
  car.points = 0;
  car.width = get_bitmap_width(texture);
  car.height = get_bitmap_height(texture);
  car.speed = 0.0;
  car.position_x = -425;
  car.position_y = 0.0;
  car.screen_position_x = sw/2;
  car.screen_position_y = 0;
  car.fuel = 100.0;
  car.gear = 1;
  car.max_gear = 6;
  car.will_colide = false;
  car.texture = texture;
  return car;
}

// Return a AI car
CAR new_oponent(int lvl, ALLEGRO_BITMAP *texture){
  CAR car;
  car.lvl = lvl;
  car.name = names[lvl];
  car.points = 0;
  car.width = get_bitmap_width(texture);
  car.height = get_bitmap_height(texture);
  car.speed = 0.0;
  car.fuel = 100.0;
  car.gear = 1;
  car.max_gear = 6;
  car.texture = texture;
  if(lvl%3 == 0) car.will_colide = false;
  else car.will_colide = true;
  car.going_right = false;
  if(lvl < 4){
    car.position_y = (3*STARTING_DISTANCE);
  }
  else if(lvl < 7){
    car.position_y = (2*STARTING_DISTANCE);
  }
  else if(lvl < 10){
    car.position_y = (1*STARTING_DISTANCE);
  }
  else if(lvl < 13){
    car.position_y = (0*STARTING_DISTANCE);
  }
  float x = ((lvl%3)-1);
  car.position_x = (x*25)+(x*400);
  return car;
}

// Replace the cars at the begginig of the road based on their position on the last match
void restart_positions(CAR* cars, int count){
  for (int i = 0; i < count; i++) {
    cars[i].speed = 0.0;
    cars[i].gear = 1;
    if(i+1 < 4){
      cars[i].position_y = (3*STARTING_DISTANCE);
    }
    else if(i+1 < 7){
      cars[i].position_y = (2*STARTING_DISTANCE);
    }
    else if(i+1 < 10){
      cars[i].position_y = (1*STARTING_DISTANCE);
    }
    else if(i+1 < 13){
      cars[i].position_y = (0*STARTING_DISTANCE);
    }
    float x = (((i+1)%3)-1);
    cars[i].position_x = (x*25)+(x*400);
  }
}

// Gear up
void gear_up(CAR *car){
  if(car->gear < car->max_gear) car->gear++;
}

// Gear down
void gear_down(CAR *car){
  if(car->gear > 1) car->gear--;
}

// Set to specifc gear
void set_gear(CAR *car, int gear){
  if(gear > 0 && gear <= car->max_gear) car->gear = gear;
}

// Return the max speed for each gear
float max_speed(int gear){
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
  }
}

/*
  Returns the variation rate of the car speed for each 1/fps seconds.
  If the current speed is higher than the current gear's max speed, returns the
  diference between them times a constant.
  If the gear is higher than 80% of the bellow gear's max speed, returns the best performance variation.
  Else, returns 30% to 40% of the bellow gear's max speed.
*/
float speed_increase(int gear, float speed){
  if(speed > max_speed(gear)) return (max_speed(gear)-speed)*WRONG_GEAR_EFFECT;
  switch (gear) {
    case 1:
      return 16.0/fps;
    case 2:
      if(speed > 0.8*max_speed(gear-1)) return 6.4/fps;
      else return 0.3*speed_increase(gear-1, speed);
    case 3:
      if(speed > 0.8*max_speed(gear-1)) return 7.0/fps;
      else return 0.4*speed_increase(gear-1, speed);
    case 4:
      if(speed > 0.8*max_speed(gear-1)) return 7.7/fps;
      else return 0.4*speed_increase(gear-1, speed);
    case 5:
      if(speed > 0.8*max_speed(gear-1)) return 8.0/fps;
      else return 0.4*speed_increase(gear-1, speed);
    case 6:
      if(speed > 0.8*max_speed(gear-1)) return 8.6/fps;
      else return 0.4*speed_increase(gear-1, speed);
  }
}

int get_gear_progress(CAR car){
  float progress = 10*car.speed/max_speed(car.gear);
  return (int)progress;
}

// quick_sort_cars sorting logic
int partition(CAR** cars, int first, int last){
  CAR *pivot = cars[first], *aux;
  int left_marker = first+1, right_marker = last;
  bool done = false;
  while (!done) {
    while (left_marker <= right_marker && cars[left_marker]->position_y <= pivot->position_y){
      left_marker++;
    }
    while(cars[right_marker]->position_y >= pivot->position_y && right_marker >= left_marker){
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
CAR** quick_sort_helper(CAR** cars, int first, int last){
  if(first < last){
    int split_point = partition(cars, first, last);
    quick_sort_helper(cars, first, split_point-1);
    quick_sort_helper(cars, split_point+1, last);
  }
  else return cars;
}

// Sort the cars array based on their Y position
CAR** quick_sort_cars(CAR** cars, int size){
  quick_sort_helper(cars, 0, size-1);
  if(debug){
    printf("----------------------\n");
    for(int i = 0; i < size; i++){
      printf("%s: %f\n", cars[i]->name, cars[i]->position_y);
    }
  }
  return cars;
}

// Returns true if cars are horizontally aligned
bool are_cars_aligned(CAR* a, CAR* b){
  float a_x0 = a->screen_position_x - (a->width/2);
  float a_xf = a->screen_position_x + (a->width/2);
  float b_x0 = b->screen_position_x - (b->width/2);
  float b_xf = b->screen_position_x + (b->width/2);
  if(debug){
    al_draw_line(a_x0, 0, a_x0, sh, RED, 1);
    al_draw_line(a_xf, 0, a_xf, sh, YELLOW, 1);
    al_draw_line(b_x0, 0, b_x0, sh, BLUE, 1);
    al_draw_line(b_xf, 0, b_xf, sh, GREEN, 1);
  }
  if ((a_xf - a_x0 >= b_x0 - a_x0 && b_x0 - a_x0 >= 0) || (b_xf - b_x0 >= a_x0 - b_x0 && a_x0 - b_x0 >= 0)) {
    if(debug) printf("aligned\n");
    return true;
  }
  else {
    if(debug) printf("not aligned\n");
    return false;
  }
}

// Returns true if the cars is horizontally aligned with the object
bool is_cars_aligned_to_object(CAR* car, OBJECT object){
  float a_x0 = car->screen_position_x - (car->width/2);
  float a_xf = car->screen_position_x + (car->width/2);
  float b_x0 = object.screen_position_x - (object.width/2);
  float b_xf = object.screen_position_x + (object.width/2);
  if(debug){
    al_draw_line(a_x0, 0, a_x0, sh, RED, 1);
    al_draw_line(a_xf, 0, a_xf, sh, YELLOW, 1);
    al_draw_line(b_x0, 0, b_x0, sh, BLUE, 1);
    al_draw_line(b_xf, 0, b_xf, sh, GREEN, 1);
  }
  if ((a_xf - a_x0 >= b_x0 - a_x0 && b_x0 - a_x0 >= 0) || (b_xf - b_x0 >= a_x0 - b_x0 && a_x0 - b_x0 >= 0)) {
    if(debug) printf("aligned\n");
    return true;
  }
  else {
    if(debug) printf("not aligned\n");
    return false;
  }
}

// Returns true if car collided with eithar an object or another car (also impacts speed)
bool car_collided(CAR* car, CAR** cars, OBJECT* objects, int car_count, int object_count, bool play_sounds){
  float relative_speed, distance;
  bool aligned;
  car->will_colide = false;

  // Collision with other cars
  for(int i = 0; i < car_count; i++){
    relative_speed = car->speed - cars[i]->speed;
    distance = cars[i]->position_y - car->position_y;
    aligned = are_cars_aligned(car, cars[i]);
    // Don't verify collisions of a car with itslef
    if(cars[i] != car){
      // If the cars are aligned and "car" is getting closer to "cars[i]"
      if(aligned && relative_speed > 0){
        // If the car is about to colide, set a warn to steer
        if (distance <= COLLISION_DISTANCE*1.8 && distance > COLLISION_DISTANCE*0.5){
          car->will_colide = true;
        }
        // If the car is less than COLLISION_DISTANCE meters away from cars[i]
        if (distance <= COLLISION_DISTANCE && distance > 0) {
          car->speed = max(0, car->speed-(relative_speed*0.8));
          cars[i]->speed += relative_speed*0.8;
          if(play_sounds) play_sample(COLLISION_SOUND);
          return true;
        }
      }
      // If "car" is not getting closer to "cars[i]" but has already touched it
      else if(aligned && (distance <= COLLISION_CRITICAL_DISTACE && distance >= 0)){
        car->speed *= 0.4;
        return true;
      }
    }
  }

  // Collision with objects
  for(int i = 0; i < object_count; i++){
    if(objects[i].collidable){
      distance = objects[i].position_y - car->position_y;
      aligned = is_cars_aligned_to_object(car, objects[i]);
      // If the car is aligned to the object
      if(aligned){
        // If the car is about to colide, set a warn to steer
        if (distance <= COLLISION_DISTANCE*2.5 && distance >= 0){
          car->will_colide = true;
        }
        // If the car is less than COLLISION_DISTANCE meters away from the object
        if (distance <= COLLISION_DISTANCE && distance > 0) {
          car->speed = -15.0;
          if(play_sounds) play_sample(COLLISION_SOUND);
          return true;
        }
      }
    }
  }

  return false;
}

// Returns the speed in which the AI controlled car sould raise its gear
float ai_gear_up_point(int gear, int lvl){
  // The closer the miss_rate gets to 0.8, the more precise the gear transition is
  float skill_rate = (0.1/12.0)*lvl;
  float miss_rate;
  switch (gear) {
    case 1:
      miss_rate = 0.65+skill_rate;
      break;
    case 2:
      miss_rate = 0.66+skill_rate;
      break;
    case 3:
      miss_rate = 0.67+skill_rate;
      break;
    case 4:
      miss_rate = 0.685+skill_rate;
      break;
    case 5:
      miss_rate = 0.69+skill_rate;
      break;
  }
  return max_speed(gear)*miss_rate;
}

void control_ia(CAR* car, CAR** cars, OBJECT* objects, int car_count, int object_count, bool play_sounds){
  float skill_rate = (0.05/12.0)*car->lvl;
  float delta_speed = speed_increase(car->gear, car->speed);
  if(collisions) {
    if(car_collided(car, cars, objects, car_count, object_count, play_sounds) && play_sounds){
      play_sample(COLLISION_SOUND);
    }
  }
  if(ai_pilots){
    // Verify collision
    if (car->will_colide && collisions) {
      // Go left
      if(!car->going_right){
        if(car->position_x > -440) car->position_x -= 20;
        else car->going_right = true;
      }
      // Go right
      if(car->going_right){
        if(car->position_x < 440) car->position_x += 20;
        else car->going_right = false;
      }
    }
    else {
      if(car->position_x >= 30) car->going_right = false;
      else if(car->position_x <= -30) car->going_right = true;
      else car->going_right = rand()%2;
    }
    // Accelerating
    if(delta_speed < 0){
      if(car->speed != 0.8*max_speed(car->max_gear-1)) gear_down(car);
      delta_speed = speed_increase(car->gear, car->speed);
      car->speed = max(0, car->speed + delta_speed);
    }
    else {
      if(car->gear > 1 && car->speed < 0.8*max_speed(car->gear-1)){
        gear_down(car);
      }
      else if(car->speed >= ai_gear_up_point(car->gear, car->lvl) && car->gear < car->max_gear) {
        gear_up(car);
      }
      car->speed += delta_speed*(1-skill_rate);
    }
  }
  else {
    car->speed = max(0, car->speed - NO_ACCELERATE_EFFECT);
  }
  car->position_y += car->speed*DISTANCE_VARIATION;
}
