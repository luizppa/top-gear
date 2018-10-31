#include "car.h"
#include "utils.h"

#include <stdio.h>

CAR new_car(ALLEGRO_BITMAP *texture){
  CAR car;
  car.lvl = 0;
  car.width = STANDARD_CAR_WIDTH;
  car.height = STANDARD_CAR_HEIGHT;
  car.speed = 0.0;
  car.position_x = 0-(car.width/2.0);
  car.position_y = 0.0;
  car.fuel = 100.0;
  car.gear = 1;
  car.max_gear = 6;
  car.texture = texture;
  return car;
}

CAR new_oponent(int lvl, ALLEGRO_BITMAP *texture){
  CAR car;
  car.lvl = lvl;
  car.width = STANDARD_CAR_WIDTH;
  car.height = STANDARD_CAR_HEIGHT;
  car.speed = 0.0;
  car.fuel = 100.0;
  car.gear = 1;
  car.max_gear = 6;
  car.texture = texture;
  if(lvl < 4){
    car.position_y = (4*STARTING_DISTANCE)+car.height+20;
  }
  else if(lvl < 7){
    car.position_y = (3*STARTING_DISTANCE)+car.height+20;
  }
  else if(lvl < 10){
    car.position_y = (2*STARTING_DISTANCE)+car.height+20;
  }
  else{
    car.position_y = (STARTING_DISTANCE)+car.height+20;
  }
  float x = ((lvl%3)-1);
  car.position_x = (x*25)+(x*400);
  return car;
}

void gear_up(CAR *car){
  if(car->gear < car->max_gear) car->gear++;
}

void gear_down(CAR *car){
  if(car->gear > 1) car->gear--;
}

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

float speed_increase(int gear, float speed){
  if(speed > max_speed(gear)) return (max_speed(gear)-speed)*WRONG_GEAR_EFFECT;
  switch (gear) {
    case 1:
      return 16.0/60;
    case 2:
      if(speed > 0.8*max_speed(gear-1)) return 6.0/60.0;
      else return 0.4*speed_increase(gear-1, speed);
    case 3:
      if(speed > 0.8*max_speed(gear-1)) return 8.0/60.0;
      else return 0.8*speed_increase(gear-1, speed);
    case 4:
      if(speed > 0.8*max_speed(gear-1)) return 10.0/60.0;
      else return 0.6*speed_increase(gear-1, speed);
    case 5:
      if(speed > 0.8*max_speed(gear-1)) return 8.4/60.0;
      else return 0.4*speed_increase(gear-1, speed);
    case 6:
      if(speed > 0.8*max_speed(gear-1)) return 6.0/60.0;
      else return 0.4*speed_increase(gear-1, speed);
  }
}

CAR* quick_sort_cars(CAR* cars, int size){
  int wall = 0, pivot = size-1;
  CAR aux;
  while(pivot > wall){
    for (int i = wall; i < pivot; i++) {
      if(cars[i].position_y < cars[pivot].position_y){
        aux = cars[wall];
        cars[wall] = cars[i];
        cars[i] = aux;
        wall++;
      }
    }
    aux = cars[wall];
    cars[wall] = cars[pivot];
    cars[pivot] = aux;
    wall++;
  }
  return cars;
}

void control_ia(CAR* car){
  // Going left
  // if (al_key_down(&key_state, ALLEGRO_KEY_A)) {
  //   if(position < max(sw, street_width)+player.width) position += min(moviment_speed*((player.speed)/40), moviment_speed);
  // }
  // Going right
  // if (al_key_down(&key_state, ALLEGRO_KEY_D)){
  //   if(position > 0-player.width) position -= min(moviment_speed*((player.speed)/40), moviment_speed);
  // }
  // Accelerating
  float delta_speed = speed_increase(car->gear, car->speed);
  if(delta_speed < 0){
    gear_down(car);
    car->speed = max(0, car->speed + delta_speed);
  }
  else if(car->speed+delta_speed < 0.86*max_speed(car->gear)) {
    car->speed += delta_speed;
  }
  else {
    gear_up(car);
    car->speed += delta_speed/6;
  }
  car->position_y += car->speed * DISTANCE_VARIATION * 0.86;
}
