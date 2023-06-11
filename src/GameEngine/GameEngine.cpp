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
    multimediaManager.Init(multimedia, inputsBuffer);
    multimediaManager.CreateWindowRenderer(960, 480);  // custom window dimensions also supported e.g. CreateWindowRenderer(1280, 720)

    #if __APPLE__ && __MACH__
    auto parentDirPath      = executablePath.substr(0, executablePath.find_last_of("/\\"));
    auto grandParentDirPath = parentDirPath.substr(0, parentDirPath.find_last_of("/\\"));
    auto assetsDirPath      = grandParentDirPath + "/assets/";
    multimediaManager.LoadTextures(textureType_t::WALLS,   {assetsDirPath + "wall_textures.bmp", 6, 110});
    multimediaManager.LoadTextures(textureType_t::OBJECTS, {assetsDirPath + "objects.bmp", 5, 50});
    multimediaManager.LoadTextures(textureType_t::GUARD,   {assetsDirPath + "guard.bmp", 8, 51});
    multimediaManager.LoadTextures(textureType_t::OFFICER, {assetsDirPath + "officer.bmp", 8, 51});
    multimediaManager.LoadTextures(textureType_t::SS,      {assetsDirPath + "SS.bmp", 8, 51});
    #else
    multimediaManager.LoadTextures(textureType_t::WALLS,   {"../assets/wall_textures.bmp", 6, 110});
    multimediaManager.LoadTextures(textureType_t::OBJECTS, {"../assets/objects.bmp", 5, 50});
    multimediaManager.LoadTextures(textureType_t::GUARD,   {"../assets/guard.bmp", 8, 51});
    multimediaManager.LoadTextures(textureType_t::OFFICER, {"../assets/officer.bmp", 8, 51});
    multimediaManager.LoadTextures(textureType_t::SS,      {"../assets/SS.bmp", 8, 51});
    #endif

    playerManager.Init(inputsBuffer, worldState, multimedia->displayParams.refreshRate);
    mapManager.Init(worldState, multimedia);

    #if __APPLE__ && __MACH__ 
    mapManager.LoadMap(assetsDirPath + "map.csv");
    #else
    mapManager.LoadMap("../assets/map.csv");
    #endif

    doorManager.Init(worldState, multimedia->displayParams.refreshRate);
    inputsParser.Init(inputsBuffer, multimedia->sdlWindow);
    enemyManager.Init(multimedia);
    renderer.Init(worldState, multimedia);

    // Set player location & view direction
    playerManager.SetPlayer(Point2(2.5, 2.5), Vec2(0, 1));

    // Seed random number generator (used for setting enemy view dirs at launch)
    std::srand(std::time(nullptr));

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
    inputsParser.ParseInputs();
    quitGameFlag = inputsBuffer->quitGameFlag;
    playerManager.Update();
    doorManager.Update();
    multimediaManager.Update();
    renderer.RenderFrame();
}
