#pragma once

#include "../../State/GameState/GameState.h"
#include "../../Multimedia/Multimedia.h"

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
    void    UpdateSpritePerpLines()                                                                  const;

private:
    Point2  GetTileCenterPt            (const int index, const int mapWidth)                         const;
};
