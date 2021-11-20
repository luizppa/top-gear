#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "./game.hpp"

#define PARALAX 1.3
#define VIEW_ANGLE 16.0
#define STREET_VISIBLE_LENGTH 500.0

namespace top_gear {
    class GameRenderer {
        
        private:
            Race* race = nullptr;
            float obstacle_distance(Obstacle* obstacle);
            float distance_from_bottom(Car* car);
            float delta(float distance);
            bool is_obstacle_visible(Obstacle* obstacle);
            bool is_car_visible(Car* car);
            void draw_obstacle(Obstacle* obstacle);
            void draw_oponent(Car* car);
            void draw_player();
            void draw_track();
            void draw_obstacles();
            void draw_cars();
            void draw_hud();
            void draw_scenario();

        public:
            GameRenderer(Race* race) { this->race = race; }
            void draw_paused_game(int op);
            void draw_game();
    };
}

#endif