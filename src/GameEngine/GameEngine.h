#pragma once

#include "State/InputsBuffer/InputsBuffer.h"
#include "InputsParser/InputsParser.h"
#include "Managers/PlayerManager/PlayerManager.h"
#include "Managers/MapManager/MapManager.h"
#include "Managers/MultimediaManager/MultimediaManager.h"
#include "Managers/DoorManager/DoorManager.h"
#include "Renderer/Renderer.h"

/*
=========================================================
    GameEngine
=========================================================
*/

class GameEngine {
public:
    bool quitGameFlag;

private:
    InputsBuffer*      inputsBuffer;
    GameState*         gameState;
    Multimedia*        multimedia;

    InputsParser       inputParser;
    MultimediaManager  multimediaManager;
    PlayerManager      playerManager;
    MapManager         mapManager;
    DoorManager        doorManager;

    Renderer           renderer;

public:
    #if __APPLE__ && __MACH__
    void Init(const std::string& executablePath);
    #else
    void Init();
    #endif
    void Exit();
    void GameLoop();
};
