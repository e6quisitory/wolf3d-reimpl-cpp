#pragma once

#include <string>

#include "../../Utilities/Vec2D.h"
#include "../../State/GameData/GameData.h"

class MapManager {
private:
    GameData* gameData;

public:
    void    Init                       (GameData* _gameData);
    void    Exit()                                                                const;
    void    LoadMap                    (const std::string& file)                  const;
    void    UpdateSpritePerpLines();

private:
    Point2  GetTileCenterPt            (const int& index, const int& mapWidth)   const;
};
