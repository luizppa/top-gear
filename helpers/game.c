#include "game.h"

float position;
float moviment_speed = 25.0;
float street_width = 800.0;
float street_length = 300.0;

void redraw_game(){
  // printf("%.2f\n", position);
  clear_display(BLACK);
  al_draw_line((sw-street_width)/2, sh, (sw-(street_width/3))/2, sh-street_length, BLUE, 1);
  al_draw_line(sw-((sw-street_width)/2), sh, sw-((sw-(street_width/3))/2), sh-street_length, BLUE, 1);
  al_draw_line(0, sh-street_length, sw, sh-street_length, BLUE, 1);
  // al_draw_bitmap(GAME_CAR_BITMAP, position-123, sh-180, 0);
  al_draw_rectangle(position-50.0, sh-100.0, position+50.0, sh-20.0, RED, 1);
  al_flip_display();
}

int update(){
  al_get_keyboard_state(&key_state);
  if (al_key_down(&key_state, ALLEGRO_KEY_A)) {
    if(position-50.0 > 1) position -= moviment_speed;
    redraw_game();
  }
  else if (al_key_down(&key_state, ALLEGRO_KEY_D)){
    if(position+50.0 < sw) position += moviment_speed;
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
