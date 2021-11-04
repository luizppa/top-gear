#include "../include/environment.hpp"
#include "../include/colors.hpp"
#include "../include/display.hpp"
#include "../include/fonts.hpp"
#include "../include/sounds.hpp"

namespace top_gear {

  namespace environment {

    // Setings
    // Not a good ideia to change this, probably
    float fps = 60.0;
    // Turn on game music
    bool music_on = true;
    // Turn off game music
    bool sounds_on = true;
    // Cars will colide
    bool collisions = true;
    /*
      Debug markers will appear on screen and
      events will be logged if debug is set to true
    */
    bool debug = false;
    // Oponent cars will be controlled by AI
    bool ai_pilots = true;
    // Lateral movement speed
    float movement_speed = 18.0;

    ALLEGRO_DISPLAY* display = nullptr;
    ALLEGRO_EVENT_QUEUE* event_queue = nullptr;
    ALLEGRO_EVENT_QUEUE* input_event_queue = nullptr;
    ALLEGRO_EVENT_QUEUE* timer_event_queue = nullptr;
    ALLEGRO_TIMER* timer = nullptr;
    ALLEGRO_KEYBOARD_STATE key_state;

    // Install plugins
    void init_environment(){
      if (!al_init()) {
        printf("Allegro init failed.\n");
      }
      if (!al_install_audio() || !al_install_keyboard()) {
        printf("Install failed.\n");
      }
      al_init_font_addon();
      al_init_ttf_addon();
      al_init_acodec_addon();
      al_init_image_addon();
      al_init_primitives_addon();
    }

    // Configure sound assets
    void setup_sounds(){
      sounds::init_sounds();
      al_reserve_samples(5);
    }

    // Configure display
    void setup_display(){
      colors::init_colors();
      display::init_bitmaps();
      fonts::init_fonts();

      // windowed
      al_set_new_display_flags(ALLEGRO_WINDOWED);
      // al_set_new_display_flags(ALLEGRO_RESIZABLE);
      display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
      al_set_window_title(display, "Top Gear");
      al_set_display_icon(display, display::GAME_ICON);
    }

    // Configure event listeners
    void setup_events(){
      event_queue = al_create_event_queue();
      input_event_queue = al_create_event_queue();
      timer_event_queue = al_create_event_queue();
      timer = al_create_timer(1.0 / fps);
      
      al_register_event_source(event_queue, al_get_display_event_source(display));
      al_register_event_source(event_queue, al_get_timer_event_source(timer));
      al_register_event_source(event_queue, al_get_keyboard_event_source());
      al_register_event_source(input_event_queue, al_get_display_event_source(display));
      al_register_event_source(input_event_queue, al_get_keyboard_event_source());
      al_register_event_source(timer_event_queue, al_get_timer_event_source(timer));
      al_start_timer(timer);
    }

    // Free pointers
    void destroy_environment(){
      al_destroy_timer(timer);
      al_destroy_display(display);
      al_destroy_event_queue(event_queue);
      al_destroy_event_queue(input_event_queue);
      al_destroy_event_queue(timer_event_queue);
    }

  }

}
