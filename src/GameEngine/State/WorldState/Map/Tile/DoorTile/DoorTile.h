#pragma once

#include "../Tile.h"

/*
===========================================
    Relevant type definitions
===========================================
*/

enum class doorStatus_t {
    CLOSED,
    OPENING,
    OPEN,
    CLOSING
};

enum class doorPosition_t {
    OPEN   = 0,
    CLOSED = 1
};

enum class doorTimerVal_t {
    NO_TIME_LEFT   = 0,
    FULL_TIME_LEFT = 1
};

/*
===========================================
    DoorTile
===========================================
*/

class DoorTile : public Tile {
public:
    doorStatus_t   doorStatus;
    double         doorPosition;
    double         doorTimerVal;

private:
    texturePair_t  gateTexture;
    texturePair_t  gateSidewallTexture;

public:
    DoorTile(const texturePairsPair_t doorTextures);

    virtual textureSliceDistPair_o  RayTileHit      (RayHitMarker& hitInfo, const texturePair_o textureOverride) const override;
    virtual bool                    PlayerTileHit()                                                              const override;
};