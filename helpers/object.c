#include "object.h"
#include "environment.h"

OBJECT new_object(float x, float y, float width, float height, ALLEGRO_BITMAP* texture, bool collidable){
  OBJECT object;
  object.position_x = x;
  object.position_y = y;
  object.width = width;
  object.height = height;
  object.collidable = collidable;
  object.texture = texture;
  return object;
}

OBJECT get_random_obstacle(float x, float y){
  int selected = rand()%1;
  switch (selected) {
    // Rock
    case 0:
      return new_object(x, y, 140.0, 132.0, ROCK_BITMAP, true);
  }
}
