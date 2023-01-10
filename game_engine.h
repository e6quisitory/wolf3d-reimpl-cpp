/*
 * game_engine.h:
 *
 * The top level block that houses the main game_data object, the various managers, and the renderer.
 * Initializes and exits the managers, as well as contains the main game_loop to be continuously executed
 * in the main function (in main.cpp).
 *
 */

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

class game_engine {
public:
    void init() {
        // Allocate game data object (shared across managers & renderer)
        GameData = new game_data;

        // Initialize managers
        MultimediaManager.init(GameData);
        MultimediaManager.create_window_and_renderer(1280, 720);
        MultimediaManager.load_wall_texture_pairs("wall_textures.bmp", 6);

        PlayerManager.init(GameData);

        MapManager.init(GameData);
        MapManager.load_map("map.csv");

        DoorManager.init(GameData);

        InputManager.init(GameData);

        // Initialize Renderer
        Renderer.init(GameData);

        // Set player location & view direction
        PlayerManager.set_player(point2(6,11), vec2(1,-1));
    }

    void exit() {
        // Exit managers (that have memory clean-up to do)
        MapManager.exit();
        MultimediaManager.exit();

        // Delete game_data object
        delete GameData;
    }

    // Main game loop continuously executed in the main function
    void game_loop() {
        static bool running = true;
        if (running) {
            InputManager.poll_inputs();
            PlayerManager.update();
            DoorManager.update();
            Renderer.render_frame();
        } else
            SDL_Delay(40);

        /* Need to re-add checking of whether new rendering needs to happen or not. Rn it's just wasting CPU at idle. */
        /* The line below doesn't work. */

        //running = GameData->Inputs.any_inputs() || GameData->Map.any_active_doors;
    }

public:
    game_data* GameData;

private:
    multimedia_manager MultimediaManager;
    player_manager PlayerManager;
    map_manager MapManager;
    door_manager DoorManager;
    input_manager InputManager;

    renderer Renderer;
};