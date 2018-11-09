#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

struct object {
  float position_x;
  float position_y;
  float screen_position_x;
  float screen_position_y;
  float width;
  float height;
  float apparent_width;
  float apparent_height;
  bool collidable;
  ALLEGRO_BITMAP* texture;
};
typedef struct object OBJECT;

OBJECT new_object(float x, float y, float width, float height, ALLEGRO_BITMAP* texture, bool collidable);
OBJECT get_random_obstacle(float x, float y);
