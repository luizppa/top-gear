#include "car.h"

CAR new_car(ALLEGRO_BITMAP *texture){
  CAR car;
  car.width = 166.0;
  car.height = 120.0;
  car.speed = 0.0;
  car.fuel = 100.0;
  car.gear = 1;
  car.max_gear = 6;
  car.texture = texture;
  return car;
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
      if(speed > 0.8*max_speed(gear-1)) return 6.0/60.;
      else return 0.4*speed_increase(gear-1, speed);
    case 3:
      if(speed > 0.8*max_speed(gear-1)) return 8.0/60.;
      else return 0.8*speed_increase(gear-1, speed);
    case 4:
      if(speed > 0.8*max_speed(gear-1)) return 10.0/60.;
      else return 0.6*speed_increase(gear-1, speed);
    case 5:
      if(speed > 0.8*max_speed(gear-1)) return 8.4/60.;
      else return 0.4*speed_increase(gear-1, speed);
    case 6:
      if(speed > 0.8*max_speed(gear-1)) return 6.0/60.;
      else return 0.4*speed_increase(gear-1, speed);
  }
}
