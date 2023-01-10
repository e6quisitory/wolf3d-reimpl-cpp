#include "game_engine.h"

#include <iostream>

int main() {

    game_engine GameEngine;
    GameEngine.init();

    while (!GameEngine.check_quit_and_mouselock())
        GameEngine.game_loop();

    GameEngine.exit();

    return 0;
}
