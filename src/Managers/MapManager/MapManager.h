#pragma once

#include "../../GameData.h"

class MapManager {
private:
    GameData* gameData;

public:
    void    Init                       (GameData* _gameData);
    void    Exit()                                                                const;
    void    LoadMap                    (const std::string& file)                  const;
    void    UpdateSpritePerpLines();

private:
    Point2  GetTileCenterPt            (const int& index, const int& map_width)   const;
};
