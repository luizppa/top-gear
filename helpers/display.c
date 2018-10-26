#include "display.h"

// Draw a message to the screen
void draw_text(char* font_name, int size, ALLEGRO_COLOR color, int position_x, int position_y, int align, char* text){
  ALLEGRO_FONT *selected_font = font(font_name, size);
  al_draw_text(selected_font, color, position_x, position_y, align, text);
  al_flip_display();
  al_destroy_font(selected_font);
}

// Clear the display to a given color
void clear_display(ALLEGRO_COLOR color){
  al_clear_to_color(color);
  al_flip_display();
}
