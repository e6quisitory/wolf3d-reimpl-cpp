#pragma once

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
    void Init();
    void Exit();
    void GameLoop();
};
