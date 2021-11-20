#ifndef MENUS_H
#define MENUS_H

#define MENU_ACTION_GO_BACK -1
#define MENU_ACTION_QUIT_GAME 0
#define MENU_ACTION_DONE 1
#define MENU_ACTION_RESUME 2

#define GAME_MODE_QUICK_PLAY 0
#define GAME_MODE_TOURNAMENT 1

namespace top_gear {

    namespace menus {

        ALLEGRO_COLOR get_option_color(int option_number, int selected_option);
        int main_menu();
        int options_menu();

    }

}

#endif