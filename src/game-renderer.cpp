#include<vector>

#include "../include/game-renderer.hpp"
#include "../include/display.hpp"
#include "../include/colors.hpp"
#include "../include/fonts.hpp"
#include "../include/menus.hpp"
#include "../include/environment.hpp"
#include "../include/utils.hpp"

namespace top_gear {

    float GameRenderer::obstacle_distance(Obstacle* obstacle){
        return obstacle->get_y() - race->get_player()->get_y();
    }

    /**
     * Distance from the car wheels to the bottom of the screen
    */ 
    float GameRenderer::distance_from_bottom(Car* oponent){
        return oponent->get_y() - race->get_player()->get_y();
    }

    float GameRenderer::delta(float distance){
        return utils::get_delta(STREET_WIDTH, STREET_WIDTH/VIEW_ANGLE, STREET_VISIBLE_LENGTH, distance);
    }

    bool GameRenderer::is_obstacle_visible(Obstacle* obstacle){
        float distance = this->obstacle_distance(obstacle);
        float delta = this->delta(distance);
        float apparent_height = obstacle->get_height() * delta;
        return (distance <= STREET_VISIBLE_LENGTH) && (distance >= - apparent_height);
    }

    /**
     * Verify if the car should be rendered
    */
    bool GameRenderer::is_car_visible(Car* car){
        float delta = this->delta(distance_from_bottom(car));
        float apparent_height = car->get_height()*delta;
        return (distance_from_bottom(car) <= STREET_VISIBLE_LENGTH - 8) && (distance_from_bottom(car) >= -apparent_height - (20 * delta));
    }

    void GameRenderer::draw_obstacle(Obstacle* obstacle){
        float distance = this->obstacle_distance(obstacle);
        float delta = this->delta(distance);

        obstacle->set_apparent_width(obstacle->get_width()*delta);
        obstacle->set_apparent_height(obstacle->get_height()*delta);
        obstacle->set_screen_x(race->get_camera_position() + (obstacle->get_x() * delta));
        obstacle->set_screen_y(SCREEN_HEIGHT - distance - (obstacle->get_apparent_height()/2));
        al_draw_scaled_bitmap(obstacle->get_texture(), 0, 0, obstacle->get_width(), obstacle->get_height(), obstacle->get_screen_x()-(obstacle->get_apparent_width()/2), obstacle->get_screen_y()-(obstacle->get_apparent_height()/2), obstacle->get_apparent_width(), obstacle->get_apparent_height(), 0);
    }

    /**
     * Draw an oponent's car
    */
    void GameRenderer::draw_oponent(Car* car){
        float distance = this->distance_from_bottom(car);
        int camera_position = race->get_camera_position();

        float delta = this->delta(distance);

        car->set_apparent_width(car->get_width() * delta);
        car->set_apparent_height(car->get_height() * delta);
        car->set_screen_x(camera_position + car->get_x());
        car->set_screen_y(SCREEN_HEIGHT - distance - (car->get_height()/2));

        float height_variation = car->get_height() - car->get_apparent_height();

        ALLEGRO_FONT *name_font = fonts::font(PIXEL_FONT, 14 * delta);
        display::draw_text(name_font, colors::WHITE, camera_position + (car->get_x()*delta), SCREEN_HEIGHT - distance - (20*delta) - car->get_height() + height_variation, ALLEGRO_ALIGN_CENTRE, car->get_name(), false);
        al_destroy_font(name_font);

        al_draw_scaled_bitmap(car->get_texture(), 0, 0, car->get_width(), car->get_height(), camera_position + (car->get_x() * delta) - (car->get_apparent_width() / 2), SCREEN_HEIGHT - distance - car->get_apparent_height(), car->get_apparent_width(), car->get_apparent_height(), 0);
    }

    /**
     * Draw player's car
    */
    void GameRenderer::draw_player(){
        Car* player = race->get_player();
        al_draw_bitmap(player->get_texture(), (SCREEN_WIDTH/2) - (player->get_width() / 2), SCREEN_HEIGHT - player->get_height(), 0);
    }

    void GameRenderer::draw_track(){
        int camera_position = race->get_camera_position();
        al_draw_bitmap(race->get_map_landscape(), -(2600/2)+(camera_position/PARALAX), 0, 0);
        al_draw_bitmap(display::LAS_VEGAS_ROAD_BITMAP, camera_position-(STREET_WIDTH/2), SCREEN_HEIGHT-STREET_VISIBLE_LENGTH, 0);
    }

    void GameRenderer::draw_obstacles(){
        std::vector<Obstacle*>::iterator it;
        for (it = race->get_obstacles().begin(); it != race->get_obstacles().end(); ++it) {
            if(this->is_obstacle_visible(*it)){
                this->draw_obstacle(*it);
            }
        }
    }

    void GameRenderer::draw_cars(){
        int placement = 1;
        bool player_drawn = false;
        std::vector<Car*>::iterator it;
        Car* player = race->get_player();

        if(!race->has_finished()) {
            race->sort();
        }
        for(it = race->get_oponents().end() - 1; it != race->get_oponents().begin() - 1; --it){
            Car* car = *it;
            if(!race->has_finished() && car->get_y() > player->get_y()) {
                placement++;
            }
            else if(car->get_y() <= player->get_y() && !player_drawn){
                this->draw_player();
                player_drawn = true;
            }

            if(this->is_car_visible(car)){
                this->draw_oponent(car);
            }
        }
    }

    void GameRenderer::draw_hud(){
        float minimap_heigth = TRACK_LENGTH/300;
        float player_minimap_position = utils::min(race->get_player()->get_y(), TRACK_LENGTH)/300;
        int gear_progress = utils::min(10, race->get_player()->get_gear_progress());

        // Minimap
        al_draw_line(30, SCREEN_HEIGHT-250, 30, (SCREEN_HEIGHT-250)-minimap_heigth, colors::YELLOW, 6);
        al_draw_filled_circle(30, SCREEN_HEIGHT-250, 9, colors::ORANGE);
        al_draw_filled_circle(30, (SCREEN_HEIGHT-250)-minimap_heigth, 9, colors::ORANGE);
        for (int i = 0; i < race->get_oponents_count(); i++) {
            al_draw_filled_circle(30, (SCREEN_HEIGHT-250)-(utils::min(race->get_oponents().at(i)->get_y(), TRACK_LENGTH)/300), 9, colors::RED);
        }
        al_draw_filled_circle(30, (SCREEN_HEIGHT-250)-player_minimap_position, 9, colors::BLUE);

        // Position
        al_draw_text(fonts::DISKUN_60, colors::YELLOW, 30, SCREEN_HEIGHT-160, ALLEGRO_ALIGN_LEFT, "POSITION");
        al_draw_textf(fonts::DISKUN_100, colors::YELLOW, 30, SCREEN_HEIGHT-100, ALLEGRO_ALIGN_LEFT, "%dth", race->get_placement());

        // Gears
        al_draw_text(fonts::DISKUN_60, colors::YELLOW, SCREEN_WIDTH-30, SCREEN_HEIGHT-280, ALLEGRO_ALIGN_RIGHT, "GEAR");
        al_draw_textf(fonts::DISKUN_80, colors::YELLOW, SCREEN_WIDTH-30, SCREEN_HEIGHT-220, ALLEGRO_ALIGN_RIGHT, "%d", race->get_player()->get_gear());
        al_draw_filled_rounded_rectangle(15, 15, 258, 85, 5.0, 5.0, colors::YELLOW);
        al_draw_filled_rounded_rectangle(20, 20, 253, 80, 3.0, 3.0, colors::rgb(50, 50, 50));
        for (int i = 1; i <= gear_progress; i++) {
        int red = (int)(200*(i/10.0));
        int green = 65+(int)(190*((10-i)/10.0));
        al_draw_filled_rectangle(i*23, 23, (i*23)+20, 77, colors::rgb(red, green, 40));
        }

        // Speed
        al_draw_text(fonts::DISKUN_60, colors::YELLOW, SCREEN_WIDTH-30, SCREEN_HEIGHT-140, ALLEGRO_ALIGN_RIGHT, "SPEED");
        al_draw_textf(fonts::DISKUN_80, colors::YELLOW, SCREEN_WIDTH-30, SCREEN_HEIGHT-80, ALLEGRO_ALIGN_RIGHT, "%.0f Km/h", utils::max(0, race->get_player()->get_speed()));

        // Nitrox
        al_draw_text(fonts::DISKUN_60, colors::YELLOW, SCREEN_WIDTH-30, 20, ALLEGRO_ALIGN_RIGHT, "NITROX");
        al_draw_filled_rounded_rectangle(SCREEN_WIDTH-60, 90, SCREEN_WIDTH-30, 300, 5.0, 5.0, colors::YELLOW);
        al_draw_filled_rounded_rectangle(SCREEN_WIDTH-55, 95, SCREEN_WIDTH-35, 295, 3.0, 3.0, colors::rgb(50, 50, 50));
        al_draw_filled_rectangle(SCREEN_WIDTH-53, 293-(196*(race->get_player()->get_nitrox()/100)), SCREEN_WIDTH-37, 293, colors::RED);
    }

    void GameRenderer::draw_scenario(){
        al_clear_to_color(race->get_map_soil());

        this->draw_track();
        this->draw_obstacles();
        this->draw_cars();
        this->draw_hud();
    }

    void GameRenderer::draw_paused_game(int op){
        this->draw_scenario();
        
        // Draw dialog box
        al_draw_filled_rectangle((SCREEN_WIDTH/2)-300, utils::normalize(-200), (SCREEN_WIDTH/2)+300, utils::normalize(200), colors::BLUE);
        al_draw_rectangle((SCREEN_WIDTH/2)-300, utils::normalize(-200), (SCREEN_WIDTH/2)+300, utils::normalize(200), colors::YELLOW, 5);

        display::draw_text(fonts::PIXEL_28, menus::get_option_color(op, 0), SCREEN_WIDTH/2, utils::normalize(-71), ALLEGRO_ALIGN_CENTRE, "RESUME", false);
        display::draw_text(fonts::PIXEL_28, menus::get_option_color(op, 1), SCREEN_WIDTH/2, utils::normalize(-33), ALLEGRO_ALIGN_CENTRE, "RESTART", false);
        display::draw_text(fonts::PIXEL_28, menus::get_option_color(op, 2), SCREEN_WIDTH/2, utils::normalize(5), ALLEGRO_ALIGN_CENTRE, "OPTIONS", false);
        display::draw_text(fonts::PIXEL_28, menus::get_option_color(op, 3), SCREEN_WIDTH/2, utils::normalize(38), ALLEGRO_ALIGN_CENTRE, "MAIN MENU", false);
        al_flip_display();
    }

    void GameRenderer::draw_game(){
        this->draw_scenario();
        al_flip_display();
    }
}