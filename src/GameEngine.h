/*
 * game_engine.h:
 *
 * The top level block that houses the main game_data object, the various managers, and the renderer.
 * Initializes and exits the managers, as well as contains the main game_loop to be continuously executed
 * in the main function (in main.cpp).
 *
 */

#pragma once

#include <string>
#include <cstring>

#include "InputParser.h"
#include "Managers/PlayerManager/PlayerManager.h"
#include "Managers/MapManager/MapManager.h"
#include "Managers/MultimediaManager/MultimediaManager.h"
#include "Managers/DoorManager/DoorManager.h"
#include "Renderer.h"

class GameEngine {
public:
    void Init() {
        // Allocate new Inputs objects (to pass to some managers)
        inputsBuffer = new InputsBuffer;
        inputsBuffer->quitGameFlag = false;

        // Allocate game data object (shared across managers & renderer)
        gameState = new GameState;

        // Allocate new multimedia object (used to store game assets in memory)
        multimedia = new Multimedia;

        // Initialize managers & InputParser
        multimediaManager.Init(multimedia);
        multimediaManager.CreateWindowRenderer(1280 , 720);
        multimediaManager.LoadTextures(textureType_t::WALLS,   {"wall_textures.bmp", 6, 110});
        multimediaManager.LoadTextures(textureType_t::GUARD,   {"guard.bmp", 8, 51});
        multimediaManager.LoadTextures(textureType_t::OBJECTS, {"objects.bmp", 5, 50});
        playerManager.Init(inputsBuffer, gameState, multimedia->windowParams.refreshRate);
        mapManager.Init(gameState, multimedia);
        mapManager.LoadMap("map.csv");
        doorManager.Init(gameState, multimedia->windowParams.refreshRate);
        inputParser.Init(inputsBuffer);
        renderer.Init(gameState, multimedia);

        // Set player location & view direction
        playerManager.SetPlayer(Point2(4.8, 28.1), Vec2(1, 1));
    }

    void Exit() {
        // Exit managers (that have memory clean-up to do)
        mapManager.Exit();
        multimediaManager.Exit();

        delete inputsBuffer;
        delete gameState;
        delete multimedia;
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

        running = inputsBuffer->AnyActiveInputs() || gameState->map.anyDoorsAwaitingRendering;
    }

public:
    InputsBuffer* inputsBuffer;

private:
    GameState*   gameState;
    Multimedia*  multimedia;

    MultimediaManager multimediaManager;
    PlayerManager playerManager;
    MapManager mapManager;
    DoorManager doorManager;
    InputParser inputParser;

    Renderer renderer;
};
