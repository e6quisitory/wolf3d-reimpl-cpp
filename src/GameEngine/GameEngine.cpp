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
    worldState   = new WorldState;
    multimedia   = new Multimedia;

    // Initialize input parser, managers, and renderer
    multimediaManager.Init(multimedia);
    multimediaManager.CreateWindowRenderer(640, 480);  // custom window dimensions also supported e.g. CreateWindowRenderer(1280, 720)

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

    playerManager.Init(inputsBuffer, worldState, multimedia->displayParams.refreshRate);
    mapManager.Init(worldState, multimedia);

    #if __APPLE__ && __MACH__ 
    mapManager.LoadMap(assetsDirPath + "map.csv");
    #else
    mapManager.LoadMap("../assets/map.csv");
    #endif

    doorManager.Init(worldState, multimedia->displayParams.refreshRate);
    inputsParser.Init(inputsBuffer);
    renderer.Init(worldState, multimedia);

    // Set player location & view direction
    playerManager.SetPlayer(Point2(4.8, 28.1), Vec2(1, 1));

    quitGameFlag = false;
}

void GameEngine::Exit() {
    // Exit managers (that have memory clean-up to do)
    mapManager.Exit();
    multimediaManager.Exit();

    delete inputsBuffer;
    delete worldState;
    delete multimedia;
}

void GameEngine::GameLoop() {
    static bool running = true;

    if (running) {
        playerManager.Update();
        mapManager.UpdateSpritePerplines();
        renderer.RenderFrame();
    } else
        SDL_Delay(20);

    inputsParser.ParseInputs();
    doorManager.Update();

    running = inputsBuffer->AnyActiveInputs() || worldState->map.anyDoorsAwaitingRendering;

    quitGameFlag = inputsBuffer->quitGameFlag;
}