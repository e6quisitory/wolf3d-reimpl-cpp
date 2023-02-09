#pragma once

#include <string>
#include <cstring>

#include "InputsParser/InputsParser.h"
#include "Managers/PlayerManager/PlayerManager.h"
#include "Managers/MapManager/MapManager.h"
#include "Managers/MultimediaManager/MultimediaManager.h"
#include "Managers/DoorManager/DoorManager.h"
#include "Renderer/Renderer.h"

class GameEngine {
public:
    void Init() {
        // Allocate shared objects
        inputsBuffer = new InputsBuffer;
        gameState = new GameState;
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

        quitGameFlag = inputsBuffer->quitGameFlag;
    }

public:
    bool quitGameFlag;

private:
    InputsBuffer* inputsBuffer;
    GameState*   gameState;
    Multimedia*  multimedia;

    MultimediaManager multimediaManager;
    PlayerManager playerManager;
    MapManager mapManager;
    DoorManager doorManager;
    InputsParser inputParser;

    Renderer renderer;
};
