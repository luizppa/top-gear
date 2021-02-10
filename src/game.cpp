#include "../include/game.hpp"
#include "../include/display.hpp"
#include "../include/environment.hpp"
#include "../include/utils.hpp"
#include "../include/menus.hpp"
#include "../include/fonts.hpp"
#include "../include/colors.hpp"
#include "../include/sounds.hpp"

#include <time.h>
#include <unistd.h>

namespace top_gear {

  bool finished = false;
  bool engine_running = false;
  bool boosting = false;
  bool best_time = false;
  int map; // The map code
  int placement = 1; // Player race position
  int oponent_count; // Number of AI controlled oponents on the game
  int object_count = 0; // Number of static objects on the map
  int record = 0;
  float position; // The road center x coordinate on the screen
  float street_width = 1300.0; // Street base width
  float street_length = 500.0; // Street visible s pam
  float view_angle = 16.0; // Perspective angle (not related to any real wolrd angle value)
  float street_left_limit; // Street border
  float track_length = 50000.0;
  float paralax = 1.3;
  double race_time;
  char* map_music_title;
  ALLEGRO_EVENT ev;
  Car* player;
  Car** oponents;
  Car** cars;
  Object** objects;
  ALLEGRO_COLOR map_soil;
  ALLEGRO_BITMAP* map_landscape = nullptr;
  ALLEGRO_SAMPLE_INSTANCE *player_engine_sound_instance = nullptr;

  void countdown();
  void clear_game();

  // Distance from the car wheels to the bottom of the screen
  float distance_from_bottom(Car* oponent){
    return oponent->get_y() - player->get_y();
  }

  // Distance from the car wheels to static object
  float object_distance(int i){
    return objects[i]->get_y() - player->get_y();
  }

  // Verify if the car should be rendered
  bool is_car_visible(Car* car){
    float delta = utils::get_delta(street_width, street_width/view_angle, street_length, distance_from_bottom(car));
    float apparent_height = car->get_height()*delta;
    return (distance_from_bottom(car) <= street_length-8 && distance_from_bottom(car) >= -apparent_height-(20*delta));
    // return true;
  }

  // Verify if the static object should be rendered
  bool is_object_visible(int i){
    float delta = utils::get_delta(street_width, street_width/view_angle, street_length, object_distance(i));
    float apparent_height = objects[i]->get_height()*delta;
    return (object_distance(i) <= street_length && object_distance(i) >= -apparent_height);
  }

  // Verify it the car went off the road
  bool is_car_on_track(){
    return (position-(player->get_width()/2) >= street_left_limit && position+(player->get_width()/2) <= SCREEN_WIDTH-street_left_limit);
  }

  // Draw the scenario
  void draw_track(){
    al_draw_bitmap(map_landscape, -(2600/2)+(position/paralax), 0, 0);
    al_draw_bitmap(display::LAS_VEGAS_ROAD_BITMAP, position-(street_width/2), SCREEN_HEIGHT-street_length, 0);
    if(environment::debug){
      al_draw_filled_circle(position, SCREEN_HEIGHT-60, 1, colors::BLUE);
      al_draw_line(position-(street_width/2), SCREEN_HEIGHT, position-((street_width/view_angle)/2), SCREEN_HEIGHT-street_length, colors::BLUE, 1);
      al_draw_line(position+(street_width/2), SCREEN_HEIGHT, position+((street_width/view_angle)/2), SCREEN_HEIGHT-street_length, colors::BLUE, 1);
    }
  }

  // Draw player's car
  void draw_player(){
    al_draw_bitmap(player->get_texture(), (SCREEN_WIDTH/2)-(player->get_width()/2), SCREEN_HEIGHT-player->get_height(), 0);
    if(environment::debug) {
      al_draw_rectangle((SCREEN_WIDTH/2)-(player->get_width()/2), SCREEN_HEIGHT-player->get_height(), (SCREEN_WIDTH/2)+(player->get_width()/2), SCREEN_HEIGHT, colors::BLUE, 1);
      al_draw_filled_circle(SCREEN_WIDTH/2, SCREEN_HEIGHT-(player->get_height()/2), 1, colors::BLUE);
      al_draw_line(player->get_screen_x()-(player->get_width()/2), 0, player->get_screen_x()-(player->get_width()/2), SCREEN_HEIGHT, colors::BLUE, 1);
      al_draw_line(player->get_screen_x()+(player->get_width()/2), 0, player->get_screen_x()+(player->get_width()/2), SCREEN_HEIGHT, colors::BLUE, 1);
      al_draw_line(0, SCREEN_HEIGHT-(player->get_height()/2), SCREEN_WIDTH, SCREEN_HEIGHT-(player->get_height()/2), colors::BLUE, 1);
    }
  }

  // Draw oponent's car
  void draw_oponent(Car* oponent){
    float distance = distance_from_bottom(oponent);
    // Perspective rate
    float delta = utils::get_delta(street_width, street_width/view_angle, street_length, distance);
    oponent->set_apparent_width(oponent->get_width() * delta);
    oponent->set_apparent_height(oponent->get_height() * delta);
    oponent->set_screen_x(position + oponent->get_x());
    oponent->set_screen_y(SCREEN_HEIGHT-distance-(oponent->get_height()/2));
    float height_variation = oponent->get_height()-oponent->get_apparent_height();
    ALLEGRO_FONT *name_font = fonts::font(PIXEL_FONT, 14*delta);
    display::draw_text(name_font, colors::WHITE, position+(oponent->get_x()*delta), SCREEN_HEIGHT-distance-(20*delta)-oponent->get_height()+height_variation, ALLEGRO_ALIGN_CENTRE, oponent->get_name(), false);
    al_destroy_font(name_font);
    al_draw_scaled_bitmap(oponent->get_texture(), 0, 0, oponent->get_width(), oponent->get_height(), position+(oponent->get_x()*delta)-(oponent->get_apparent_width()/2), SCREEN_HEIGHT-distance-oponent->get_apparent_height(), oponent->get_apparent_width(), oponent->get_apparent_height(), 0);
  }

  // Draw a static object to the screen
  void draw_object(int i){
    Object* object = objects[i];
    float distance = object_distance(i);
    // Perspective rate
    float delta = utils::get_delta(street_width, street_width/view_angle, street_length, distance);
    object->set_apparent_width(object->get_width()*delta);
    object->set_apparent_height(object->get_height()*delta);
    object->set_screen_x(position+(object->get_x()*delta));
    object->set_screen_y(SCREEN_HEIGHT-distance-(object->get_apparent_height()/2));
    al_draw_scaled_bitmap(object->get_texture(), 0, 0, object->get_width(), object->get_height(), object->get_screen_x()-(object->get_apparent_width()/2), object->get_screen_y()-(object->get_apparent_height()/2), object->get_apparent_width(), object->get_apparent_height(), 0);
  }

  // Draw screen info
  void draw_hud(){
    float minimap_heigth = track_length/300;
    float player_minimap_position = utils::min(player->get_y(), track_length)/300;
    int gear_progress = utils::min(10, player->get_gear_progress());

    // Minimap
    al_draw_line(30, SCREEN_HEIGHT-250, 30, (SCREEN_HEIGHT-250)-minimap_heigth, colors::YELLOW, 6);
    al_draw_filled_circle(30, SCREEN_HEIGHT-250, 9, colors::ORANGE);
    al_draw_filled_circle(30, (SCREEN_HEIGHT-250)-minimap_heigth, 9, colors::ORANGE);
    for (int i = 0; i < oponent_count; i++) {
      al_draw_filled_circle(30, (SCREEN_HEIGHT-250)-(utils::min(oponents[i]->get_y(), track_length)/300), 9, colors::RED);
    }
    al_draw_filled_circle(30, (SCREEN_HEIGHT-250)-player_minimap_position, 9, colors::BLUE);

    // Position
    al_draw_text(fonts::DISKUN_60, colors::YELLOW, 30, SCREEN_HEIGHT-160, ALLEGRO_ALIGN_LEFT, "POSITION");
    al_draw_textf(fonts::DISKUN_100, colors::YELLOW, 30, SCREEN_HEIGHT-100, ALLEGRO_ALIGN_LEFT, "%dth", placement);

    // Gears
    al_draw_text(fonts::DISKUN_60, colors::YELLOW, SCREEN_WIDTH-30, SCREEN_HEIGHT-280, ALLEGRO_ALIGN_RIGHT, "GEAR");
    al_draw_textf(fonts::DISKUN_80, colors::YELLOW, SCREEN_WIDTH-30, SCREEN_HEIGHT-220, ALLEGRO_ALIGN_RIGHT, "%d", player->get_gear());
    al_draw_filled_rounded_rectangle(15, 15, 258, 85, 5.0, 5.0, colors::YELLOW);
    al_draw_filled_rounded_rectangle(20, 20, 253, 80, 3.0, 3.0, colors::rgb(50, 50, 50));
    for (int i = 1; i <= gear_progress; i++) {
      int red = (int)(200*(i/10.0));
      int green = 65+(int)(190*((10-i)/10.0));
      al_draw_filled_rectangle(i*23, 23, (i*23)+20, 77, colors::rgb(red, green, 40));
    }

    // Speed
    al_draw_text(fonts::DISKUN_60, colors::YELLOW, SCREEN_WIDTH-30, SCREEN_HEIGHT-140, ALLEGRO_ALIGN_RIGHT, "SPEED");
    al_draw_textf(fonts::DISKUN_80, colors::YELLOW, SCREEN_WIDTH-30, SCREEN_HEIGHT-80, ALLEGRO_ALIGN_RIGHT, "%.0f Km/h", utils::max(0, player->get_speed()));

    // Nitrox
    al_draw_text(fonts::DISKUN_60, colors::YELLOW, SCREEN_WIDTH-30, 20, ALLEGRO_ALIGN_RIGHT, "NITROX");
    al_draw_filled_rounded_rectangle(SCREEN_WIDTH-60, 90, SCREEN_WIDTH-30, 300, 5.0, 5.0, colors::YELLOW);
    al_draw_filled_rounded_rectangle(SCREEN_WIDTH-55, 95, SCREEN_WIDTH-35, 295, 3.0, 3.0, colors::rgb(50, 50, 50));
    al_draw_filled_rectangle(SCREEN_WIDTH-53, 293-(196*(player->get_nitrox()/100)), SCREEN_WIDTH-37, 293, colors::RED);
  }

  // Draw the game cars
  void draw_cars(){
    if(!finished) {
      Car::quick_sort_cars(cars, oponent_count+1);
      placement = 1;
    } 
    for(int i = oponent_count; i >= 0; i--){
      if(cars[i] == player){
        draw_player();
      }
      else{
        if(!finished && cars[i]->get_y() > player->get_y()) {
          placement++;
        }
        if(is_car_visible(cars[i])){
          draw_oponent(cars[i]);
        }
      }
    }
  }

  // Draw static objects
  // TODO: Fix the order in which the objects are rendered (not sure how is this possible)
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
    colors[0] = colors::YELLOW;
    colors[1] = colors::YELLOW;
    colors[2] = colors::YELLOW;
    colors[3] = colors::YELLOW;
    colors[op] = colors::WHITE;
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
    al_draw_filled_rectangle((SCREEN_WIDTH/2)-300, (SCREEN_HEIGHT/2)-200, (SCREEN_WIDTH/2)+300, (SCREEN_HEIGHT/2)+200, colors::BLUE);
    al_draw_rectangle((SCREEN_WIDTH/2)-300, (SCREEN_HEIGHT/2)-200, (SCREEN_WIDTH/2)+300, (SCREEN_HEIGHT/2)+200, colors::YELLOW, 5);
    display::draw_text(fonts::PIXEL_28, colors[0], SCREEN_WIDTH/2, (SCREEN_HEIGHT/2)-71, ALLEGRO_ALIGN_CENTRE, "RESUME", false);
    display::draw_text(fonts::PIXEL_28, colors[1], SCREEN_WIDTH/2, (SCREEN_HEIGHT/2)-33, ALLEGRO_ALIGN_CENTRE, "RESTART", false);
    display::draw_text(fonts::PIXEL_28, colors[2], SCREEN_WIDTH/2, (SCREEN_HEIGHT/2)+5, ALLEGRO_ALIGN_CENTRE, "OPTIONS", false);
    display::draw_text(fonts::PIXEL_28, colors[3], SCREEN_WIDTH/2, (SCREEN_HEIGHT/2)+38, ALLEGRO_ALIGN_CENTRE, "MAIN MENU", true);
    al_flip_display();
  }

  // Set gear to a specifc value if the numeric key corresponding to the gear is pressed
  void control_gears(){
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_1)) {
      player->set_gear(1);
    }
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_2)) {
      player->set_gear(2);
    }
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_3)) {
      player->set_gear(3);
    }
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_4)) {
      player->set_gear(4);
    }
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_5)) {
      player->set_gear(5);
    }
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_6)) {
      player->set_gear(6);
    }
  }

  // Move the player based on input
  void move(){
    float delta_speed = player->speed_increase();
    if(environment::collisions) player->car_collided(cars, objects, oponent_count+1, object_count);
    al_get_keyboard_state(&environment::key_state);
    
    if(is_car_on_track()){
      environment::movement_speed = 22.0;
    }
    else{
      environment::movement_speed = 10.0;
      player->set_speed(utils::max(0, player->get_speed() - GRASS_SLOW_EFFECT));
    }
    
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_A)) {
      if(position < utils::max(SCREEN_WIDTH, street_width)+player->get_width()) position += utils::min(environment::movement_speed*((player->get_speed())/40), environment::movement_speed);
    }
    
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_D)){
      if(position > 0-player->get_width()) position -= utils::min(environment::movement_speed*((player->get_speed())/40), environment::movement_speed);
    }
    
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_W)){
      if(!engine_running){
        player_engine_sound_instance = sounds::continuously_play_sample(sounds::CAR_ENGINE_SOUND);
        engine_running = true;
      }
      sounds::set_sample_volume(player_engine_sound_instance, utils::max(0, player->get_speed()/Car::max_speed(player->get_gear())));
      if(delta_speed < 0){
        player->set_speed(utils::max(0, player->get_speed() + delta_speed));
      }
      else if(player->get_speed() + delta_speed < 0.86 * Car::max_speed(player->get_gear())) {
        player->set_speed(player->get_speed() + delta_speed);
      }
      else {
        player->set_speed(player->get_speed() + (delta_speed/6));
      }
    }
    else {
      if(engine_running && player->get_speed() == 0.0){
        sounds::stop_sample(player_engine_sound_instance);
        engine_running = false;
      }
      if(player->speed_increase() < 0){
        player->set_speed(utils::max(0, player->get_speed() + player->speed_increase()));
      }
      player->set_speed(utils::max(0, player->get_speed() - NO_ACCELERATE_EFFECT));
    }
    
    if(al_key_down(&environment::key_state, ALLEGRO_KEY_LSHIFT)){
      if(player->get_nitrox() > 33.333/environment::fps){
        if(!boosting) {
          sounds::play_sample(sounds::CAR_BOOST_SOUND);
          boosting = true;
        }
        player->set_speed(player->get_speed() + (15.0/environment::fps));
        player->set_nitrox(utils::max(0, player->get_nitrox()-(33.333/environment::fps)));
      }
      else if(boosting){
        boosting = false;
      }
    }
    else{
      if(boosting){
        boosting = false;
      }
      if(player->get_nitrox() < 100.0){
        player->set_nitrox(utils::min(100.0, player->get_nitrox()+(4.0/environment::fps)));
      }
    }
    // Break
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_S)){
      player->set_speed(utils::max(0, player->get_speed() - BREAK_EFFECT));
    }
    player->set_y(player->get_y() + (player->get_speed() * DISTANCE_VARIATION));
  }

  void control_oponents(){
    for (int i = 0; i < oponent_count; i++){
      oponents[i]->control_ia(cars, objects, oponent_count+1, object_count, is_car_visible(oponents[i]));
    }
  }

  // Update game instant
  int update(){
    control_gears();
    al_get_keyboard_state(&environment::key_state);
    move();
    control_oponents();
    for (int i = 0; i < 5; i++) {
      if(object_distance(i) >= objects[i]->get_height()){
        objects[i]->set_y(objects[i]->get_y() + ((5-1)*120) + objects[i]->get_height());
        objects[i+5]->set_y(objects[i+5]->get_y() + ((5-1)*120) + objects[i]->get_height());
      }
    }
    // Update screen
    // al_stop_timer(environment::timer);
    draw_game();
    // al_start_timer(environment::timer);
    if(player->get_y() >= track_length) {
      return 1;
    }
    else return 0;
  }

  // Reinicia a corrida
  void restart(){
    Car::restart_positions(oponents, oponent_count);
    Car::quick_sort_cars(cars, oponent_count+1);
    int player_position = oponent_count+1;
    if(player_position < 4){
      player->set_y(3*STARTING_DISTANCE);
    }
    else if(player_position < 7){
      player->set_y(2*STARTING_DISTANCE);
    }
    else if(player_position < 10){
      player->set_y(1*STARTING_DISTANCE);
    }
    else if(player_position < 13){
      player->set_y(0*STARTING_DISTANCE);
    }
    float x = (player_position%3)-1;
    player->set_x((x*25)+(x*400));
    position = (SCREEN_WIDTH/2)-player->get_x();
    player->set_gear(1);
    player->set_speed(0.0);
    player->set_nitrox(100.0);
    for (int i = 0; i < 5; i++) {
      objects[i]->set_x(-(street_width/2)-120);
      objects[i]->set_y(i*120);
      objects[i+5]->set_x((street_width/2)+120);
      objects[i+5]->set_y(i*120);
    }
    sounds::restart_music(sounds::music);
    countdown();
    al_set_timer_count(environment::timer, 0);
  }

  // Handle paused game
  int pause(){
    int op = 0;
    draw_paused_game(op);
    sounds::set_music_volume(sounds::music, 0.6);
    while (true) {
      ALLEGRO_EVENT ev;
      al_wait_for_event(environment::priority_queue, &ev);
      // Quit game
      if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return 4;
      }
      else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
        // Return to title
        if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
          sounds::play_sample(sounds::MENU_BACK_SOUND);
          break;
        }
        switch (ev.keyboard.keycode) {
          // Selection UP
          case ALLEGRO_KEY_UP:
          case ALLEGRO_KEY_W:
            if(op > 0){
              op--;
              sounds::play_sample(sounds::MENU_MOVE_SOUND);
              // Update screen
              draw_paused_game(op);
            }
            break;
          // Selection DOWN
          case ALLEGRO_KEY_DOWN:
          case ALLEGRO_KEY_S:
            if(op < 3){
              op++;
              sounds::play_sample(sounds::MENU_MOVE_SOUND);
              // Update screen
              draw_paused_game(op);
            }
            break;
          // Confirm selection
          case ALLEGRO_KEY_ENTER:
            sounds::play_sample(sounds::MENU_SELECT_SOUND);
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
                if(menus::options_menu() == 4) return 4;
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
    sounds::play_sample(sounds::MENU_BACK_SOUND);
    sounds::set_music_volume(sounds::music, 1.0);
    return 0;
  }

  int deaccelerate_until_stop(){
    while (true) {
      al_wait_for_event(environment::queue, &ev);
      if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) return 4;

      else if(ev.type == ALLEGRO_EVENT_TIMER) {
        for (int i = 0; i < oponent_count; i++){
          oponents[i]->control_ia(cars, objects, oponent_count+1, object_count, is_car_visible(oponents[i]));
        }
        player->set_speed(utils::max(0, player->get_speed()-(800/(environment::fps*3))));
        player->set_y(player->get_y() + player->get_speed() * DISTANCE_VARIATION);

        // Stop environment::timer to avoid flooding the event environment::queue
        al_stop_timer(environment::timer);
        draw_game();
        al_start_timer(environment::timer);

        sounds::set_sample_volume(player_engine_sound_instance, player->get_speed()/Car::max_speed(player->get_max_gear()));
        if(engine_running && player->get_speed() <= 0.0){
          sounds::stop_sample(player_engine_sound_instance);
          engine_running = false;
        }
        if(player->get_speed() <= 0){
          al_rest(3);
          al_flush_event_queue(environment::queue);
          return 0;
        }
      }
    }
    return 0;
  }

  // Show match leaderboards
  int show_leaderboard(){
    float landscape_position = 0.0;
    bool going_right = true;
    int race_time_minutes = (int)(race_time/60);
    sounds::stop_music(sounds::music);
    if(placement <= 3) sounds::music = sounds::set_music(QUALIFIED_MUSIC);
    else sounds::music = sounds::set_music(DISQUALIFIED_MUSIC);
    sounds::start_music(sounds::music, false);
    display::clear_display(colors::BLUE, false);
    if(placement <= 3){
      display::draw_text(fonts::PIXEL_32, colors::YELLOW, SCREEN_WIDTH/2, 25, ALLEGRO_ALIGN_CENTER, "CONGRATULATIONS", false);
      al_draw_textf(fonts::PIXEL_32, colors::YELLOW, SCREEN_WIDTH/2, 67, ALLEGRO_ALIGN_CENTER, "YOU FINISHED %dth", placement);
    }
    else display::draw_text(fonts::PIXEL_32, colors::YELLOW, SCREEN_WIDTH/2, 25, ALLEGRO_ALIGN_CENTER, "YOU DID NOT QUALIFIED", false);
    display::draw_text(fonts::PIXEL_22, colors::ORANGE, SCREEN_WIDTH/2, 150, ALLEGRO_ALIGN_CENTER, "Press enter to continue", false);
    while (true) {
      al_wait_for_event(environment::queue, &ev);
      // Continue
      if(ev.type == ALLEGRO_EVENT_KEY_UP){
        if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER || ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) break;
      }
      // Update screen
      else if(ev.type == ALLEGRO_EVENT_TIMER){
        al_draw_bitmap(map_landscape, (SCREEN_WIDTH/2)-1500+landscape_position, SCREEN_HEIGHT-250, 0);
        if(going_right && (SCREEN_WIDTH/2)-1500+landscape_position >= 0) going_right = false;
        else if(!going_right && (SCREEN_WIDTH/2)-1500+landscape_position <= -3000+SCREEN_WIDTH) going_right = true;
        if(going_right) landscape_position += 15.0/60.0;
        else landscape_position -= 15.0/60.0;
        al_flip_display();
      }
      // Quit game
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) return 4;
    }
    display::clear_display(colors::BLUE, false);
    al_draw_textf(fonts::PIXEL_28, colors::ORANGE, 30, 10, ALLEGRO_ALIGN_LEFT, "RACE DURATION: %d utils::min %.0f sec", race_time_minutes, race_time-(race_time_minutes*60));
    if(best_time) al_draw_text(fonts::PIXEL_28, colors::YELLOW, SCREEN_WIDTH-30, 10, ALLEGRO_ALIGN_RIGHT, "NEW BEST TIME!");
    for (int i = 0; i < oponent_count+1; i++) {
      if(i%2 == 1) al_draw_textf(fonts::PIXEL_28, colors::YELLOW, (SCREEN_WIDTH/2)+30, ((i)*28)+38, ALLEGRO_ALIGN_LEFT, "%d: %s", i+1, cars[oponent_count-i]->get_name());
      else al_draw_textf(fonts::PIXEL_28, colors::YELLOW, 30, ((i+1)*28)+38, ALLEGRO_ALIGN_LEFT, "%d: %s", i+1, cars[oponent_count-i]->get_name());
    }
    al_draw_bitmap(map_landscape, (SCREEN_WIDTH/2)-1500+landscape_position, SCREEN_HEIGHT-250, 0);
    al_flip_display();
    while (true) {
      al_wait_for_event(environment::queue, &ev);
      // Continue
      if(ev.type == ALLEGRO_EVENT_KEY_UP){
        if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER || ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) break;
      }
      // Update screen
      else if(ev.type == ALLEGRO_EVENT_TIMER){
        al_draw_bitmap(map_landscape, (SCREEN_WIDTH/2)-1500+landscape_position, SCREEN_HEIGHT-250, 0);
        if(going_right && (SCREEN_WIDTH/2)-1500+landscape_position >= 0) going_right = false;
        else if(!going_right && (SCREEN_WIDTH/2)-1500+landscape_position <= -3000+SCREEN_WIDTH) going_right = true;
        if(going_right) landscape_position += 15.0/60.0;
        else landscape_position -= 15.0/60.0;
        al_flip_display();
      }
      // Quit game
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) return 4;
    }
    return 0;
  }

  // Setup game environment
  void setup(ALLEGRO_BITMAP* player_texture, Car** tournament_cars, bool single_match){
    if(access(SAVE_FILE, F_OK) == 0){
      FILE *save = fopen(SAVE_FILE, "r");
      fscanf(save, "%d", &record);
      fclose(save);
    }
    else{
      record = 0;
      FILE *save = fopen(SAVE_FILE, "w");
      fprintf(save, "%d", record);
      fclose(save);
    }
    // Initialize environment
    int player_position = oponent_count+1;
    street_left_limit = (SCREEN_WIDTH-street_width)/2;
    object_count = 30;
    objects = (Object**) malloc(object_count*sizeof(Object*));
    cars = (Car**) malloc((oponent_count+1)*sizeof(Car*));

    // Initialize objects
    objects[object_count-1] = new Object(0, track_length, display::FINISH_LINE, false);
    for (int i = 0; i < 5; i++) {
      objects[i] = new Object(-(street_width/2)-120, i*120, display::ROAD_SIGN, false);
      objects[i+5] = new Object((street_width/2)+120, i*120, display::ROAD_SIGN, false);
    }
    for (int i = 10; i < object_count-1; i++) {
      float x = (rand()%(int)(street_width-30))-((street_width-30)/2), y = (rand()%(int)(track_length-(track_length/4)+1))+(track_length/4);
      objects[i] = Object::get_random_obstacle(x, y);
    }

    // Initialize oponents
    if(tournament_cars){
      oponents = tournament_cars;
    }
    else{
      oponents = (Car**) malloc(oponent_count*sizeof(Car*));
      int car_type, car_color;
      for (int i = 0; i < oponent_count; i++) {
        car_type = rand()%4;
        car_color = rand()%7;
        oponents[i] = new Car(i+1, display::get_car((CarsTypes)car_type, car_color));
      }
    }
    for (int i = 0; i < oponent_count; i++) {
      cars[i] = oponents[i];
    }

    // Change track to Las Vegas theme
    sounds::stop_music(sounds::music);
    map_music_title = maps::get_map_music_title((Map)map);
    sounds::music = maps::get_map_music((Map)map);
    sounds::start_music(sounds::music, true);

    // Initialize Landscape
    map_landscape = maps::get_map_landscape((Map)map);
    map_soil = maps::get_map_color((Map)map);

    // Initialize player
    player = new Car(player_texture);
    cars[oponent_count] = player;
    Car::quick_sort_cars(cars, oponent_count+1);
    if(player_position < 4){
      player->set_y(3*STARTING_DISTANCE);
    }
    else if(player_position < 7){
      player->set_y(2*STARTING_DISTANCE);
    }
    else if(player_position < 10){
      player->set_y(1*STARTING_DISTANCE);
    }
    else if(player_position < 13){
      player->set_y(0*STARTING_DISTANCE);
    }
    float x = (player_position%3)-1;
    player->set_x((x*25)+(x*400));
    position = (SCREEN_WIDTH/2)-player->get_x();
  }

  int handle_key_events(){
    int result;
    // Gear up
    if(ev.keyboard.keycode == ALLEGRO_KEY_E) player->gear_up();
    // Gear down
    else if(ev.keyboard.keycode == ALLEGRO_KEY_Q) player->gear_down();
    // Reset position
    else if(ev.keyboard.keycode == ALLEGRO_KEY_R){
      int new_position = rand()%(int)(street_width + 1);
      position = (float)new_position;
    }
    // Pause
    if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
      if(engine_running){
        sounds::stop_sample(player_engine_sound_instance);
      }
      al_stop_timer(environment::timer);
      result = pause();
      al_start_timer(environment::timer);
      sounds::set_music_volume(sounds::music, 1.0);
      if(result == -1 || result == 4) return result;
      if(result == 0) {
        if(engine_running){
          player_engine_sound_instance = sounds::continuously_play_sample(sounds::CAR_ENGINE_SOUND);
          sounds::set_sample_volume(player_engine_sound_instance, player->get_speed()/Car::max_speed(player->get_gear()));
        }
        draw_game();
      }
    }
  }

  // Countdown to race start
  void countdown(){
    for (int i = 3; i > 0; i--) {
      draw_game();
      al_draw_textf(fonts::DISKUN_60, colors::YELLOW, SCREEN_WIDTH/2, SCREEN_HEIGHT/3, ALLEGRO_ALIGN_CENTRE, "%d", i);
      al_flip_display();
      sounds::play_sample(sounds::READY_SOUND);
      al_rest(1);
    }
    sounds::play_sample(sounds::GO_SOUND);
    al_flush_event_queue(environment::queue);
    al_flush_event_queue(environment::priority_queue);
    al_set_timer_count(environment::timer, 0);
  }

  // Single match
  int play(ALLEGRO_BITMAP* player_texture, Car** tournament_cars, int oponents_amount, Map choosen_map, bool single_match){
    int result;
    map = choosen_map;
    oponent_count = oponents_amount;
    if(single_match) best_time = false;

    setup(player_texture, tournament_cars, single_match);
    
    countdown();
    // Main loop
    while (true) {
      while(!al_is_event_queue_empty(environment::priority_queue)){
        al_wait_for_event(environment::priority_queue, &ev);
        // Quit game
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
          if(engine_running && player_engine_sound_instance) sounds::stop_sample(player_engine_sound_instance);
          return 4;
        }
        // Handle input
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
          result = handle_key_events();
        }
      }
      al_wait_for_event(environment::queue, &ev);
      // Each 1/environment::fps seconds
      if(ev.type == ALLEGRO_EVENT_TIMER) {
        result = update();
        if(result == -1){
          if(engine_running) sounds::stop_sample(player_engine_sound_instance);
          return -1;
        }
        else if(result == 1){
          finished = true;
          boosting = false;
          race_time = al_get_timer_count(environment::timer)/environment::fps;
          if(al_get_timer_count(environment::timer) < record || record == 0){
            best_time = true;
            record = al_get_timer_count(environment::timer);
            FILE *save = fopen(SAVE_FILE, "w");
            fprintf(save, "%d", record);
            fclose(save);
          }
          deaccelerate_until_stop();
          if(tournament_cars == nullptr) free(oponents);
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
    Car **tournament_cars = (Car**) calloc(oponents_amount, sizeof(Car*));
    int car_type, car_color, op;
    best_time = false;
    for (int i = 0; i < oponents_amount; i++) {
      car_type = (rand()%4)+1;
      car_color = rand()%7;
      tournament_cars[i] = new Car(i+1, display::get_car((CarsTypes)car_type, car_color));
    }
    for (int i = 0; i < 4; i++) {
      op = play(player_texture, tournament_cars, oponents_amount, (Map)i, (i==3));
      clear_game();
      Car::restart_positions(tournament_cars, oponents_amount);
      if(op == 4 || op == -1) return op;
    }
    free(tournament_cars);
    return 0;
  }

  void clear_game(){
    free(cars);
  }

}
