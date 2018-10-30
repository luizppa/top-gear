#include "car.h"

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
  car.max_gear = 5;
  car.texture = texture;
  if(lvl < 5){
    car.position_x = -((1*25)+(1*400));
  }
  else if(lvl < 9){
    car.position_x = 0;
  }
  else{
    car.position_x = ((1*25)+(1*400));
  }
  car.position_y = (((lvl%4)+1)*100)+car.height;
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
  for(int i = 0; i < size; i++){
    printf("%.2f\n", cars[i].position_y);
  }
  return cars;
}
