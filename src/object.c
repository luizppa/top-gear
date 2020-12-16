#include "./include/object.h"
#include "./include/environment.h"

OBJECT new_object(float x, float y, ALLEGRO_BITMAP* texture, bool collidable){
  OBJECT object;
  object.position_x = x;
  object.position_y = y;
  object.width = get_bitmap_width(texture);
  object.height = get_bitmap_height(texture);
  object.collidable = collidable;
  object.texture = texture;
  return object;
}

OBJECT get_random_obstacle(float x, float y){
  int selected = rand()%5;
  switch (selected) {
    // Rock
    case 0:
      return new_object(x, y, ROCK_BITMAP, true);
    case 1:
      return new_object(x, y, TIRES_1_BITMAP, true);
    case 2:
      return new_object(x, y, TIRES_2_BITMAP, true);
    case 3:
      return new_object(x, y, BOX_1_BITMAP, true);
    case 4:
      return new_object(x, y, BOX_2_BITMAP, true);
  }
}
