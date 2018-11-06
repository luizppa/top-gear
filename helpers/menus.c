#include "menus.h"
#include "game.h"
#include "sounds.h"
#include "environment.h"

ALLEGRO_COLOR colors[7];

// Updates menu on screen
int redraw_main_menu(int op){
  clear_display(BLUE, false);
  al_draw_bitmap(GAME_TITLE, (sw/2)-203, (sh/2)-250, 0);
  draw_text(PIXEL_FONT, 28, colors[0], sw/2, (sh/2)-35, ALLEGRO_ALIGN_CENTRE, "PLAY", false);
  draw_text(PIXEL_FONT, 28, colors[1], sw/2, (sh/2)+5, ALLEGRO_ALIGN_CENTRE, "OPTIONS", false);
  draw_text(PIXEL_FONT, 28, colors[2], sw/2, (sh/2)+45, ALLEGRO_ALIGN_CENTRE, "CREDITS", false);
  draw_text(PIXEL_FONT, 28, colors[3], sw/2, (sh/2)+85, ALLEGRO_ALIGN_CENTRE, "EXIT", true);
}

int mode_selection();
int* car_selection();
int color_selection();

// Runs main menu
int main_menu(){
  int op = 1;
  // Selected option = WHITE
  colors[0] = WHITE;
  colors[1] = YELLOW;
  colors[2] = YELLOW;
  colors[3] = YELLOW;
  colors[4] = YELLOW;
  // Update screen
  redraw_main_menu(op);
  while (true) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(queue, &ev);
    // Quit game
    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      return 4;
    }
    else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      // Return to title
      if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) return -1;
      switch (ev.keyboard.keycode) {
        // Selection UP
        case ALLEGRO_KEY_UP:
        case ALLEGRO_KEY_W:
          if(op > 1){
            colors[op-1] = YELLOW;
            op--;
            colors[op-1] = WHITE;
            // Update screen
            redraw_main_menu(op);
          }
          break;
        // Selection DOWN
        case ALLEGRO_KEY_DOWN:
        case ALLEGRO_KEY_S:
          if(op < 4){
            colors[op-1] = YELLOW;
            op++;
            colors[op-1] = WHITE;
            // Update screen
            redraw_main_menu(op);
          }
          break;
        // Confirm selection
        case ALLEGRO_KEY_ENTER:
          switch (op) {
            // Select game mode
            case 1:
              op = mode_selection();
              break;
          }
          if(op != -1) return op;
          op = 1;
          colors[0] = WHITE;
          colors[1] = YELLOW;
          colors[2] = YELLOW;
          colors[3] = YELLOW;
          redraw_main_menu(op);
      }
    }
  }
  draw_title();
  return 0;
}

// Update mode selection screen
void redraw_mode_selection(int op){
  float square_side = 250.0;
  clear_display(BLUE, false);
  draw_text(PIXEL_FONT, 32, YELLOW, (sw/2), 35, ALLEGRO_ALIGN_CENTRE, "GAME MODE", false);
  al_draw_rounded_rectangle((sw/2)-25-square_side, (sh/2)-(square_side/2), (sw/2)-25, (sh/2)+(square_side/2), 0, 0, colors[0], 5);
  draw_text(PIXEL_FONT, 28, colors[0], (sw/2)-25-(square_side/2), (sh/2)-14, ALLEGRO_ALIGN_CENTRE, "QUICK PLAY", false);
  al_draw_rounded_rectangle((sw/2)+25, (sh/2)-(square_side/2), (sw/2)+25+square_side, (sh/2)+(square_side/2), 0, 0, colors[1], 5);
  draw_text(PIXEL_FONT, 28, colors[1], (sw/2)+25+(square_side/2), (sh/2)-14, ALLEGRO_ALIGN_CENTRE, "TOURNAMENT", false);
  switch (op) {
    case 1:
      draw_text(PIXEL_FONT, 22, WHITE, 30, sh-37, ALLEGRO_ALIGN_LEFT, "A single race to test your skills", true);
      break;
    case 2:
      draw_text(PIXEL_FONT, 22, WHITE, 30, sh-37, ALLEGRO_ALIGN_LEFT, "Join a series of races and make your way to the top!", true);
      break;
  }
}

// Select gamemode
int mode_selection(){
  int op = 1, *car;
  colors[0] = WHITE;
  colors[1] = YELLOW;
  redraw_mode_selection(op);
  while (true) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(queue, &ev);
    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      return 4;
    }
    else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) return -1;
      switch (ev.keyboard.keycode) {
        case ALLEGRO_KEY_RIGHT:
        case ALLEGRO_KEY_D:
          if(op < 2){
            colors[op-1] = YELLOW;
            op++;
            colors[op-1] = WHITE;
            redraw_mode_selection(op);
          }
          break;
        case ALLEGRO_KEY_LEFT:
        case ALLEGRO_KEY_A:
          if(op > 1){
            colors[op-1] = YELLOW;
            op--;
            colors[op-1] = WHITE;
            redraw_mode_selection(op);
          }
          break;
        case ALLEGRO_KEY_ENTER:
          switch (op) {
            // Start race
            case 1:
              car = car_selection();
              if(car[0] == 4) return 4;
              if(car[0] == -1) return -1;
              op = play(get_car(car[0], car[1]), NULL, 11);
              stop_music(music);
              music = set_music(TITLE_MUSIC);
              start_music(music, true);
              break;
          }
          return op;
      }
    }
  }
  return 0;
}

// Update car selection screen
void redraw_car_selection(int op){
  float square_width = 340.0, square_height = 220;
  float dominus_w = 239.0, dominus_h = 70.0;
  float octane_w = 187.0, octane_h = 88.0;
  float roadhog_w = 233.0, roadhog_h = 116.0;
  clear_display(BLUE, false);
  // Dominus
  al_draw_rectangle((sw/2)-25-square_width, (sh/2)-25-square_height, (sw/2)-25, (sh/2)-25, colors[0], 5);
  draw_text(PIXEL_FONT, 22, colors[0], (sw/2)-25-(square_width/2), (sh/2)-25-square_height+12, ALLEGRO_ALIGN_CENTRE, "Dominus GT", false);
  al_draw_bitmap(DOMINUS_GT_ICON_BITMAP, (sw/2)-25-((square_width-dominus_w)/2)-dominus_w, (sh/2)-25-35-dominus_h, 0);

  // Octane
  al_draw_rectangle((sw/2)+25, (sh/2)-25-square_height, (sw/2)+25+square_width, (sh/2)-25, colors[1], 5);
  draw_text(PIXEL_FONT, 28, colors[1], (sw/2)+25+(square_width/2), (sh/2)-25-square_height+12, ALLEGRO_ALIGN_CENTRE, "Octane ZSR", false);
  al_draw_bitmap(OCTANE_ZSR_ICON_BITMAP, (sw/2)+25+((square_width-octane_w)/2), (sh/2)-25-35-octane_h, 0);

  // Roadhog
  al_draw_rectangle((sw/2)-25-square_width, (sh/2)+25, (sw/2)-25, (sh/2)+25+square_height, colors[2], 5);
  draw_text(PIXEL_FONT, 28, colors[2], (sw/2)-25-(square_width/2), (sh/2)+25+12, ALLEGRO_ALIGN_CENTRE, "Roadhog", false);
  al_draw_bitmap(ROADHOG_ICON_BITMAP, (sw/2)-25-(square_width/2)-(roadhog_w/2), (sh/2)+25+square_height-35-roadhog_h, 0);

  draw_text(PIXEL_FONT, 32, YELLOW, (sw/2), 30, ALLEGRO_ALIGN_CENTRE, "SELECT YOUR VEICHLE", true);
}

// Select car
int* car_selection(){
  int op = 1, *res = (int*) calloc(2, sizeof(int)), color;
  colors[0] = WHITE;
  colors[1] = YELLOW;
  colors[2] = YELLOW;
  redraw_car_selection(op);
  while (true) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(queue, &ev);
    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      res[0] = 4;
      return res;
    }
    else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
        res[0] = -1;
        return res;
      }
      switch (ev.keyboard.keycode) {
        case ALLEGRO_KEY_RIGHT:
        case ALLEGRO_KEY_D:
          if(op < 2){
            colors[op-1] = YELLOW;
            op++;
            colors[op-1] = WHITE;
            redraw_car_selection(op);
          }
          break;
        case ALLEGRO_KEY_DOWN:
        case ALLEGRO_KEY_S:
          if(op < 2){
            colors[op-1] = YELLOW;
            op += 2;
            colors[op-1] = WHITE;
            redraw_car_selection(op);
          }
          break;
        case ALLEGRO_KEY_LEFT:
        case ALLEGRO_KEY_A:
          if(op > 1 && op < 3){
            colors[op-1] = YELLOW;
            op--;
            colors[op-1] = WHITE;
            redraw_car_selection(op);
          }
          break;
        case ALLEGRO_KEY_UP:
        case ALLEGRO_KEY_W:
          if(op > 2){
            colors[op-1] = YELLOW;
            op -= 2;
            colors[op-1] = WHITE;
            redraw_car_selection(op);
          }
          break;
        case ALLEGRO_KEY_ENTER:
          res[0] = op;
          color = color_selection();
          if(color == 7) res[0] = 4;
          if(color == -1) res[0] = -1;
          res[1] = color;
          return res;
      }
    }
  }
  res[0] = 0;
  return res;
}

// Update color selection screen
void redraw_color_selection(int op){
  float block_size = 150.0;
  ALLEGRO_COLOR options[7] = {rgb(255, 0, 0), rgb(0, 0, 255), rgb(0, 255, 0), rgb(100, 100, 100), rgb(170, 0, 240), rgb(247, 165, 0), rgb(0, 0, 0)};
  clear_display(BLUE, false);
  draw_text(PIXEL_FONT, 32, YELLOW, (sw/2), 35, ALLEGRO_ALIGN_CENTRE, "SELECT YOUR COLOR", false);
  for(int i = -3; i < 4; i++){
    al_draw_filled_rounded_rectangle((sw/2)+(i*(block_size+25.0))-(block_size/2), (sh/2)-(block_size/2), (sw/2)+(i*(block_size+25))+(block_size/2), (sh/2)+(block_size/2), 10, 10, options[i+3]);
    al_draw_rounded_rectangle((sw/2)+(i*(block_size+25.0))-(block_size/2), (sh/2)-(block_size/2), (sw/2)+(i*(block_size+25))+(block_size/2), (sh/2)+(block_size/2), 10, 10, colors[i+3], 5);
  }
  al_flip_display();
}

// Select car color
int color_selection(){
  int op = 0;
  // Selected option = WHITE
  colors[0] = WHITE;
  colors[1] = YELLOW;
  colors[2] = YELLOW;
  colors[3] = YELLOW;
  colors[4] = YELLOW;
  colors[5] = YELLOW;
  colors[6] = YELLOW;
  redraw_color_selection(op);
  while (true) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(queue, &ev);
    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      return 7;
    }
    else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) return -1;
      switch (ev.keyboard.keycode) {
        case ALLEGRO_KEY_RIGHT:
        case ALLEGRO_KEY_D:
          if(op < 6){
            colors[op] = YELLOW;
            op++;
            colors[op] = WHITE;
            redraw_color_selection(op);
          }
          break;
        case ALLEGRO_KEY_LEFT:
        case ALLEGRO_KEY_A:
          if(op > 0){
            colors[op] = YELLOW;
            op--;
            colors[op] = WHITE;
            redraw_color_selection(op);
          }
          break;
        case ALLEGRO_KEY_ENTER:
          return op;
      }
    }
  }
  return 0;
}
