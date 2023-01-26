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

#include "InputParser.h"
#include "PlayerManager.h"
#include "MapManager.h"
#include "MultimediaManager.h"
#include "DoorManager.h"
#include "Renderer.h"

class GameEngine {
public:
    void Init() {
        // Allocate new Inputs objects (to pass to some managers)
        inputs = new Inputs;
        inputs->quit_flag = false;

        // Allocate game data object (shared across managers & renderer)
        gameData = new GameData;

        // Initialize managers & InputParser
        multimediaManager.Init(gameData);
        multimediaManager.CreateWindowRenderer(1280, 720);
        multimediaManager.LoadTextures(TEXTURE_WALLS, "wall_textures.bmp", 6, 110);
        multimediaManager.LoadTextures(TEXTURE_GUARD, "guard.bmp", 8, 51);
        multimediaManager.LoadTextures(TEXTURE_OBJECTS, "objects.bmp", 5, 50);
        playerManager.Init(inputs, gameData);
        mapManager.Init(gameData);
        mapManager.LoadMap("map.csv");
        doorManager.Init(gameData);
        inputParser.Init(inputs);
        renderer.Init(gameData);

        // Set player location & view direction
        playerManager.SetPlayer(point2(4.8, 28.1), vec2(1, 1));
    }

    void Exit() {
        // Exit managers (that have memory clean-up to do)
        mapManager.Exit();
        multimediaManager.Exit();

        delete inputs;
        delete gameData;
    }

    // Main game loop continuously executed in the main function
    void GameLoop() {
        static bool running = true;

        if (running) {
            playerManager.Update();
            mapManager.UpdateSpritePerpLines();
            renderer.RenderFrame();
        } else
            SDL_Delay(40);

        inputParser.ParseInputs();
        doorManager.Update();

        running = inputs->AnyActiveInputs() || gameData->Map.anyDoorsAwaitingRendering;
    }

public:
    Inputs* inputs;

private:
    GameData* gameData;

    MultimediaManager multimediaManager;
    PlayerManager playerManager;
    MapManager mapManager;
    DoorManager doorManager;
    InputParser inputParser;

    Renderer renderer;
};
