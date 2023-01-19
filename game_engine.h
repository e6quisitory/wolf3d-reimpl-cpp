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

#include "input_parser.h"
#include "player_manager.h"
#include "map_manager.h"
#include "multimedia_manager.h"
#include "door_manager.h"
#include "renderer.h"

class game_engine {
public:
    void init() {
        // Allocate new Inputs objects (to pass to some managers)
        Inputs = new inputs;
        Inputs->quit_flag = false;

        // Allocate game data object (shared across managers & renderer)
        GameData = new game_data;

        // Initialize managers & InputParser
        MultimediaManager.init(GameData);
        MultimediaManager.create_window_and_renderer(1280, 720);
        MultimediaManager.load_textures(WALLS, "wall_textures.bmp", 6, 110);
        MultimediaManager.load_textures(GUARD, "guard.bmp", 8, 51);
        PlayerManager.init(Inputs, GameData);
        MapManager.init(GameData);
        MapManager.load_map("map.csv");
        DoorManager.init(GameData);
        InputParser.init(Inputs);
        Renderer.init(GameData);

        // Set player location & view direction
        PlayerManager.set_player(point2(6,11), vec2(1,-1));
    }

    void exit() {
        // Exit managers (that have memory clean-up to do)
        MapManager.exit();
        MultimediaManager.exit();

        delete Inputs;
        delete GameData;
    }

    // Main game loop continuously executed in the main function
    void game_loop() {
        static bool running = true;

        if (running) {
            PlayerManager.update();
            MapManager.update_sprite_perp_lines();
            Renderer.render_frame();
        } else
            SDL_Delay(40);

        InputParser.parse_inputs();
        DoorManager.update();

        running = Inputs->any_active_inputs() || GameData->Map.any_doors_awaiting_rendering;
    }

public:
    inputs* Inputs;

private:
    game_data* GameData;

    multimedia_manager MultimediaManager;
    player_manager PlayerManager;
    map_manager MapManager;
    door_manager DoorManager;
    input_parser InputParser;

    renderer Renderer;
};
