#ifndef OBJECT_H
#define OBJECT_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>


class Object {
  private:
    float x;
    float y;
    float screen_x;
    float screen_y;
    float width;
    float height;
    float apparent_width;
    float apparent_height;
    bool collidable;
    ALLEGRO_BITMAP* texture;

  public:
    Object(float x, float y, ALLEGRO_BITMAP* texture, bool collidable);
    Object* get_random_obstacle(float x, float y);
    float get_x();
    float get_y();
    float get_screen_x();
    float set_screen_x(float screen_x);
    float get_screen_y();
    float set_screen_y(float screen_y);
    float get_width();
    float get_height();
    float get_apparent_width();
    float set_apparent_width(float apparent_width);
    float get_apparent_height();
    float set_apparent_height(float apparent_height);
    bool is_collidable();
};

#endif
