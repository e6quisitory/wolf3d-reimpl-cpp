#pragma once

#include <thread>
#include <chrono>

#include "game_data.h"
#include "input_manager.h"
#include "player_manager.h"
#include "map_manager.h"
#include "multimedia_manager.h"
#include "door_manager.h"
#include "renderer.h"

using namespace std::chrono_literals;

class game_engine {
public:
    void init() {
        // Allocate game data object (shared across managers & renderer)
        GameData = new game_data;

        // Initialize managers
        MultimediaManager.init(GameData);
        MultimediaManager.create_window_renderer(1280, 720);
        MultimediaManager.load_wall_textures("wall_textures.bmp", 6);

        PlayerManager.init(GameData);

        MapManager.init(GameData);
        MapManager.load_map("map.csv");

        DoorManager.init(GameData);

        InputManager.init(GameData);

        // Initialize Renderer
        Renderer.init(GameData);

        // Set player location & view direction
        PlayerManager.set_player(point2(5.86,11.1), vec2(1,0));
    }

    void exit() {
        // Exit managers
        MapManager.exit();
        MultimediaManager.exit();

        // Delete game_data object
        delete GameData;
    }

    bool check_quit() {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return true;
            } else
                return false;
        }
        return false;
    }

    void game_loop() {
        static bool running = true;

        if (running) {
            PlayerManager.update();
            Renderer.render_frame();
        } else
            std::this_thread::sleep_for(40ms);

        running = false;

        running |= InputManager.poll_inputs();
        running |= DoorManager.update();
    }


private:

    game_data* GameData;

    multimedia_manager MultimediaManager;
    player_manager PlayerManager;
    map_manager MapManager;
    door_manager DoorManager;
    input_manager InputManager;

    renderer Renderer;
};