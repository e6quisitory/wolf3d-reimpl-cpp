#pragma once

#include "../../State/GameState/GameState.h"
#include "../../State/InputsBuffer/InputsBuffer.h"

/*
=========================================================
    PlayerManager
=========================================================
*/

class PlayerManager {
private:
    InputsBuffer*  inputsBuffer;
    GameState*     gameState;

    double      movementIncrement;
    double      swivelIncrement;

private:
    enum class playerSpeed_t {
        FULL,
        HALF
    };

public:
    void        Init                   (InputsBuffer* const _inputsBuffer, GameState* const _gameState, const int _screenRefreshRate);
    void        SetPlayer              (const Point2& location, const Vec2& viewDir);
    void        Update()                                                                      const;

private:
    double      GetPlayerSpeedCoeff    (const playerSpeed_t playerSpeed)                      const;
    void        MoveX                  (const xDir_t xDir, const playerSpeed_t playerSpeed)   const;
    void        MoveY                  (const yDir_t yDir, const playerSpeed_t playerSpeed)   const;
    void        Swivel                 (const swivelDir_t swivelDir)                          const;
    void        MovePlayerIfValid      (const Point2& proposedLoc)                            const;
    Ray         GetPlayerViewDirRay()                                                         const;
    void        OpenDoor()                                                                    const;
};
