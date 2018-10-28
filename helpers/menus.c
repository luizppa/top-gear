#include "menus.h"

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

// Runs main menu
int main_menu(){
  int op = 1;
  colors[0] = WHITE;
  colors[1] = YELLOW;
  colors[2] = YELLOW;
  colors[3] = YELLOW;
  redraw_main_menu(op);
  while (true) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(queue, &ev);
    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      return 4;
    }
    else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) return -1;
      switch (ev.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
        case ALLEGRO_KEY_W:
          if(op > 1){
            colors[op-1] = YELLOW;
            op--;
            colors[op-1] = WHITE;
            redraw_main_menu(op);
          }
          break;
        case ALLEGRO_KEY_DOWN:
        case ALLEGRO_KEY_S:
          if(op < 4){
            colors[op-1] = YELLOW;
            op++;
            colors[op-1] = WHITE;
            redraw_main_menu(op);
          }
          break;
        case ALLEGRO_KEY_ENTER:
          return op;
          break;
      }
    }
  }
  draw_title();
  return 0;
}
