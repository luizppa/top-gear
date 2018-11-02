#include "colors.h"

// Predefined colors
ALLEGRO_COLOR BLACK;
ALLEGRO_COLOR WHITE;
ALLEGRO_COLOR GREY;
ALLEGRO_COLOR BLUE;
ALLEGRO_COLOR LIGHT_BLUE;
ALLEGRO_COLOR RED;
ALLEGRO_COLOR ORANGE;
ALLEGRO_COLOR YELLOW;
ALLEGRO_COLOR GREEN;

// Returns a rgb allegro color
ALLEGRO_COLOR rgb(int r, int g, int b){
  return al_map_rgb(r, g, b);
}

// Initialize color values
void init_colors(){
  BLACK = rgb(0, 0, 0);
  WHITE = rgb(255, 255, 255);
  GREY = rgb(45, 45, 45);
  BLUE = rgb(42, 51, 232);
  LIGHT_BLUE = rgb(13, 162, 255);
  RED = rgb(255, 68, 23);
  ORANGE = rgb(255, 168, 40);
  YELLOW = rgb(255, 193, 30);
  GREEN = rgb(74, 191, 48);
}
