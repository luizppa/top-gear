#include "menus.h"
#include "game.h"
#include "sounds.h"
#include "environment.h"

ALLEGRO_COLOR colors[4];

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
int car_selection();

// Runs main menu
int main_menu(){
  int op = 1;
  // Selected option = WHITE
  colors[0] = WHITE;
  colors[1] = YELLOW;
  colors[2] = YELLOW;
  colors[3] = YELLOW;
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

void redraw_mode_selection(int op){
  float square_side = 250.0;
  clear_display(BLUE, false);
  al_draw_rectangle((sw/2)-25-square_side, (sh/2)-(square_side/2), (sw/2)-25, (sh/2)+(square_side/2), colors[0], 5);
  draw_text(PIXEL_FONT, 28, colors[0], (sw/2)-25-(square_side/2), (sh/2)-14, ALLEGRO_ALIGN_CENTRE, "QUICK PLAY", false);
  al_draw_rectangle((sw/2)+25+square_side, (sh/2)-(square_side/2), (sw/2)+25, (sh/2)+(square_side/2), colors[1], 5);
  draw_text(PIXEL_FONT, 28, colors[1], (sw/2)+25+(square_side/2), (sh/2)-14, ALLEGRO_ALIGN_CENTRE, "TOURNAMENT", true);
}

// Select gamemode
int mode_selection(){
  int op = 1;
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
              op = car_selection();
              if(op == 4) return op;
              if(op == -1) return 0;
              op = play(get_car(op, 4, true));
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

void redraw_car_selection(int op){
  float square_side = 500.0;
  float dominus_w = 290.0, dominus_h = 70.0;
  float octane_w = 187.0, octane_h = 88.0;
  clear_display(BLUE, false);
  al_draw_rectangle((sw/2)-25-square_side, (sh/2)-(square_side/2), (sw/2)-25, (sh/2)+(square_side/2), colors[0], 5);
  draw_text(PIXEL_FONT, 22, colors[0], (sw/2)-25-(square_side/2), (sh/2)-(square_side/2)+12, ALLEGRO_ALIGN_CENTRE, "Dominus GT", false);
  al_draw_bitmap(DOMINUS_GT_ICON_BITMAP, (sw/2)-25-((square_side-dominus_w)/2)-dominus_w, (sh/2)-(dominus_h/2), 0);
  al_draw_rectangle((sw/2)+25+square_side, (sh/2)-(square_side/2), (sw/2)+25, (sh/2)+(square_side/2), colors[1], 5);
  al_draw_bitmap(OCTANE_ZSR_ICON_BITMAP, (sw/2)+25+((square_side-octane_w)/2), (sh/2)-(octane_h/2), 0);
  draw_text(PIXEL_FONT, 28, colors[1], (sw/2)+25+(square_side/2), (sh/2)-(square_side/2)+12, ALLEGRO_ALIGN_CENTRE, "Octane ZSR", true);
}

// Select car
int car_selection(){
  int op = 1;
  colors[0] = WHITE;
  colors[1] = YELLOW;
  redraw_car_selection(op);
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
            redraw_car_selection(op);
          }
          break;
        case ALLEGRO_KEY_LEFT:
        case ALLEGRO_KEY_A:
          if(op > 1){
            colors[op-1] = YELLOW;
            op--;
            colors[op-1] = WHITE;
            redraw_car_selection(op);
          }
          break;
        case ALLEGRO_KEY_ENTER:
          return op;
      }
    }
  }
  return 0;
}
