#include "game.h"

#include <time.h>

bool finished = false;
bool engine_runing = false;
int oponent_count = 7; // Number of AI controlled oponents on the game
int object_count = 0; // Number of static objects on the map
float position; // The road center x coordinate on the screen
float street_width = 1300.0; // Street base width
float street_length = 500.0; // Street visible spam
float view_angle = 16.0; // Perspective angle (not related to any real wolrd angle value)
float street_left_limit; // Street border
float track_length = 50000.0; // Standart at 60000.0
float paralax = 1.3;
double race_time;
int placement = 1;
ALLEGRO_EVENT ev;
CAR player;
CAR* oponents;
CAR** cars;
CAR** leaderboard;
OBJECT* objects;
ALLEGRO_SAMPLE_INSTANCE *player_engine_sound_instance = NULL;


// Distance from the car wheels to the bottom of the screen
float distance_from_bottom(CAR* oponent){
  return oponent->position_y-player.position_y;
}

// Distance from the car wheels to static object
float object_distance(int i){
  return objects[i].position_y-player.position_y;
}

// Verify if the car should be rendered
bool is_car_on_sight(CAR* car){
  float delta = get_delta(street_width, street_width/view_angle, street_length, distance_from_bottom(car));
  float apparent_height = car->height*delta;
  return (distance_from_bottom(car) <= street_length-8 && distance_from_bottom(car) >= -apparent_height-(20*delta));
  // return true;
}

// Verify if the static object should be rendered
bool is_object_visible(int i){
  float delta = get_delta(street_width, street_width/view_angle, street_length, object_distance(i));
  float apparent_height = objects[i].height*delta;
  return (object_distance(i) <= street_length && object_distance(i) >= -apparent_height);
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
    al_draw_line(0, sh-(player.height/2), sw, sh-(player.height/2), BLUE, 1);
  }
}

// Draw oponent's cars
void draw_oponent(CAR* oponent){
  float distance = distance_from_bottom(oponent);
  // Perspective rate
  float delta = get_delta(street_width, street_width/view_angle, street_length, distance);
  oponent->apparent_width = oponent->width*delta;
  oponent->apparent_height = oponent->height*delta;
  oponent->screen_position_x = position+oponent->position_x;
  oponent->screen_position_y = sh-distance-(oponent->height/2);
  float height_variation = oponent->height-oponent->apparent_height;
  // Oponent name
  draw_text(PIXEL_FONT, 14*delta, WHITE, position+(oponent->position_x*delta), sh-distance-(20*delta)-oponent->height+height_variation, ALLEGRO_ALIGN_CENTRE, oponent->name, false);
  // Car texture
  al_draw_scaled_bitmap(oponent->texture, 0, 0, oponent->width, oponent->height, position+(oponent->position_x*delta)-(oponent->apparent_width/2), sh-distance-oponent->apparent_height, oponent->apparent_width, oponent->apparent_height, 0);
  if(debug){
    // Car boundaries
    al_draw_rectangle(position+(oponent->position_x*delta)-(oponent->apparent_width/2), sh-distance-oponent->apparent_height, position+(oponent->position_x*delta)+(oponent->apparent_width/2), sh-distance, RED, 1);
    // Sideline
    al_draw_line(oponent->screen_position_x-(oponent->apparent_width/2), 0, oponent->screen_position_x-(oponent->apparent_width/2), sh, RED, 1);
    al_draw_line(oponent->screen_position_x+(oponent->apparent_width/2), 0, oponent->screen_position_x+(oponent->apparent_width/2), sh, RED, 1);
    // Center line
    al_draw_line(0, sh-distance-(oponent->apparent_height/2), sw, sh-distance-(oponent->apparent_height/2), RED, 1);
    // Baseline
    al_draw_line(0, sh-distance, sw, sh-distance, RED, 1);
  }
}

// Draw a static object to the screen
void draw_object(int i){
  OBJECT* object = &objects[i];
  float distance = object_distance(i);
  // Perspective rate
  float delta = get_delta(street_width, street_width/view_angle, street_length, distance);
  object->apparent_width = object->width*delta;
  object->apparent_height = object->height*delta;
  object->screen_position_x = position+(object->position_x*delta);
  object->screen_position_y = sh-distance-(object->apparent_height/2);
  // Objcet texture
  al_draw_scaled_bitmap(object->texture, 0, 0, object->width, object->height, object->screen_position_x-(object->apparent_width/2), object->screen_position_y-(object->apparent_height/2), object->apparent_width, object->apparent_height, 0);
  if(debug){
    // Object boundaries
    al_draw_rectangle(position+(object->position_x*delta)-(object->apparent_width/2), sh-distance-object->apparent_height, position+(object->position_x*delta)+(object->apparent_width/2), sh-distance, GREEN, 1);
    // Sideline
    al_draw_line(object->screen_position_x-(object->apparent_width/2), 0, object->screen_position_x-(object->apparent_width/2), sh, GREEN, 1);
    al_draw_line(object->screen_position_x+(object->apparent_width/2), 0, object->screen_position_x+(object->apparent_width/2), sh, GREEN, 1);
    // Center line
    al_draw_line(0, sh-distance-(object->apparent_height/2), sw, sh-distance-(object->apparent_height/2), GREEN, 1);
    // Baseline
    al_draw_line(0, sh-distance, sw, sh-distance, GREEN, 1);
  }
}

// Draw screen info
void draw_hud(){
  char position[8];
  char gear[8];
  char speed[16];
  float minimap_heigth = track_length/300;
  float player_minimap_position = min(player.position_y, track_length)/300;
  // Minimap
  al_draw_line(30, sh-250, 30, (sh-250)-minimap_heigth, YELLOW, 6);
  al_draw_filled_circle(30, sh-250, 9, ORANGE);
  al_draw_filled_circle(30, (sh-250)-minimap_heigth, 9, ORANGE);
  for (int i = 0; i < oponent_count; i++) {
    al_draw_filled_circle(30, (sh-250)-(min(oponents[i].position_y, track_length)/300), 8, RED);
  }
  al_draw_filled_circle(30, (sh-250)-player_minimap_position, 9, BLUE);
  // Position
  sprintf(position, "%dth", placement);
  draw_text(DISKUN_FONT, 60, BLUE, 30, sh-160, ALLEGRO_ALIGN_LEFT, "POSITION", false);
  draw_text(DISKUN_FONT, 100, BLUE, 30, sh-100, ALLEGRO_ALIGN_LEFT, position, false);
  // Gears
  sprintf(gear, "%d", player.gear);
  draw_text(DISKUN_FONT, 60, BLUE, sw-30, sh-280, ALLEGRO_ALIGN_RIGHT, "GEAR", false);
  draw_text(DISKUN_FONT, 80, BLUE, sw-30, sh-220, ALLEGRO_ALIGN_RIGHT, gear, false);
  // Speed
  sprintf(speed, "%.0f Km/h", max(0, player.speed));
  draw_text(DISKUN_FONT, 60, BLUE, sw-30, sh-140, ALLEGRO_ALIGN_RIGHT, "SPEED", false);
  draw_text(DISKUN_FONT, 80, BLUE, sw-30, sh-80, ALLEGRO_ALIGN_RIGHT, speed, false);
}

// Draw the game cars
void draw_cars(){
  cars = quick_sort_cars(cars, oponent_count+1);
  // Draw cars sorted cars
  if(!finished) placement = 1;
  for(int i = oponent_count; i >= 0; i--){
    if(cars[i] == &player){
      // Draw player
      draw_player();
    }
    else{
      if(!finished && cars[i]->position_y > player.position_y) placement++;
      if(is_car_on_sight(cars[i])) draw_oponent(cars[i]);
    }
    // Uncoment these to see the order in which the cars are being rendered
    // al_flip_display();
    // al_rest(1);
  }
}

// Draw static objects
// TODO: Fix the order in which the objects are rendered
void draw_objects(){
  for (int i = 0; i < object_count; i++) {
    if(is_object_visible(i)){
      draw_object(i);
    }
  }
}

// Refresh game screen
void draw_game(){
  al_clear_to_color(GREEN);
  // Draw track boundaries
  draw_track();
  // Draw static objects
  draw_objects();
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
  if(colisions) car_colided(&player, cars, objects, oponent_count+1, object_count, true);
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
    if(!engine_runing){
      player_engine_sound_instance = continuously_play_sample(CAR_ENGINE_SOUND);
      engine_runing = true;
    }
    set_sample_volume(player_engine_sound_instance, player.speed/max_speed(player.max_gear));
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
    if(engine_runing && player.speed == 0.0){
      stop_sample(player_engine_sound_instance);
      engine_runing = false;
    }
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
    control_ia(&oponents[i], cars, objects, oponent_count+1, object_count, is_car_on_sight(&oponents[i]));
  }
  for (int i = 1; i < 11; i++) {
    if(object_distance(i) < -objects[i].height) objects[i].position_y = player.position_y + street_length;
  }
  // Sort oponents array (reportedly causing rendering issues)
  // cars = quick_sort_cars(cars, oponent_count+1);
  // Stop timer to avoid flooding the event queue
  al_stop_timer(timer);
  // Update screen
  draw_game();
  // Resume timer
  al_resume_timer(timer);
  if(player.position_y >= track_length) {
    leaderboard = quick_sort_cars(cars, oponent_count+1);
    return 1;
  }
  else return 0;
}

// Deaccelerate and stop
int deaccelerate_until_stop(){
  while (true) {
    al_wait_for_event(queue, &ev);
    // Quit game
    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) return 4;
    // Each 1/fps seconds
    else if(ev.type == ALLEGRO_EVENT_TIMER) {
      for (int i = 0; i < oponent_count; i++){
        // Controll oponents
        control_ia(&oponents[i], cars, objects, oponent_count+1, object_count, is_car_on_sight(&oponents[i]));
      }
      player.speed = max(0, player.speed-(800/(fps*3)));
      player.position_y += player.speed * DISTANCE_VARIATION;
      // Stop timer to avoid flooding the event queue
      al_stop_timer(timer);
      // Update screen
      draw_game();
      // Resume timer
      al_resume_timer(timer);
      set_sample_volume(player_engine_sound_instance, player.speed/max_speed(player.max_gear));
      if(engine_runing && player.speed <= 0.0){
        stop_sample(player_engine_sound_instance);
        engine_runing = false;
      }
      if(player.speed <= 0){
        al_rest(3);
        al_flush_event_queue(queue);
        return 0;
      }
    }
  }
  return 0;
}

// Show match leaderboards
int show_leaderboard(){
  char result[50];
  char duration[50];
  char competitor[50];
  sprintf(result, "YOU FINISHED %dth", placement);
  sprintf(duration, "RACE DURATION: %.2fs", race_time);
  stop_music(music);
  if(placement <= 3) music = set_music(QUALIFIED_MUSIC);
  else music = set_music(FRANKFURT_MUSIC);
  start_music(music, false);
  clear_display(BLUE, false);
  if(placement <= 3){
    draw_text(PIXEL_FONT, 100, YELLOW, sw/2, sh/2-(200), ALLEGRO_ALIGN_CENTER, "CONGRATULATIONS", false);
    draw_text(PIXEL_FONT, 100, YELLOW, sw/2, sh/2-(80), ALLEGRO_ALIGN_CENTER, result, false);
  }
  else draw_text(PIXEL_FONT, 100, YELLOW, sw/2, sh/2-(80), ALLEGRO_ALIGN_CENTER, "YOU DID NOT QUALIFIED", false);
  draw_text(PIXEL_FONT, 28, ORANGE, sw/2, (sh/2)+100, ALLEGRO_ALIGN_CENTER, "Press enter to continue", false);
  al_flip_display();
  while (true) {
    al_wait_for_event(queue, &ev);
    if(ev.type == ALLEGRO_EVENT_KEY_UP && ev.keyboard.keycode == ALLEGRO_KEY_ENTER) break;
  }
  clear_display(BLUE, false);
  draw_text(PIXEL_FONT, 80, ORANGE, 30, 10, ALLEGRO_ALIGN_LEFT, duration, false);
  for (int i = 0; i < oponent_count+1; i++) {
    if(i+1 == placement) sprintf(competitor, "%d. PLAYER", i+1);
    else sprintf(competitor, "%d. %s", i+1, leaderboard[i]->name);
    draw_text(PIXEL_FONT, 60, YELLOW, 30, ((i+1)*80)+20, ALLEGRO_ALIGN_LEFT, competitor, false);
  }
  al_flip_display();
  while (true) {
    al_wait_for_event(queue, &ev);
    if(ev.type == ALLEGRO_EVENT_KEY_UP && ev.keyboard.keycode == ALLEGRO_KEY_ENTER) break;
  }
}

// Setup game environment
void setup(ALLEGRO_BITMAP* player_texture, CAR** tournament_cars){
  // Initialize environment
  int player_position = oponent_count+1;
  street_left_limit = (sw-street_width)/2;
  object_count = 40;
  objects = (OBJECT*) calloc(object_count, sizeof(OBJECT));
  oponents = (CAR*) calloc(oponent_count, sizeof(CAR));
  cars = (CAR**) calloc(oponent_count+1, sizeof(CAR*));

  // Initialize objects
  objects[0] = new_object(0, track_length, 1295.0, 691.0, FINISH_LINE, false);
  for (int i = 1; i < 6; i++) {
    objects[i] = new_object(-(street_width/2)-120, i*120, 120.0, 120.0, ROAD_SIGN, false);
    objects[i+5] = new_object((street_width/2)+120, i*120, 120.0, 120.0, ROAD_SIGN, false);
  }
  for (int i = 11; i < object_count; i++) {
    float x = (rand()%(int)(street_width-30))-((street_width-30)/2), y = (rand()%(int)(track_length-(track_length/4)+1))+(track_length/4);
    objects[i] = get_random_obstacle(x, y);
  }

  // Initialize oponents
  if(tournament_cars == NULL){
    int car_type, car_color;
    for (int i = 0; i < oponent_count; i++) {
      car_type = (rand()%4)+1;
      car_color = rand()%7;
      oponents[i] = new_oponent(i+1, get_car(car_type, car_color));
      cars[i] = &oponents[i];
    }
  }
  else{
    for (int i = 0; i < oponent_count; i++) {
      oponents[i] = *tournament_cars[i];
      cars[i] = &oponents[i];
    }
  }

  // Change track to Las Vegas theme
  stop_music(music);
  music = set_music(LAS_VEGAS_MUSIC);
  start_music(music, true);

  // Initialize player
  player = new_car(player_texture);
  cars[oponent_count] = &player;
  if(player_position < 4){
    player.position_y = (3*STARTING_DISTANCE);
  }
  else if(player_position < 7){
    player.position_y = (2*STARTING_DISTANCE);
  }
  else if(player_position < 10){
    player.position_y = (1*STARTING_DISTANCE);
  }
  else if(player_position < 13){
    player.position_y = (0*STARTING_DISTANCE);
  }
  float x = (player_position%3)-1;
  player.position_x = (x*25)+(x*400);
  position = (sw/2)-player.position_x;

}

// Main
int play(ALLEGRO_BITMAP* player_texture, CAR** tournament_cars, int oponents_amount){
  int result;
  clock_t begin, end;
  oponent_count = oponents_amount;

  setup(player_texture, tournament_cars);

  // Countdown
  for (int i = 3; i > 0; i--) {
    draw_game();
    char countdown[4];
    sprintf(countdown, "%d", i);
    draw_text(DISKUN_FONT, 60, BLUE, sw/2, sh/3, ALLEGRO_ALIGN_CENTRE, countdown, true);
    play_sample(READY_SOUND);
    al_rest(1);
  }
  play_sample(GO_SOUND);
  al_flush_event_queue(queue);

  // Main loop
  while (true) {
    begin = clock();
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
      result = update();
      if(result == -1){
        if(engine_runing) stop_sample(player_engine_sound_instance);
        return -1;
      }
      else if(result == 1){
        end = clock();
        finished = true;
        race_time = (double)(end-begin)/CLOCKS_PER_SEC;
        // show_leaderboard();
        return deaccelerate_until_stop();
      }
    }
  }
  return 0;
}
