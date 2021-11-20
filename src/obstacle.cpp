#include "../include/object.hpp"
#include "../include/display.hpp"

namespace top_gear {

  Obstacle::Obstacle(float x, float y, ALLEGRO_BITMAP* texture, bool collidable){
    this->x = x;
    this->y = y;
    this->width = display::get_bitmap_width(texture);
    this->height = display::get_bitmap_height(texture);
    this->collidable = collidable;
    this->texture = texture;
  }

  float Obstacle::get_x(){
    return this->x;
  }

  void Obstacle::set_x(float x){
    this->x = x;
  }

  float Obstacle::get_y(){
    return this->y;
  }

  void Obstacle::set_y(float y){
    this->y = y;
  }

  float Obstacle::get_screen_x(){
    return this->screen_x;
  }

  float Obstacle::set_screen_x(float screen_x){
    this->screen_x = screen_x;
  }

  float Obstacle::get_screen_y(){
    return this->screen_y;
  }

  float Obstacle::set_screen_y(float screen_y){
    this->screen_y = screen_y;
  }

  float Obstacle::get_width(){
    return this->width;
  }

  float Obstacle::get_height(){
    return this->height;
  }

  float Obstacle::get_apparent_width(){
    return this->apparent_width;
  }

  float Obstacle::set_apparent_width(float apparent_width){
    this->apparent_width = apparent_width;
  }

  float Obstacle::get_apparent_height(){
    return this->apparent_height;
  }

  float Obstacle::set_apparent_height(float apparent_height){
    this->apparent_height = apparent_height;
  }

  bool Obstacle::is_collidable(){
    return this->collidable;
  }

  ALLEGRO_BITMAP* Obstacle::get_texture(){
    return this->texture;
  }

  Obstacle* Obstacle::get_random_obstacle(float x, float y){
    int selected = rand()%5;
    switch (selected) {
      case 0:
        return new Obstacle(x, y, display::ROCK_BITMAP, true);
      case 1:
        return new Obstacle(x, y, display::TIRES_1_BITMAP, true);
      case 2:
        return new Obstacle(x, y, display::TIRES_2_BITMAP, true);
      case 3:
        return new Obstacle(x, y, display::BOX_1_BITMAP, true);
      case 4:
        return new Obstacle(x, y, display::BOX_2_BITMAP, true);
    }
  }

}
