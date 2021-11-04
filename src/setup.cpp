#include <cstdlib>
#include <ctime>

#include "../include/environment.hpp"
#include "../include/sounds.hpp"
#include "../include/fonts.hpp"
#include "../include/colors.hpp"
#include "../include/display.hpp"

namespace setup {

    void setup(){
        /* Setting path */
        ALLEGRO_PATH* p = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
        al_change_directory(al_path_cstr(p, '\\'));
        al_destroy_path(p);

        /* Settings */
        top_gear::environment::setup_display();
        top_gear::environment::setup_sounds();
        top_gear::sounds::music = top_gear::sounds::set_music(TITLE_MUSIC);
        al_attach_audio_stream_to_mixer(top_gear::sounds::music, al_get_default_mixer());
        top_gear::sounds::start_music(top_gear::sounds::music, true);
        top_gear::environment::setup_events();
    }

    bool init() {
        srand(time(nullptr));
        top_gear::environment::init_environment();

        setup();
        if (!top_gear::environment::display || !top_gear::environment::event_queue || !top_gear::environment::timer || !top_gear::sounds::music) {
            return false;
        }

        return true;
    }

    /* Clears allegro environment and assets batches */
    void finalize(){
        top_gear::display::destroy_bitmaps();
        top_gear::sounds::destroy_sounds();
        top_gear::environment::destroy_environment();
    }

}
