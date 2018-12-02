#include "game.h"

#include <time.h>

bool finished = false;
bool engine_running = false;
int map; // The map code
int placement = 1; // Player race position
int oponent_count; // Number of AI controlled oponents on the game
int object_count = 0; // Number of static objects on the map
float position; // The road center x coordinate on the screen
float street_width = 1300.0; // Street base width
float street_length = 500.0; // Street visible spam
float view_angle = 16.0; // Perspective angle (not related to any real wolrd angle value)
float street_left_limit; // Street border
float track_length = 50000.0; // Standart at 50000.0
float paralax = 1.3;
double race_time;
char* map_music_title;
ALLEGRO_EVENT ev;
CAR player;
CAR* oponents;
CAR** cars;
OBJECT* objects;
ALLEGRO_COLOR map_soil;
ALLEGRO_BITMAP* map_landscape = NULL;
ALLEGRO_SAMPLE_INSTANCE *player_engine_sound_instance = NULL;

void countdown();
void clear_game();

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
  al_draw_bitmap(map_landscape, -(2600/2)+(position/paralax), 0, 0);
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
  ALLEGRO_FONT *name_font = font(PIXEL_FONT, 14*delta);
  draw_text(name_font, WHITE, position+(oponent->position_x*delta), sh-distance-(20*delta)-oponent->height+height_variation, ALLEGRO_ALIGN_CENTRE, oponent->name, false);
  al_destroy_font(name_font);
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
  float minimap_heigth = track_length/300;
  float player_minimap_position = min(player.position_y, track_length)/300;
  int gear_progress = min(10, get_gear_progress(player));

  // Minimap
  al_draw_line(30, sh-250, 30, (sh-250)-minimap_heigth, YELLOW, 6);
  al_draw_filled_circle(30, sh-250, 9, ORANGE);
  al_draw_filled_circle(30, (sh-250)-minimap_heigth, 9, ORANGE);
  for (int i = 0; i < oponent_count; i++) {
    al_draw_filled_circle(30, (sh-250)-(min(oponents[i].position_y, track_length)/300), 9, RED);
  }
  al_draw_filled_circle(30, (sh-250)-player_minimap_position, 9, BLUE);
  // Position
  al_draw_text(DISKUN_60, YELLOW, 30, sh-160, ALLEGRO_ALIGN_LEFT, "POSITION");
  al_draw_textf(DISKUN_100, YELLOW, 30, sh-100, ALLEGRO_ALIGN_LEFT, "%dth", placement);
  // Gears
  al_draw_text(DISKUN_60, YELLOW, sw-30, sh-280, ALLEGRO_ALIGN_RIGHT, "GEAR");
  al_draw_textf(DISKUN_80, YELLOW, sw-30, sh-220, ALLEGRO_ALIGN_RIGHT, "%d", player.gear);
  al_draw_filled_rounded_rectangle(15, 15, 258, 85, 5.0, 5.0, YELLOW);
  al_draw_filled_rounded_rectangle(20, 20, 253, 80, 3.0, 3.0, rgb(50, 50, 50));
  for (int i = 1; i <= gear_progress; i++) {
    int red = (int)(200*(i/10.0));
    int green = 65+(int)(190*((10-i)/10.0));
    al_draw_filled_rectangle(i*23, 23, (i*23)+20, 77, rgb(red, green, 40));
  }
  // Speed
  al_draw_text(DISKUN_60, YELLOW, sw-30, sh-140, ALLEGRO_ALIGN_RIGHT, "SPEED");
  al_draw_textf(DISKUN_80, YELLOW, sw-30, sh-80, ALLEGRO_ALIGN_RIGHT, "%.0f Km/h", max(0, player.speed));
  // Nitrox
  al_draw_text(DISKUN_60, YELLOW, sw-30, 20, ALLEGRO_ALIGN_RIGHT, "NITROX");
  al_draw_filled_rounded_rectangle(sw-60, 90, sw-30, 300, 5.0, 5.0, YELLOW);
  al_draw_filled_rounded_rectangle(sw-55, 95, sw-35, 295, 3.0, 3.0, rgb(50, 50, 50));
  al_draw_filled_rectangle(sw-53, 293-(196*(player.nitrox/100)), sw-37, 293, RED);
}

// Draw the game cars
void draw_cars(){
  if(!finished) quick_sort_cars(cars, oponent_count+1);
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
// TODO: Fix the order in which the objects are rendered (not sure how is that possible)
void draw_objects(){
  for (int i = 0; i < object_count; i++) {
    if(is_object_visible(i)){
      draw_object(i);
    }
  }
}

// Refresh game screen
void draw_game(){
  al_clear_to_color(map_soil);
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

// Draw game screen on pause
void draw_paused_game(int op){
  ALLEGRO_COLOR colors[4];
  colors[0] = YELLOW;
  colors[1] = YELLOW;
  colors[2] = YELLOW;
  colors[3] = YELLOW;
  colors[op] = WHITE;
  al_clear_to_color(map_soil);
  // Draw track boundaries
  draw_track();
  // Draw static objects
  draw_objects();
  // Draw player and oponents
  draw_cars();
  // Draw screen stats
  draw_hud();
  // Draw dialog box
  al_draw_filled_rectangle((sw/2)-300, (sh/2)-200, (sw/2)+300, (sh/2)+200, BLUE);
  al_draw_rectangle((sw/2)-300, (sh/2)-200, (sw/2)+300, (sh/2)+200, YELLOW, 5);
  draw_text(PIXEL_28, colors[0], sw/2, (sh/2)-71, ALLEGRO_ALIGN_CENTRE, "RESUME", false);
  draw_text(PIXEL_28, colors[1], sw/2, (sh/2)-33, ALLEGRO_ALIGN_CENTRE, "RESTART", false);
  draw_text(PIXEL_28, colors[2], sw/2, (sh/2)+5, ALLEGRO_ALIGN_CENTRE, "OPTIONS", false);
  draw_text(PIXEL_28, colors[3], sw/2, (sh/2)+38, ALLEGRO_ALIGN_CENTRE, "MAIN MENU", true);
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
  if(collisions) car_collided(&player, cars, objects, oponent_count+1, object_count, true);
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
    if(!engine_running){
      player_engine_sound_instance = continuously_play_sample(CAR_ENGINE_SOUND);
      engine_running = true;
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
    if(engine_running && player.speed == 0.0){
      stop_sample(player_engine_sound_instance);
      engine_running = false;
    }
    if(speed_increase(player.gear, player.speed) < 0){
      player.speed = max(0, player.speed + speed_increase(player.gear, player.speed));
    }
    player.speed = max(0, player.speed - NO_ACCELERATE_EFFECT);
  }
  // Boost
  if(al_key_down(&key_state, ALLEGRO_KEY_LSHIFT)){
    if(player.nitrox > 33.333/60.0){
      player.speed += (15.0/60.0);
      player.nitrox = max(0, player.nitrox-(33.333/60.0));
    }
  }
  else{
    if(player.nitrox < 100.0){
      player.nitrox = min(100.0, player.nitrox+(4.0/60.0));
    }
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
  // Control player
  move();
  for (int i = 0; i < oponent_count; i++){
    // Controll oponents
    control_ia(&oponents[i], cars, objects, oponent_count+1, object_count, is_car_on_sight(&oponents[i]));
  }
  for (int i = 0; i < 5; i++) {
    if(object_distance(i) < -objects[i].height){
      objects[i].position_y = objects[i].position_y + ((5-1)*120) + objects[i].height;
      objects[i+5].position_y = objects[i+5].position_y + ((5-1)*120) + objects[i].height;
    }
  }
  // Update screen
  draw_game();
  if(player.position_y >= track_length) {
    return 1;
  }
  else return 0;
}

void restart(){
  restart_positions(oponents, oponent_count);
  quick_sort_cars(cars, oponent_count+1);
  int player_position = oponent_count+1;
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
  player.gear = 1;
  player.speed = 0.0;
  for (int i = 0; i < 5; i++) {
    objects[i].position_x = -(street_width/2)-120;
    objects[i].position_y = i*120;
    objects[i+5].position_x = (street_width/2)+120;
    objects[i+5].position_y = i*120;
  }
  restart_music(music);
  countdown();
  al_set_timer_count(timer, 0);
}

// Handle paused game
int pause(){
  int op = 0;
  draw_paused_game(op);
  set_music_volume(music, 0.6);
  while (true) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(queue, &ev);
    // Quit game
    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      return 4;
    }
    else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      // Return to title
      if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
        play_sample(MENU_BACK_SOUND);
        break;
      }
      switch (ev.keyboard.keycode) {
        // Selection UP
        case ALLEGRO_KEY_UP:
        case ALLEGRO_KEY_W:
          if(op > 0){
            op--;
            play_sample(MENU_MOVE_SOUND);
            // Update screen
            draw_paused_game(op);
          }
          break;
        // Selection DOWN
        case ALLEGRO_KEY_DOWN:
        case ALLEGRO_KEY_S:
          if(op < 3){
            op++;
            play_sample(MENU_MOVE_SOUND);
            // Update screen
            draw_paused_game(op);
          }
          break;
        // Confirm selection
        case ALLEGRO_KEY_ENTER:
          play_sample(MENU_SELECT_SOUND);
          switch (op) {
            case 0:
              // Resume
              return 0;
              break;
            case 1:
              // Restart race
              restart();
              return 0;
            case 2:
              // Configure options
              if(options_menu() == 4) return 4;
              op = 0;
              break;
            case 3:
              // Quit to title
              return -1;
          }
          draw_paused_game(op);
      }
    }
  }
  play_sample(MENU_BACK_SOUND);
  set_music_volume(music, 1.0);
  return 0;
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
      if(engine_running && player.speed <= 0.0){
        stop_sample(player_engine_sound_instance);
        engine_running = false;
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
  float landscape_position = 0.0;
  bool going_right = true;
  int race_time_minutes = (int)(race_time/60);
  stop_music(music);
  if(placement <= 3) music = set_music(QUALIFIED_MUSIC);
  else music = set_music(DISQUALIFIED_MUSIC);
  start_music(music, false);
  clear_display(BLUE, false);
  if(placement <= 3){
    draw_text(PIXEL_32, YELLOW, sw/2, 25, ALLEGRO_ALIGN_CENTER, "CONGRATULATIONS", false);
    al_draw_textf(PIXEL_32, YELLOW, sw/2, 67, ALLEGRO_ALIGN_CENTER, "YOU FINISHED %dth", placement);
  }
  else draw_text(PIXEL_32, YELLOW, sw/2, 25, ALLEGRO_ALIGN_CENTER, "YOU DID NOT QUALIFIED", false);
  draw_text(PIXEL_22, ORANGE, sw/2, 150, ALLEGRO_ALIGN_CENTER, "Press enter to continue", false);
  while (true) {
    al_wait_for_event(queue, &ev);
    // Continue
    if(ev.type == ALLEGRO_EVENT_KEY_UP){
      if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER || ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) break;
    }
    // Update screen
    else if(ev.type == ALLEGRO_EVENT_TIMER){
      al_draw_bitmap(map_landscape, (sw/2)-1500+landscape_position, sh-250, 0);
      if(going_right && (sw/2)-1500+landscape_position >= 0) going_right = false;
      else if(!going_right && (sw/2)-1500+landscape_position <= -3000+sw) going_right = true;
      if(going_right) landscape_position += 15.0/60.0;
      else landscape_position -= 15.0/60.0;
      al_flip_display();
    }
    // Quit game
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) return 4;
  }
  clear_display(BLUE, false);
  al_draw_textf(PIXEL_28, ORANGE, 30, 10, ALLEGRO_ALIGN_LEFT, "RACE DURATION: %d min %.0f sec", race_time_minutes, race_time-(race_time_minutes*60));
  for (int i = 0; i < oponent_count+1; i++) {
    if(i%2 == 1) al_draw_textf(PIXEL_28, YELLOW, (sw/2)+30, ((i)*28)+38, ALLEGRO_ALIGN_LEFT, "%d: %s", i+1, cars[oponent_count-i]->name);
    else al_draw_textf(PIXEL_28, YELLOW, 30, ((i+1)*28)+38, ALLEGRO_ALIGN_LEFT, "%d: %s", i+1, cars[oponent_count-i]->name);
  }
  al_draw_bitmap(map_landscape, (sw/2)-1500+landscape_position, sh-250, 0);
  al_flip_display();
  while (true) {
    al_wait_for_event(queue, &ev);
    // Continue
    if(ev.type == ALLEGRO_EVENT_KEY_UP){
      if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER || ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) break;
    }
    // Update screen
    else if(ev.type == ALLEGRO_EVENT_TIMER){
      al_draw_bitmap(map_landscape, (sw/2)-1500+landscape_position, sh-250, 0);
      if(going_right && (sw/2)-1500+landscape_position >= 0) going_right = false;
      else if(!going_right && (sw/2)-1500+landscape_position <= -3000+sw) going_right = true;
      if(going_right) landscape_position += 15.0/60.0;
      else landscape_position -= 15.0/60.0;
      al_flip_display();
    }
    // Quit game
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) return 4;
  }
}

// Setup game environment
void setup(ALLEGRO_BITMAP* player_texture, CAR* tournament_cars, bool single_match){
  // Initialize environment
  int player_position = oponent_count+1;
  street_left_limit = (sw-street_width)/2;
  object_count = 30;
  objects = (OBJECT*) malloc(object_count*sizeof(OBJECT));
  cars = (CAR**) malloc((oponent_count+1)*sizeof(CAR*));

  // Initialize objects
  objects[object_count-1] = new_object(0, track_length, FINISH_LINE, false);
  for (int i = 0; i < 5; i++) {
    objects[i] = new_object(-(street_width/2)-120, i*120, ROAD_SIGN, false);
    objects[i+5] = new_object((street_width/2)+120, i*120, ROAD_SIGN, false);
  }
  for (int i = 10; i < object_count-1; i++) {
    float x = (rand()%(int)(street_width-30))-((street_width-30)/2), y = (rand()%(int)(track_length-(track_length/4)+1))+(track_length/4);
    objects[i] = get_random_obstacle(x, y);
  }

  // Initialize oponents
  if(tournament_cars){
    oponents = tournament_cars;
  }
  else{
    oponents = (CAR*) malloc(oponent_count*sizeof(CAR));
    int car_type, car_color;
    for (int i = 0; i < oponent_count; i++) {
      car_type = (rand()%4)+1;
      car_color = rand()%7;
      oponents[i] = new_oponent(i+1, get_car(car_type, car_color));
    }
  }
  for (int i = 0; i < oponent_count; i++) {
    cars[i] = &oponents[i];
  }

  // Change track to Las Vegas theme
  stop_music(music);
  map_music_title = get_map_music_title(map);
  music = get_map_music(map);
  start_music(music, true);

  // Initialize Landscape
  map_landscape = get_map_landscape(map);
  map_soil = get_map_color(map);

  // Initialize player
  player = new_car(player_texture);
  cars[oponent_count] = &player;
  quick_sort_cars(cars, oponent_count+1);
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

// Countdown to race start
void countdown(){
  for (int i = 3; i > 0; i--) {
    draw_game();
    al_draw_textf(DISKUN_60, YELLOW, sw/2, sh/3, ALLEGRO_ALIGN_CENTRE, "%d", i);
    al_flip_display();
    play_sample(READY_SOUND);
    al_rest(1);
  }
  play_sample(GO_SOUND);
  al_flush_event_queue(queue);
}

// Single match
int play(ALLEGRO_BITMAP* player_texture, CAR* tournament_cars, int oponents_amount, int choosen_map, bool single_match){
  int result;
  map = choosen_map;
  oponent_count = oponents_amount;

  setup(player_texture, tournament_cars, single_match);

  countdown();
  al_set_timer_count(timer, 0);
  // Main loop
  while (true) {
    al_wait_for_event(queue, &ev);
    if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
      // Gear up
      if(ev.keyboard.keycode == ALLEGRO_KEY_E) gear_up(&player);
      // Gear down
      else if(ev.keyboard.keycode == ALLEGRO_KEY_Q) gear_down(&player);
      // Pause
      if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
        if(engine_running){
          stop_sample(player_engine_sound_instance);
          engine_running = false;
        }
        al_stop_timer(timer);
        result = pause();
        al_resume_timer(timer);
        set_music_volume(music, 1.0);
        if(result == -1 || result == 4) return result;
        if(result == 0) {
          player_engine_sound_instance = continuously_play_sample(CAR_ENGINE_SOUND);
          set_sample_volume(player_engine_sound_instance, player.speed/max_speed(player.max_gear));
          engine_running = true;
          draw_game();
        }
      }
    }
    // Quit game
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) return 4;
    // Each 1/fps seconds
    else if(ev.type == ALLEGRO_EVENT_TIMER) {
      result = update();
      if(result == -1){
        if(engine_running) stop_sample(player_engine_sound_instance);
        return -1;
      }
      else if(result == 1){
        finished = true;
        race_time = al_get_timer_count(timer)/fps;
        deaccelerate_until_stop();
        // if(tournament_cars == NULL) free(oponents);
        if(single_match){
          result = show_leaderboard();
          clear_game();
          return result;
        }
        else return 0;
      }
    }
  }
  return 0;
}

int tournament(ALLEGRO_BITMAP* player_texture, int oponents_amount){
  CAR *tournament_cars = (CAR*) calloc(oponents_amount, sizeof(CAR));
  int car_type, car_color, op;
  for (int i = 0; i < oponents_amount; i++) {
    car_type = (rand()%4)+1;
    car_color = rand()%7;
    tournament_cars[i] = new_oponent(i+1, get_car(car_type, car_color));
  }
  for (int i = 0; i < 4; i++) {
    op = play(player_texture, tournament_cars, oponents_amount, i, (i==3));
    clear_game();
    restart_positions(tournament_cars, oponents_amount);
    if(op == 4 || op == -1) return op;
  }
  free(tournament_cars);
  return 0;
}

void clear_game(){
  free(cars);
}
