#include "game.h"
#include "car.h"

int oponent_count = 12;
float position;
float moviment_speed = 22.0;
float street_width = 1300.0;
float street_length = 500.0;
float street_left_limit;
float view_angle = 16.0;
CAR player;
CAR* oponents;

float max(float a, float b){
  if(a > b) return a;
  return b;
}

float min(float a, float b){
  if(a < b) return a;
  return b;
}

float distance_to(int i){
  return oponents[i].position_y - player.position_y;
}

bool is_car_on_sight(int i){
  return (distance_to(i) <= 650 && distance_to(i) >= -20);
}

void draw_track(){
  // Street center
  // al_draw_filled_circle(position, sh-60, 1, BLUE);
  // Sky
  al_draw_filled_rectangle(0, 0, sw, sh-street_length, LIGHT_BLUE);
  // Left strret border
  al_draw_line(position-(street_width/2), sh, position-((street_width/view_angle)/2), sh-street_length, BLUE, 1);
  // Right street border
  al_draw_line(position+(street_width/2), sh, position+((street_width/view_angle)/2), sh-street_length, BLUE, 1);
  // al_draw_line(0, sh-street_length, sw, sh-street_length, BLUE, 1);
}

void draw_car(){
  // Car texture
  al_draw_bitmap(player.texture, (sw/2)-(player.width/2), (sh)-(20+player.height), 0);
  // Car boundaries
  // al_draw_rectangle((sw/2)-(player.width/2), sh-(player.height+20), (sw/2)+(player.width/2), sh-20.0, RED, 1);
  // Car center
  // al_draw_filled_circle(sw/2, sh-(20+(player.height/2)), 1, RED);
}

void draw_oponent(int i){
  CAR oponent = oponents[i];
  al_draw_bitmap(oponent.texture, position+(oponent.position_x-oponent.width/2), sh-(20+distance_to(i)), 0);
}

void draw_hud(){
  char gear[10];
  char speed[20];
  sprintf(gear, "%d", player.gear);
  sprintf(speed, "%.0f Km/h", player.speed);
  draw_text(DISKUN_FONT, 60, YELLOW, 30, sh-140, ALLEGRO_ALIGN_LEFT, "GEAR", false);
  draw_text(DISKUN_FONT, 80, YELLOW, 30, sh-80, ALLEGRO_ALIGN_LEFT, gear, false);
  draw_text(DISKUN_FONT, 60, YELLOW, sw-30, sh-140, ALLEGRO_ALIGN_RIGHT, "SPEED", false);
  draw_text(DISKUN_FONT, 80, YELLOW, sw-30, sh-80, ALLEGRO_ALIGN_RIGHT, speed, false);
}

void redraw_game(){
  al_clear_to_color(GREY);
  draw_track();
  for(int i = oponent_count-1; i >= 0; i--){
    if(is_car_on_sight(i) && distance_to(i) > STANDARD_CAR_HEIGHT){
      draw_oponent(i);
    }
  }
  draw_car();
  for(int i = oponent_count-1; i >= 0; i--){
    if(is_car_on_sight(i) && distance_to(i) < STANDARD_CAR_HEIGHT){
      draw_oponent(i);
    }
  }
  draw_hud();
  al_flip_display();
}

bool is_car_on_track(){
  return (position-(player.width/2) >= street_left_limit && position+(player.width/2) <= sw-street_left_limit);
}

void move(){
  al_get_keyboard_state(&key_state);
  if (al_key_down(&key_state, ALLEGRO_KEY_A)) {
    if(position < max(sw, street_width)+player.width) position += min(moviment_speed*((player.speed)/40), moviment_speed);
  }
  if (al_key_down(&key_state, ALLEGRO_KEY_D)){
    if(position > 0-player.width) position -= min(moviment_speed*((player.speed)/40), moviment_speed);
  }
  if (al_key_down(&key_state, ALLEGRO_KEY_W)){
    if(speed_increase(player.gear, player.speed) < 0){
      player.speed = max(0, player.speed + speed_increase(player.gear, player.speed));
    }
    else if(player.speed+speed_increase(player.gear, player.speed) < 0.86*max_speed(player.gear)) {
      player.speed += speed_increase(player.gear, player.speed);
    }
    else {
      player.speed += speed_increase(player.gear, player.speed)/6;
    }
  }
  else {
    if(speed_increase(player.gear, player.speed) < 0){
      player.speed = max(0, player.speed + speed_increase(player.gear, player.speed));
    }
    player.speed = max(0, player.speed - NO_ACCELERATE_EFFECT);
  }
  if (al_key_down(&key_state, ALLEGRO_KEY_S)){
    player.speed = max(0, player.speed - BREAK_EFFECT);
  }
  player.position_y += player.speed * DISTANCE_VARIATION;
}

int update(){
  if(is_car_on_track()){
    moviment_speed = 22.0;
  }
  else{
    moviment_speed = 10.0;
    player.speed = max(0, player.speed - GRASS_SLOW_EFFECT);
  }
  move();
  oponents = quick_sort_cars(oponents, oponent_count);
  redraw_game();
  return 0;
}

int play(){
  // Initialize environment
  street_left_limit = (sw-street_width)/2;

  // Change track to Las Vegas theme
  stop_music(music);
  music = set_music(LAS_VEGAS_MUSIC);
  start_music(music, true);

  // Initialize player
  position = (sw/2);
  player = new_car(GAME_CAR_BITMAP);

  // Initialize oponents
  oponents = (CAR*) calloc(oponent_count, sizeof(CAR));
  for (int i = 0; i < oponent_count; i++) {
    oponents[i] = new_oponent(i+1, OPONENT_CAR_BITMAP);
  }

  redraw_game();
  while (true) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(queue, &ev);
    // Return to menu
    if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
      if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) return -1;
      // Gear up
      else if(ev.keyboard.keycode == ALLEGRO_KEY_E) gear_up(&player);
      // Gear down
      else if(ev.keyboard.keycode == ALLEGRO_KEY_Q) gear_down(&player);
    }
    // Quit game
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) return 4;
    // Each 1/fps seconds
    if(ev.type == ALLEGRO_EVENT_TIMER) {
      if(update() == -1){
        return -1;
      }
    }
  }
  return 0;
}
