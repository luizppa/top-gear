#ifndef GAME_H
#define GAME_H

#include "./car.hpp"
#include "./maps.hpp"
#include "./display.hpp"
#include "./game-renderer.hpp"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define STREET_WIDTH 1300.0
#define STREET_LEFT_LIMIT (SCREEN_WIDTH - STREET_WIDTH)/2
#define OBSTACLE_COUNT 30
#define TRACK_LENGTH 50000.0

enum Direction {LEFT, RIGHT};
typedef enum Direction Direction;
enum InputResult {CONTINUE, PAUSE};
typedef enum InputResult InputResult; 

namespace top_gear {
    class Race {
        private:
            GameRenderer renderer = nullptr;

            int oponent_count;
            int placement;
            int best_time;
            float camera_position;
            double race_time;
            bool is_best_time = false;
            bool is_tournament;
            bool is_single_race;
            bool finished;

            std::vector<Obstacle*> obstacles;
            std::vector<Car*> oponents;
            Map map = Map::NONE;
            std::string map_music_title;
            Car* player;
            Car** leaderboard;
            ALLEGRO_COLOR map_soil;
            ALLEGRO_BITMAP* map_landscape = nullptr;

            void setup(ALLEGRO_BITMAP* player_texture, std::vector<Car*> tournament_cars);
            void create_obstacles();
            void create_oponents(std::vector<Car*> tournament_cars);
            void setup_music();
            void setup_scenario();
            void restart();

            void countdown();
            int perform_pause_action(int op);
            int pause_menu_loop();
            int pause_menu();
            InputResult handle_key_events(ALLEGRO_EVENT ev);

            int update();
            void control_gears();
            void move();

            int static load_best_time();
            static float get_player_start_distance(int placement);
            static float get_player_start_position(int placement);
            
        public:
            Race(ALLEGRO_BITMAP* player_texture, Map map, std::vector<Car*> tournament_cars);
            Race(ALLEGRO_BITMAP* player_texture, Map map, int oponents_amount);
            int play();
            int tournament(ALLEGRO_BITMAP* player_texture, int oponents_amount);

            // Getters
            int get_placement() { return this->placement; }
            int get_camera_position() { return this->camera_position; }
            int get_obstacles_count() { return this->obstacles.size(); }
            int get_oponents_count() { return this->oponents.size(); }
            bool has_finished() { return this->finished; }
            void sort();
            std::vector<Obstacle*> get_obstacles() { return this->obstacles; }
            std::vector<Car*> get_oponents() { return this->oponents; }
            ALLEGRO_COLOR get_map_soil() { return this->map_soil; }
            ALLEGRO_BITMAP* get_map_landscape() { return this->map_landscape; }
            Car* get_player() { return this->player; }

            // Setters
            void set_placement(int placement) { this->placement = placement; }
    };
}

#endif
