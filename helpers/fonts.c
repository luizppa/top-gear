#include "fonts.h"

// Returns a font with the specified name and size
ALLEGRO_FONT* font(char* font_name, int size){
  return al_load_font(font_name, size, 1);
}
