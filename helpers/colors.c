#include "colors.h"

ALLEGRO_COLOR BLACK;
ALLEGRO_COLOR WHITE;
ALLEGRO_COLOR BLUE;
ALLEGRO_COLOR RED;
ALLEGRO_COLOR ORANGE;

// Returns a rgb allegro color
ALLEGRO_COLOR rgb(int r, int g, int b){
  return al_map_rgb(r, g, b);
}

void init_colors(){
  BLACK = rgb(0, 0, 0);
  WHITE = rgb(255, 255, 255);
  BLUE = rgb(42, 51, 232);
  RED = rgb(255, 68, 23);
  ORANGE = rgb(255, 168, 40);
}
