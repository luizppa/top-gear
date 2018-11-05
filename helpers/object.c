#include "object.h"

OBJECT new_object( float x, float y, float width, float height, ALLEGRO_BITMAP* texture){
  OBJECT object;
  object.position_x = x;
  object.position_y = y;
  object.width = width;
  object.height = height;
  object.texture = texture;
  return object;
}
