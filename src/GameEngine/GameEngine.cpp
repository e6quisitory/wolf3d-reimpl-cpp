#include "GameEngine.h"

/*
=========================================================
    Public Methods
=========================================================
*/

#if __APPLE__ && __MACH__
void GameEngine::Init(const std::string& executablePath) {
#else
void GameEngine::Init() {
#endif
    // Allocate shared objects
    inputsBuffer = new InputsBuffer;
    gameState    = new GameState;
    multimedia   = new Multimedia;

    // Initialize input parser, managers, and renderer
    multimediaManager.Init(multimedia);
    multimediaManager.CreateWindowRenderer();  // custom window dimensions also supported e.g. CreateWindowRenderer(1280, 720)

    #if __APPLE__ && __MACH__
    auto parentDirPath      = executablePath.substr(0, executablePath.find_last_of("/\\"));
    auto grandParentDirPath = parentDirPath.substr(0, parentDirPath.find_last_of("/\\"));
    auto assetsDirPath      = grandParentDirPath + "/assets/";
    multimediaManager.LoadTextures(textureType_t::WALLS,   {assetsDirPath + "wall_textures.bmp", 6, 110});
    multimediaManager.LoadTextures(textureType_t::GUARD,   {assetsDirPath + "guard.bmp", 8, 51});
    multimediaManager.LoadTextures(textureType_t::OBJECTS, {assetsDirPath + "objects.bmp", 5, 50});
    #else
    multimediaManager.LoadTextures(textureType_t::WALLS,   {"../assets/wall_textures.bmp", 6, 110});
    multimediaManager.LoadTextures(textureType_t::GUARD,   {"../assets/guard.bmp", 8, 51});
    multimediaManager.LoadTextures(textureType_t::OBJECTS, {"../assets/objects.bmp", 5, 50});
    #endif

    playerManager.Init(inputsBuffer, gameState, multimedia->windowParams.refreshRate);
    mapManager.Init(gameState, multimedia);

    #if __APPLE__ && __MACH__ 
    mapManager.LoadMap(assetsDirPath + "map.csv");
    #else
    mapManager.LoadMap("../assets/map.csv");
    #endif

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