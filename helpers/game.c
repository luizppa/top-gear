#include "game.h"

float position;
float moviment_speed = 22.0;
float street_width = 1300.0;
float street_length = 500.0;
float view_angle = 16.0;
float car_width = 166.0;
float car_height = 120.0;

float max(float a, float b){
  if(a > b) return a;
  return b;
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
  al_draw_line(0, sh-street_length, sw, sh-street_length, BLUE, 1);
}

void draw_car(){
  // al_draw_filled_circle(sw/2, sh-60, 1, RED);
  al_draw_bitmap(GAME_CAR_BITMAP, (sw/2)-(car_width/2), (sh)-(20+car_height), 0);
  al_draw_rectangle((sw/2)-(car_width/2), sh-(car_height+20), (sw/2)+(car_width/2), sh-20.0, RED, 1);
}

void redraw_game(){
  al_clear_to_color(GREY);
  draw_track();
  draw_car();
  al_flip_display();
}

bool is_car_on_track(){
  return (position-(car_width/2) >= (sw-street_width)/2 && position+(car_width/2) <= sw-((sw-street_width)/2));
}

int update(){
  al_get_keyboard_state(&key_state);
  if(is_car_on_track()){
    moviment_speed = 22.0;
  }
  else moviment_speed = 10.0;
  if (al_key_down(&key_state, ALLEGRO_KEY_A)) {
    if(position < max(sw, street_width)+car_width) position += moviment_speed;
    redraw_game();
  }
  else if (al_key_down(&key_state, ALLEGRO_KEY_D)){
    if(position > 0-car_width) position -= moviment_speed;
    redraw_game();
  }
  return 0;
}

int play(){
  position = (sw/2);
  stop_music(music);
  music = set_music(LAS_VEGAS_MUSIC);
  start_music(music, true);
  redraw_game();
  while (true) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(queue, &ev);
    // Return to menu
    if(ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) return -1;
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
