#include <string>
#include <cstring>

#include "GameEngine.h"

void GameEngine::Init() {
    // Allocate shared objects
    inputsBuffer = new InputsBuffer;
    gameState    = new GameState;
    multimedia   = new Multimedia;

    // Initialize input parser, managers, and renderer
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

void GameEngine::Exit() {
    // Exit managers (that have memory clean-up to do)
    mapManager.Exit();
    multimediaManager.Exit();

    delete inputsBuffer;
    delete gameState;
    delete multimedia;
}

void GameEngine::GameLoop() {
    static bool running = true;

    if (running) {
        playerManager.Update();
        mapManager.UpdateSpritePerpLines();
        renderer.RenderFrame();
    } else
        SDL_Delay(20);

    inputParser.ParseInputs();
    doorManager.Update();

    running = inputsBuffer->AnyActiveInputs() || gameState->map.anyDoorsAwaitingRendering;

    quitGameFlag = inputsBuffer->quitGameFlag;
}