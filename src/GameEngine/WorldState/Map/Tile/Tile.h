#pragma once

#include "../../../Multimedia/Multimedia.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

struct textureSlice_t {
    textureSlice_t(SDL_Texture* const t, SDL_Rect r);
    textureSlice_t();

    SDL_Texture*  texture;
    SDL_Rect      textureRect;
};

typedef std::optional<std::pair<textureSlice_t, double>> textureSliceDistPair_o;

enum class tileType_t {
    EMPTY,
    WALL,
    DOOR,
    OBJECT,
    ENEMY
};

/*
=========================================================
    Tile base class (abstract)
=========================================================
*/

class Tile {
public:
    tileType_t type;
    iPoint2    coordinate;

public:
    virtual                          ~Tile();
    virtual textureSliceDistPair_o   RayTileHit        (RayHitMarker& hitInfo, const texturePair_o textureOverride) const = 0;
    virtual bool                     PlayerTileHit()                                                                const = 0;

protected:
    SDL_Texture*                     LightTexture      (const texturePair_t _texturePair, RayHitMarker& hitInfo)    const;
};
