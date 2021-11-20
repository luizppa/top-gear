#include "../include/game.hpp"
#include "../include/display.hpp"
#include "../include/environment.hpp"
#include "../include/utils.hpp"
#include "../include/menus.hpp"
#include "../include/fonts.hpp"
#include "../include/colors.hpp"
#include "../include/sounds.hpp"

#include <unistd.h>
#include <ctime>
#include <vector>
#include <fstream>

namespace top_gear {

  Race::Race(ALLEGRO_BITMAP* player_texture, Map map, std::vector<Car*> tournament_cars){
    this->renderer = GameRenderer(this);
    this->map = map;
    this->is_tournament = true;
    this->is_single_race = !this->is_tournament;
    this->oponent_count = tournament_cars.size();
    this->setup(player_texture, tournament_cars);
  }

  Race::Race(ALLEGRO_BITMAP* player_texture, Map map, int oponents_amount){
    this->renderer = GameRenderer(this);
    this->map = map;
    this->is_tournament = false;
    this->is_single_race = !this->is_tournament;
    this->oponent_count = oponents_amount;
    this->setup(player_texture, std::vector<Car*>(oponents_amount));
  }

  void Race::sort(){
    Car::sort(this->oponents);
  }

  /**
   * Load the stored best time from disk
  */
  int Race::load_best_time(){
    int best_time = 0;
    std::fstream save_file;
    if(access(SAVE_FILE, F_OK) == 0){
      save_file.open(SAVE_FILE, std::fstream::ios_base::in);
      save_file >> best_time;
    }
    else{
      save_file.open(SAVE_FILE, std::fstream::ios_base::out);
      save_file << best_time;
    }
    save_file.close();
    return best_time;
  }

  /**
   *  Initialize obstacles 
  */
  void Race::create_obstacles(){
    this->obstacles = std::vector<Obstacle*>(OBSTACLE_COUNT);
    this->obstacles[OBSTACLE_COUNT-1] = new Obstacle(0, TRACK_LENGTH, display::FINISH_LINE, false);
    
    for (int i = 0; i < 5; i++) {
      this->obstacles[i] = new Obstacle(-(STREET_WIDTH/2)-120, i*120, display::ROAD_SIGN, false);
      this->obstacles[i+5] = new Obstacle((STREET_WIDTH/2)+120, i*120, display::ROAD_SIGN, false);
    }
    
    for (int i = 10; i < OBSTACLE_COUNT-1; i++) {
      float x = (rand()%(int)(STREET_WIDTH-40))-((STREET_WIDTH-40)/2);
      float y = (rand()%(int)(TRACK_LENGTH-(TRACK_LENGTH/4)+1))+(TRACK_LENGTH/4);
      this->obstacles[i] = Obstacle::get_random_obstacle(x, y);
    }
  }

  /**
   * Initialize oponents
   * @param tournament_cars vector of oponent cars
  */
  void Race::create_oponents(std::vector<Car*> tournament_cars){
    this->oponents = tournament_cars;
    if(this->is_single_race){
      int car_type, car_color;
      for (int i = 0; i < oponent_count; i++) {
        car_type = rand()%4;
        car_color = rand()%7;
        oponents[i] = new Car(i+1, display::get_car_bitmap((CarsTypes)car_type, car_color));
      }
    }
    // cars = (Car**) malloc((oponent_count+1)*sizeof(Car*));
    // for (int i = 0; i < oponent_count; i++) {
    //   cars[i] = oponents[i];
    // }
  }

  /**
   * Change track to map's theme
  */
  void Race::setup_music(){
    if(this->map != Map::NONE){
      sounds::stop_music(sounds::music);
      this->map_music_title = maps::get_map_song_title(this->map);
      sounds::music = maps::get_map_music(this->map);
      sounds::start_music(sounds::music, true);
    }
  }

  /**
   * Initialize map's scenario
  */
  void Race::setup_scenario(){
    if(this->map != Map::NONE){
      this->map_landscape = maps::get_map_landscape((Map)map);
      this->map_soil = maps::get_map_color((Map)map);
    }
  }

  void Race::restart(){
    Car::restart_positions(this->oponents);
    this->sort();

    int placement = this->oponents.size() + 1; 
    player->set_y(this->get_player_start_distance(placement));
    player->set_x(this->get_player_start_position(placement));

    this->camera_position = utils::normalize(-player->get_x());
    player->set_gear(1);
    player->set_speed(0.0);
    player->set_nitrox(100.0);

    for (int i = 0; i < 5; i++) {
      this->obstacles[i]->set_x(-(STREET_WIDTH / 2) - 120);
      this->obstacles[i]->set_y(i * 120);
      this->obstacles[i+5]->set_x((STREET_WIDTH / 2) + 120);
      this->obstacles[i+5]->set_y(i * 120);
    }

    sounds::restart_music(sounds::music);
    countdown();
    al_set_timer_count(environment::timer, 0);
  }

  // TODO: Make logic generic
  /**
   * Returns the initial y coordinate for the player
  */
  float Race::get_player_start_distance(int placement){
    if(placement < 4){
      return (3*STARTING_DISTANCE);
    }
    else if(placement < 7){
      return (2*STARTING_DISTANCE);
    }
    else if(placement < 10){
      return (1*STARTING_DISTANCE);
    }
    else if(placement < 13){
      return (0*STARTING_DISTANCE);
    }
    else {
      return 0;
    }
  }

  /**
   * Returns the initial x coordinate for the player
  */
  float Race::get_player_start_position(int placement){
    float x = (placement % 3) - 1;
    return ((x*25)+(x*400));
  }

  void Race::setup(ALLEGRO_BITMAP* player_texture, std::vector<Car*> tournament_cars){
    this->best_time = Race::load_best_time();
    this->create_obstacles();
    this->create_oponents(tournament_cars);
    this->setup_music();
    this->setup_scenario();

    this->player = new Car(player_texture);
    this->player->set_y(Race::get_player_start_distance(oponent_count + 1));
    this->player->set_x(Race::get_player_start_position(oponent_count + 1));

    // cars[oponent_count] = player;
    // Car::quick_sort_cars(cars, oponent_count+1);
    
    this->camera_position = utils::normalize(-player->get_x());
  }

  /**
   * Displays countdown to race start
  */
  void Race::countdown(){
    for (int i = 3; i > 0; i--) {
      renderer.draw_game();
      al_draw_textf(fonts::DISKUN_60, colors::YELLOW, SCREEN_WIDTH/2, SCREEN_HEIGHT/3, ALLEGRO_ALIGN_CENTRE, "%d", i);
      al_flip_display();
      sounds::play_sample(sounds::READY_SOUND);
      al_rest(1);
    }
    sounds::play_sample(sounds::GO_SOUND);
    al_flush_event_queue(environment::event_queue);
    al_flush_event_queue(environment::input_event_queue);
    al_flush_event_queue(environment::timer_event_queue);
    al_set_timer_count(environment::timer, 0);
  }

  InputResult Race::handle_key_events(ALLEGRO_EVENT ev){
    int result;

    switch (ev.keyboard.keycode) {
      case ALLEGRO_KEY_E:
        this->player->gear_up();
        return InputResult::CONTINUE;

      case ALLEGRO_KEY_Q:
        this->player->gear_up();
        return InputResult::CONTINUE;

      case ALLEGRO_KEY_R:
        int new_position = rand()%(int)(STREET_WIDTH + 1);
        this->player->set_x((float)new_position);
        return InputResult::CONTINUE;

      case ALLEGRO_KEY_ESCAPE:
        return InputResult::PAUSE;
      
      default:
        return InputResult::CONTINUE;
    }
  }

  int Race::perform_pause_action(int op){
    int result;

    switch (op) {
      case 0: return MENU_ACTION_RESUME;

      case 1:
        this->restart();
        return MENU_ACTION_RESUME;

      case 2:
        result = menus::options_menu();
        if(result == MENU_ACTION_QUIT_GAME) return MENU_ACTION_QUIT_GAME;
        op = 0;
        return MENU_ACTION_DONE;

      case 3:
        return MENU_ACTION_GO_BACK;
    }
  }

  int Race::pause_menu_loop(){
    int op = 0, result;
    ALLEGRO_EVENT ev;

    renderer.draw_paused_game(op);
    sounds::set_music_volume(sounds::music, 0.6);

    while (true) {
      al_wait_for_event(environment::input_event_queue, &ev);

      switch(ev.type){
        case ALLEGRO_EVENT_DISPLAY_CLOSE: return MENU_ACTION_QUIT_GAME;

        case ALLEGRO_EVENT_KEY_DOWN:
          switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
              sounds::play_sample(sounds::MENU_BACK_SOUND);
              return MENU_ACTION_RESUME;

            case ALLEGRO_KEY_UP:
            case ALLEGRO_KEY_W:
              if(op > 0){
                op--;
                sounds::play_sample(sounds::MENU_MOVE_SOUND);
                renderer.draw_paused_game(op);
              }
              break;

            case ALLEGRO_KEY_DOWN:
            case ALLEGRO_KEY_S:
              if(op < 3){
                op++;
                sounds::play_sample(sounds::MENU_MOVE_SOUND);
                renderer.draw_paused_game(op);
              }
              break;

            case ALLEGRO_KEY_ENTER:
              sounds::play_sample(sounds::MENU_SELECT_SOUND);
              result = this->perform_pause_action(op);
              if(result == MENU_ACTION_DONE){
                renderer.draw_paused_game(op);
              }
              else{
                return result;
              }
              break;
          }
          break;
      }
    }
    sounds::play_sample(sounds::MENU_BACK_SOUND);
    sounds::set_music_volume(sounds::music, 1.0);
    return MENU_ACTION_RESUME;
  }

  int Race::pause_menu(){
    int result;

    player->stop_engine_sound();
    al_stop_timer(environment::timer);
    result = this->pause_menu_loop();
    al_start_timer(environment::timer);
    sounds::set_music_volume(sounds::music, 1.0);

    if(result == MENU_ACTION_RESUME) {
      player->stop_engine_sound();
      renderer.draw_game();
    }
    else {
      return result;
    }
  }

  /**
   * @brief Set gear to a specifc value if the numeric key corresponding to the gear is pressed
   * 
   */
  void Race::control_gears(){
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_1)) {
      this->player->set_gear(1);
    }
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_2)) {
      this->player->set_gear(2);
    }
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_3)) {
      this->player->set_gear(3);
    }
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_4)) {
      this->player->set_gear(4);
    }
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_5)) {
      this->player->set_gear(5);
    }
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_6)) {
      this->player->set_gear(6);
    }
  }

  /**
   * @brief Move the player based on input
   * 
   */
  void Race::move(){
    float delta_speed = player->speed_increase();
    if(environment::collisions) player->car_collided(this->oponents, this->obstacles);
    al_get_keyboard_state(&environment::key_state);
    
    if(is_car_on_track()){
      environment::movement_speed = 22.0;
    }
    else{
      environment::movement_speed = 10.0;
      player->set_speed(utils::max(0, player->get_speed() - GRASS_SLOW_EFFECT));
    }
    
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_A) && camera_position < utils::max(SCREEN_WIDTH, STREET_WIDTH)+player->get_width()) {
      camera_position += utils::min(environment::movement_speed*((player->get_speed())/40), environment::movement_speed);
    }
    
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_D) && camera_position > -player->get_width()){
      camera_position -= utils::min(environment::movement_speed*((player->get_speed())/40), environment::movement_speed);
    }
    
    if (al_key_down(&environment::key_state, ALLEGRO_KEY_W)){
      if(!player->is_engine_running()){
        player->set_engine_running(true);
        player->play_engine_sound();
      }
      sounds::set_sample_volume(player->get_engine_sound_instance(), utils::max(0, player->get_speed()/Car::max_speed(player->get_gear())));
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
      if(player->is_engine_running() && player->get_speed() == 0.0){
        player->set_engine_running(false);
        player->stop_engine_sound();
      }
      if(player->speed_increase() < 0){
        player->set_speed(utils::max(0, player->get_speed() + player->speed_increase()));
      }
      player->set_speed(utils::max(0, player->get_speed() - NO_ACCELERATE_EFFECT));
    }
    
    if(al_key_down(&environment::key_state, ALLEGRO_KEY_LSHIFT)){
      if(player->get_nitrox() > 33.333/environment::fps){
        if(!player->is_boosting()) {
          sounds::play_sample(sounds::CAR_BOOST_SOUND);
          player->set_boosting(true);
        }
        player->set_speed(player->get_speed() + (15.0/environment::fps));
        player->set_nitrox(utils::max(0, player->get_nitrox()-(33.333/environment::fps)));
      }
      else if(player->is_boosting()){
        player->set_boosting(false);
      }
    }
    else{
      if(player->is_boosting()){
        player->set_boosting(false);
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

  /**
   * @brief Update game instant
   * 
   * @return int 
   */
  int Race::update(){
    this->control_gears();
    this->move();
    control_oponents();
    move_scenario_objects();
    draw_game();
    if(player->get_y() >= track_length) {
      return 1;
    }
    else return 0;
  }

  int Race::play(){
    int result;
    ALLEGRO_EVENT ev;
    
    this->countdown();

    while (true) {
      while(!al_is_event_queue_empty(environment::input_event_queue)){
        al_wait_for_event(environment::input_event_queue, &ev);
        
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
          if(player->is_engine_running() && player->get_engine_sound_instance() != nullptr){
            sounds::stop_sample(player->get_engine_sound_instance());
          }
          return MENU_ACTION_QUIT_GAME;
        }
        
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
          result = handle_key_events(ev);
          if(result == InputResult::PAUSE){
            result = this->pause_menu();
            if(result != MENU_ACTION_RESUME){
              return result;
            }
          }
        }
      }

      al_wait_for_event(environment::event_queue, &ev);
      if(ev.type == ALLEGRO_EVENT_TIMER) {
        result = update();
        if(result == -1){
          if(engine_running) sounds::stop_sample(player->get_engine_sound_instance());
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

  // bool finished = false;
  // bool engine_running = false;
  // bool boosting = false;
  // bool best_time = false;
  // int map; // The map code
  // int placement = 1; // Player race position
  // int object_count = 0; // Number of static objects on the map
  // float position; // The road center x coordinate on the screen
  // float street_width = STREET_WIDTH;
  // float track_visible_length = 500.0; // Street visible s pam
  // float view_angle = 16.0; // Perspective angle (not related to any real wolrd angle value)
  // float street_left_limit; // Street border
  // float track_length = 50000.0;
  // float paralax = 1.3;
  // double race_time;
  // char* map_music_title;
  // ALLEGRO_EVENT ev;
  // Car* player;
  // Car** oponents;
  // Car** cars;
  // Obstacle** objects;
  // ALLEGRO_COLOR map_soil;
  // ALLEGRO_BITMAP* map_landscape = nullptr;
  // ALLEGRO_SAMPLE_INSTANCE *player_engine_sound_instance = nullptr;

  void countdown();
  void clear_game();

  // Verify it the car went off the road
  bool is_car_on_track(){
    return (position-(player->get_width()/2) >= street_left_limit && position+(player->get_width()/2) <= SCREEN_WIDTH-street_left_limit);
  }

  // Process AI commands for each oponent
  void control_oponents(){
    for (int i = 0; i < oponent_count; i++){
      oponents[i]->control_ia(cars, objects, oponent_count+1, object_count, is_car_visible(oponents[i]));
    }
  }

  // Move visible scenario objects
  void move_scenario_objects(){
    for (int i = 0; i < 5; i++) {
      if(object_distance(i) < -objects[i]->get_height()){
        objects[i]->set_y(objects[i]->get_y() + ((5-1)*120) + objects[i]->get_height());
        objects[i+5]->set_y(objects[i+5]->get_y() + ((5-1)*120) + objects[i]->get_height());
      }
    }
  }

  int deaccelerate_until_stop(){
    while (true) {
      al_wait_for_event(environment::event_queue, &ev);
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
          al_flush_event_queue(environment::event_queue);
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
      al_wait_for_event(environment::event_queue, &ev);
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
      al_wait_for_event(environment::event_queue, &ev);
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

  // Single match
  

  int tournament(ALLEGRO_BITMAP* player_texture, int oponents_amount){
    Car **tournament_cars = (Car**) calloc(oponents_amount, sizeof(Car*));
    int car_type, car_color, op;
    best_time = false;
    for (int i = 0; i < oponents_amount; i++) {
      car_type = (rand()%4)+1;
      car_color = rand()%7;
      tournament_cars[i] = new Car(i+1, display::get_car_bitmap((CarsTypes)car_type, car_color));
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
