#include "../include/object.hpp"
#include "../include/display.hpp"

namespace top_gear {

  Object::Object(float x, float y, ALLEGRO_BITMAP* texture, bool collidable){
    this->x = x;
    this->y = y;
    this->width = display::get_bitmap_width(texture);
    this->height = display::get_bitmap_height(texture);
    this->collidable = collidable;
    this->texture = texture;
  }

  Object* get_random_obstacle(float x, float y){
    int selected = rand()%5;
    switch (selected) {
      case 0:
        return new Object(x, y, display::ROCK_BITMAP, true);
      case 1:
        return new Object(x, y, display::TIRES_1_BITMAP, true);
      case 2:
        return new Object(x, y, display::TIRES_2_BITMAP, true);
      case 3:
        return new Object(x, y, display::BOX_1_BITMAP, true);
      case 4:
        return new Object(x, y, display::BOX_2_BITMAP, true);
    }
  }

  float Object::get_x(){
    return this->x;
  }

  float Object::get_y(){
    return this->y;
  }

  float Object::get_screen_x(){
    return this->screen_x;
  }

  float Object::set_screen_x(float screen_x){
    this->screen_x = screen_x;
  }

  float Object::get_screen_y(){
    return this->screen_y;
  }

  float Object::set_screen_y(float screen_y){
    this->screen_y = screen_y;
  }

  float Object::get_width(){
    return this->width;
  }

  float Object::get_height(){
    return this->height;
  }

  float Object::get_apparent_width(){
    return this->apparent_width;
  }

  float Object::set_apparent_width(float apparent_width){
    this->apparent_width = apparent_width;
  }

  float Object::get_apparent_height(){
    return this->apparent_height;
  }

  float Object::set_apparent_height(float apparent_height){
    this->apparent_height = apparent_height;
  }

  bool Object::is_collidable(){
    return this->collidable;
  }

}
