#include "game_engine.h"

#include <iostream>

int main() {

    game_engine GameEngine;
    GameEngine.init();

    while (!GameEngine.check_quit())
        GameEngine.game_loop();

    return 0;
}
