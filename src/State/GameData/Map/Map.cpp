#include "Map.h"

/*
================================
    Operators
================================
*/

Tile* Map::operator [] (const int& tileIndex) const {
    return tiles[tileIndex];
}

Tile* Map::operator () (const int& tileCoordX, const int& tileCoordY) const {
    return tiles[tileCoordY * mapWidth + tileCoordX];
}

/*
================================
    Public Methods
================================
*/

Tile* Map::GetTile(const iPoint2& tileCoord) const {
    return (*this)(tileCoord.x(), tileCoord.y());
}

bool Map::WithinMap(const iPoint2& tileCoord) const {
    if((tileCoord.x() >= 0 && tileCoord.x() <= mapWidth) && (tileCoord.y() >= 0 && tileCoord.y() <= mapHeight))
        return true;
    else
        return false;
}

void Map::AddActiveDoor(DoorTile* const door) {
    activeDoors.insert({door, door});
}

void Map::RemoveActiveDoor(DoorTile* const door) {
    activeDoors.erase(door);
}

std::optional<DoorTile*> Map::GetLoneActiveDoor() const {
    if (activeDoors.size() == 1)
        return activeDoors.begin()->first;
    else
        return std::nullopt;
}