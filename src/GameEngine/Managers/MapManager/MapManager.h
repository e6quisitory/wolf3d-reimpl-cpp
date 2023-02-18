#pragma once

#include "../../State/WorldState/WorldState.h"
#include "../../Multimedia/Multimedia.h"

/*
=========================================================
    MapManager
=========================================================
*/

class MapManager {
private:
    WorldState*  worldState;
    Multimedia*  multimedia;

public:
    void    Init                       (WorldState* const _worldState, Multimedia* const _multimedia);
    void    Exit()                                                                                      const;
    void    LoadMap                    (const std::string file)                                         const;
    void    UpdateSpritePerplines()                                                                     const;

private:
    Point2  GetTileCenterPt            (const int index, const int mapWidth)                            const;
};
