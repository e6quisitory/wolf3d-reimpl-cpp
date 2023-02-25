#pragma once

#include "InputsBuffer/InputsBuffer.h"
#include "InputsParser/InputsParser.h"
#include "Managers/PlayerManager/PlayerManager.h"
#include "Managers/MapManager/MapManager.h"
#include "Managers/MultimediaManager/MultimediaManager.h"
#include "Managers/DoorManager/DoorManager.h"
#include "Managers/EnemyManager/EnemyManager.h"
#include "Renderer/Renderer.h"

/*
=========================================================
    GameEngine
=========================================================
*/

class GameEngine {
public:
    bool quitGameFlag;
    WorldState*        worldState;
    Multimedia*        multimedia;

private:
    InputsBuffer*      inputsBuffer;

    InputsParser       inputsParser;
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
