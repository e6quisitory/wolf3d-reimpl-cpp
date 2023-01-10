#include "game_engine.h"

#include <iostream>

int main() {

    game_engine GameEngine;
    GameEngine.init();

    while (GameEngine.GameData->quit_flag == false)
        GameEngine.game_loop();

    GameEngine.exit();

    return 0;
}
