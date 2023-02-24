#include "Map.h"

/*
================================
    Public Methods
================================
*/

Tile* Map::GetTile(const iPoint2& tileCoord) const {
    return tiles[tileCoord.x()][tileCoord.y()];
}

void Map::SetTile(const iPoint2 &tileCoord, Tile *const tile) {
    tiles[tileCoord.x()][tileCoord.y()] = tile;
}

bool Map::WithinMap(const iPoint2& tileCoord) const {
    if((tileCoord.x() >= 0 && tileCoord.x() <= width) && (tileCoord.y() >= 0 && tileCoord.y() <= height))
        return true;
    else
        return false;
}

void Map::AddActiveDoor(Door* const door) {
    activeDoors.insert({door, door});
}

void Map::RemoveActiveDoor(Door* const door) {
    activeDoors.erase(door);
}

std::optional<Door*> Map::GetLoneActiveDoor() const {
    if (activeDoors.size() == 1)
        return activeDoors.begin()->first;
    else
        return std::nullopt;
}