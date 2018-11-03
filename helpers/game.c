#include "game.h"
#include "car.h"
#include "utils.h"

int oponent_count = 1; // Number of AI controlled oponents on the game
float position; // The road center x coordinate on the screen
float movement_speed = 22.0; // Lateral movement speed
float street_width = 1300.0; // Street base width
float street_length = 500.0; // Street visible spam
float view_angle = 16.0; // Perspective angle (not related to any real wolrd angle value)
float street_left_limit; // Street border
float paralax = 1.3;
int placement = 1;
ALLEGRO_EVENT ev;
CAR player;
CAR* oponents;
CAR** cars;

// Distance from the car wheels to the bottom of the screen
float distance_from_bottom(int i){
  return oponents[i].position_y-player.position_y;
}

// Verify if the car should be rendered
bool is_car_on_sight(int i){
  float delta = get_delta(street_width, street_width/view_angle, street_length, distance_from_bottom(i));
  float apparent_height = oponents[i].height*delta;
  return (distance_from_bottom(i) <= street_length-8 && distance_from_bottom(i) >= -apparent_height);
  // return true;
}

// Verify it the car went off the road
bool is_car_on_track(){
  return (position-(player.width/2) >= street_left_limit && position+(player.width/2) <= sw-street_left_limit);
}

// Draw the scenario
void draw_track(){
  // Street center
  if(debug) al_draw_filled_circle(position, sh-60, 1, BLUE);
  // Landscape
  al_draw_bitmap(LAS_VEGAS_LANDSCAPE_BITMAP, -(2600/2)+(position/paralax), 0, 0);
  // Road
  al_draw_bitmap(LAS_VEGAS_ROAD_BITMAP, position-(street_width/2), sh-street_length, 0);
  if(debug){
    // Left road border
    al_draw_line(position-(street_width/2), sh, position-((street_width/view_angle)/2), sh-street_length, BLUE, 1);
    // Right road border
    al_draw_line(position+(street_width/2), sh, position+((street_width/view_angle)/2), sh-street_length, BLUE, 1);
  }
}

// Draw player's car
void draw_player(){
  // Car texture
  al_draw_bitmap(player.texture, (sw/2)-(player.width/2), sh-player.height, 0);
  if(debug) {
    // Car boundaries
    al_draw_rectangle((sw/2)-(player.width/2), sh-player.height, (sw/2)+(player.width/2), sh, BLUE, 1);
    // Car center
    al_draw_filled_circle(sw/2, sh-(player.height/2), 1, BLUE);
    // Sideline
    al_draw_line(player.screen_position_x-(player.width/2), 0, player.screen_position_x-(player.width/2), sh, BLUE, 1);
    al_draw_line(player.screen_position_x+(player.width/2), 0, player.screen_position_x+(player.width/2), sh, BLUE, 1);
    // Center line
    al_draw_line(0, player.screen_position_y, sw, player.screen_position_y, BLUE, 1);
    // Baseline
    al_draw_line(0, player.position_y, sw, player.position_y, BLUE, 1);
  }
}

// Draw oponent's cars
void draw_oponent(int i){
  CAR* oponent = &oponents[i];
  float distance = distance_from_bottom(i);
  // Perspective rate
  float delta = get_delta(street_width, street_width/view_angle, street_length, distance);
  oponent->apparent_width = oponent->width*delta;
  oponent->apparent_height = oponent->height*delta;
  oponent->screen_position_x = position+oponent->position_x;
  oponent->screen_position_y = sh-distance-(oponent->height/2);
  // Car texture
  /*
    TODO: Fix the x and y relative positions in perspective of the oponents
  */
  al_draw_scaled_bitmap(oponent->texture, 0, 0, oponent->width, oponent->height, position+oponent->position_x-(oponent->apparent_width/2), sh-distance-oponent->apparent_height, oponent->apparent_width, oponent->apparent_height, 0);
  if(debug){
    // Car boundaries
    al_draw_rectangle(position+oponent->position_x-(oponent->apparent_width/2), sh-distance-oponent->apparent_height, position+oponent->position_x+(oponent->apparent_width/2), sh-distance, RED, 1);
    // Sideline
    al_draw_line(oponent->screen_position_x-(oponent->width/2), 0, oponent->screen_position_x-(oponent->width/2), sh, RED, 1);
    al_draw_line(oponent->screen_position_x+(oponent->width/2), 0, oponent->screen_position_x+(oponent->width/2), sh, RED, 1);
    // Center line
    al_draw_line(0, oponent->screen_position_y, sw, oponent->screen_position_y, RED, 1);
    // Baseline
    al_draw_line(0, oponent->screen_position_y+(oponent->apparent_height/2), sw, oponent->screen_position_y+(oponent->apparent_height/2), RED, 1);
  }
}

// Draw screen info
void draw_hud(){
  char position[8];
  char gear[8];
  char speed[16];
  // Position
  /*
    TODO: Fix the event queue problems
    Drawing to much text on the screen causes te update to take longer than
    1/60 seconds which is the refresh rate of the game. Therefore, the event
    queue gets flooded with clock events causing the keyboard input listeners
    (like gears up and down) to be drastically delayed.
    -- Uncomment the three lines bellow to reproduce the error --
  */
  sprintf(position, "%dth", placement);
  draw_text(DISKUN_FONT, 60, YELLOW_ORANGE, 30, 50, ALLEGRO_ALIGN_LEFT, "POSITION", false);
  draw_text(DISKUN_FONT, 80, YELLOW_ORANGE, 30, 120, ALLEGRO_ALIGN_LEFT, position, false);
  // Gears
  sprintf(gear, "%d", player.gear);
  draw_text(DISKUN_FONT, 60, YELLOW_ORANGE, 30, sh-140, ALLEGRO_ALIGN_LEFT, "GEAR", false);
  draw_text(DISKUN_FONT, 80, YELLOW_ORANGE, 30, sh-80, ALLEGRO_ALIGN_LEFT, gear, false);
  // Speed
  sprintf(speed, "%.0f Km/h", player.speed);
  draw_text(DISKUN_FONT, 60, YELLOW_ORANGE, sw-30, sh-140, ALLEGRO_ALIGN_RIGHT, "SPEED", false);
  draw_text(DISKUN_FONT, 80, YELLOW_ORANGE, sw-30, sh-80, ALLEGRO_ALIGN_RIGHT, speed, false);
}

// Draw the game cars
void draw_cars(){
  // Draw cars ahead to the player
  placement = 1;
  for(int i = 0; i < oponent_count; i++){
    if(distance_from_bottom(i) > 0){
      placement++;
      if(is_car_on_sight(i)){
        draw_oponent(i);
        // Uncoment these to see the order in which the cars are being rendered
        // al_flip_display();
        // al_rest(1);
      }
    }
  }
  // Draw player
  draw_player();
  // Draw cars behind the player
  for(int i = 0; i < oponent_count; i++){
    if(is_car_on_sight(i) && distance_from_bottom(i) <= 0){
      draw_oponent(i);
    }
  }
}

// Refresh game screen
void draw_game(){
  al_clear_to_color(GREEN);
  // Draw track boundaries
  draw_track();
  // Draw player and oponents
  draw_cars();
  // Draw screen stats
  draw_hud();
  al_flip_display();
}

// Set gear to a specifc value if the numeric key corresponding to the gear is pressed
void control_gears(){
  if (al_key_down(&key_state, ALLEGRO_KEY_1)) {
    set_gear(&player, 1);
  }
  if (al_key_down(&key_state, ALLEGRO_KEY_2)) {
    set_gear(&player, 2);
  }
  if (al_key_down(&key_state, ALLEGRO_KEY_3)) {
    set_gear(&player, 3);
  }
  if (al_key_down(&key_state, ALLEGRO_KEY_4)) {
    set_gear(&player, 4);
  }
  if (al_key_down(&key_state, ALLEGRO_KEY_5)) {
    set_gear(&player, 5);
  }
  if (al_key_down(&key_state, ALLEGRO_KEY_6)) {
    set_gear(&player, 6);
  }
}

// Move the player based on input
void move(){
  float delta_speed = speed_increase(player.gear, player.speed);
  if(colisions) car_colided(&player, cars, oponent_count+1);
  al_get_keyboard_state(&key_state);
  // Decrease the speed if the player has gone out of the road
  if(is_car_on_track()){
    movement_speed = 22.0;
  }
  else{
    movement_speed = 10.0;
    player.speed = max(0, player.speed - GRASS_SLOW_EFFECT);
  }
  // Going left
  if (al_key_down(&key_state, ALLEGRO_KEY_A)) {
    if(position < max(sw, street_width)+player.width) position += min(movement_speed*((player.speed)/40), movement_speed);
  }
  // Going right
  if (al_key_down(&key_state, ALLEGRO_KEY_D)){
    if(position > 0-player.width) position -= min(movement_speed*((player.speed)/40), movement_speed);
  }
  // Accelerating
  if (al_key_down(&key_state, ALLEGRO_KEY_W)){
    if(delta_speed < 0){
      player.speed = max(0, player.speed + delta_speed);
    }
    else if(player.speed+delta_speed < 0.86*max_speed(player.gear)) {
      player.speed += delta_speed;
    }
    else {
      player.speed += delta_speed/6;
    }
  }
  // Natural deacceleration
  else {
    if(speed_increase(player.gear, player.speed) < 0){
      player.speed = max(0, player.speed + speed_increase(player.gear, player.speed));
    }
    player.speed = max(0, player.speed - NO_ACCELERATE_EFFECT);
  }
  // Break
  if (al_key_down(&key_state, ALLEGRO_KEY_S)){
    player.speed = max(0, player.speed - BREAK_EFFECT);
  }
  player.position_y += player.speed * DISTANCE_VARIATION;
}

// Update game instant
int update(){
  control_gears();
  al_get_keyboard_state(&key_state);
  // Return to menu
  if(al_key_down(&key_state, ALLEGRO_KEY_ESCAPE)) return -1;  // Control player
  move();
  for (int i = 0; i < oponent_count; i++){
    // Controll oponents
    control_ia(&oponents[i], cars, oponent_count+1);
  }
  // Sort oponents array (reportedly causing rendering issues)
  // oponents = quick_sort_cars(oponents, oponent_count);
  // Update screen
  draw_game();
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
  player = new_car(GAME_CAR_BITMAP);
  position = (sw/2)+425;

  // Initialize oponents
  oponents = (CAR*) calloc(oponent_count, sizeof(CAR));
  cars = (CAR**) calloc(oponent_count+1, sizeof(CAR*));
  for (int i = 0; i < oponent_count; i++) {
    oponents[i] = new_oponent(i+1, OPONENT_CAR_BITMAP);
    cars[i] = &oponents[i];
  }
  cars[oponent_count] = &player;

  // Countdown
  for (int i = 3; i > 0; i--) {
    draw_game();
    char countdown[4];
    sprintf(countdown, "%d", i);
    draw_text(DISKUN_FONT, 60, YELLOW, sw/2, sh/3, ALLEGRO_ALIGN_CENTRE, countdown, true);
    al_rest(1);
  }
  al_flush_event_queue(queue);

  // Main loop
  while (true) {
    al_wait_for_event(queue, &ev);
    if(ev.type == ALLEGRO_EVENT_KEY_UP){
      // Gear up
      if(ev.keyboard.keycode == ALLEGRO_KEY_E) gear_up(&player);
      // Gear down
      else if(ev.keyboard.keycode == ALLEGRO_KEY_Q) gear_down(&player);
    }
    // Quit game
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) return 4;
    // Each 1/fps seconds
    else if(ev.type == ALLEGRO_EVENT_TIMER) {
      if(update() == -1){
        return -1;
      }
    }
  }
  return 0;
}
