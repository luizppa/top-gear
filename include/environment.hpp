#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define SAVE_FILE "../saves/record.tg"

namespace top_gear {

    namespace environment {

        extern float fps;
        extern bool music_on;
        extern bool sounds_on;
        extern bool collisions;
        extern bool debug;
        extern bool ai_pilots;
        extern float movement_speed;

        extern ALLEGRO_DISPLAY* display;
        extern ALLEGRO_EVENT_QUEUE* event_queue;
        extern ALLEGRO_EVENT_QUEUE* input_event_queue;
        extern ALLEGRO_EVENT_QUEUE* timer_event_queue;
        extern ALLEGRO_TIMER* timer;
        extern ALLEGRO_KEYBOARD_STATE key_state;

        void init_environment();
        void setup_sounds();
        void setup_display();
        void setup_events();
        void destroy_environment();

    }

}

#endif