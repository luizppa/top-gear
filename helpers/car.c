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

float speed_increase(int gear, float speed){
  switch (gear) {
    case 1:
      return 4.0/60;
    case 2:
      if(speed > 8/10*max_speed(gear-1)) return 4.0/60;
      else return 4/10*speed_increase(gear-1, speed);
    case 3:
      if(speed > 8/10*max_speed(gear-1)) return 7.0/60;
      else return 8/10*speed_increase(gear-1, speed);
    case 4:
      if(speed > 8/10*max_speed(gear-1)) return 9.0/60;
      else return 6/10*speed_increase(gear-1, speed);
    case 5:
      if(speed > 8/10*max_speed(gear-1)) return 7.0/60;
      else return 4/10*speed_increase(gear-1, speed);
    case 6:
      if(speed > 8/10*max_speed(gear-1)) return 5.0/60;
      else return 4/10*speed_increase(gear-1, speed);
  }
}

float max_speed(int gear){
  switch (gear) {
    case 1:
      return 10.0;
    case 2:
      return 33.0;
    case 3:
      return 60.0;
    case 4:
      return 100.0;
    case 5:
      return 120.0;
    case 6:
      return 160.0;
  }
}
