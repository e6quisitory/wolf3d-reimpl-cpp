#pragma once

#include "../../GameData.h"
#include "../../Inputs.h"

class PlayerManager {
private:
    Inputs*     inputs;
    GameData*   gameData;

    double      movementIncrement;
    double      swivelIncrement;

private:
    enum class  playerSpeed_t : int {
        FULL,
        HALF
    };

public:
    void        Init                   (Inputs* _inputs, GameData* _gameData);
    void        SetPlayer              (const Point2& location, const Vec2& viewDir);
    void        Update()                                                               const;

private:
    double      GetPlayerSpeedCoeff    (playerSpeed_t playerSpeed)                     const;
    void        MoveX                  (xDir_t xDir, playerSpeed_t playerSpeed)        const;
    void        MoveY                  (yDir_t yDir, playerSpeed_t playerSpeed)        const;
    void        Swivel                 (swivelDir_t swivelDir)                         const;
    void        MovePlayerIfValid      (const Point2& proposedLoc)                     const;
    Ray         GetPlayerViewDirRay()                                                  const;
    void        OpenDoor()                                                             const;
};
