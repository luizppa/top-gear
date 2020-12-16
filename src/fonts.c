#include "./include/fonts.h"

ALLEGRO_FONT* DISKUN_60;
ALLEGRO_FONT* DISKUN_80;
ALLEGRO_FONT* DISKUN_100;
ALLEGRO_FONT* PIXEL_14;
ALLEGRO_FONT* PIXEL_22;
ALLEGRO_FONT* PIXEL_28;
ALLEGRO_FONT* PIXEL_32;

// Returns a font with the specified name and size
ALLEGRO_FONT* font(char* font_name, int size){
  return al_load_font(font_name, size, 1);
}

// Initialize fonts
void init_fonts(){
  DISKUN_60 = font(DISKUN_FONT, 60);
  DISKUN_80 = font(DISKUN_FONT, 80);
  DISKUN_100 = font(DISKUN_FONT, 100);
  PIXEL_14 = font(PIXEL_FONT, 14);
  PIXEL_22 = font(PIXEL_FONT, 22);
  PIXEL_28 = font(PIXEL_FONT, 28);
  PIXEL_32 = font(PIXEL_FONT, 32);
}

// Free pointers
void destroy_fonts(){
  al_destroy_font(DISKUN_60);
  al_destroy_font(DISKUN_80);
  al_destroy_font(DISKUN_100);
  al_destroy_font(PIXEL_14);
  al_destroy_font(PIXEL_22);
  al_destroy_font(PIXEL_28);
  al_destroy_font(PIXEL_32);
}
