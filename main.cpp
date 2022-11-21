#include <iostream>
#include <string>

#include "game_engine.h"

int main() {

    // Create game engine object
    game_engine ge;

    // Initialize game engine components
    ge.create_map("map.csv");

    ge.create_player(vec2(8.56,11.1), vec2(1,0), 72.0);

    ge.create_renderer(1280, 720);
    ge.create_updater();

    // Main game loop
    bool running = true;
    while (1) {
        if (ge.check_quit())
            break;
        else
            ge.game_loop();
    }

    return 0;
}
