#pragma once

#include "../../Multimedia/Multimedia.h"
#include "../../State/GameState/GameState.h"

/*
=========================================================
    MapManager
=========================================================
*/

class MapManager {
private:
    GameState*   gameState;
    Multimedia*  multimedia;

public:
    void    Init                       (GameState* const _gameState, Multimedia* const _multimedia);
    void    Exit()                                                                                   const;
    void    LoadMap                    (const std::string file)                                      const;
    void    UpdateSpritePerpLines();

private:
    Point2  GetTileCenterPt            (const int index, const int mapWidth)                         const;
};
