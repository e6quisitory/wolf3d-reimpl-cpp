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

    // Checks to see if user wants to close window; returns true if so
    bool check_quit_and_mouselock() {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return true;
            }
            /*} else if (e.type == SDL_KEYDOWN) {             // Can X out of window or press Escape key to exit game
                switch (e.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE: return true;
                    case SDL_SCANCODE_GRAVE:                // Pressing tilde key (below escape key) will toggle mouse locking to window
                        bool cursor_shown = SDL_ShowCursor(SDL_QUERY);
                        SDL_SetWindowMouseGrab(GameData->Multimedia.sdl_window, cursor_shown ? SDL_TRUE : SDL_FALSE);
                        SDL_ShowCursor(cursor_shown ? SDL_DISABLE : SDL_ENABLE);
                        return false;
                }
            } else
                return false;*/
        }
        return false;
    }

    // Main game loop continuously executed in the main function
    void game_loop() {
        static bool running = true;

        if (running) {
            PlayerManager.update();
            Renderer.render_frame();
        } else
            SDL_Delay(40);  // If there are no new changes that prompt rendering, wait 40ms then check again (to save CPU)

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