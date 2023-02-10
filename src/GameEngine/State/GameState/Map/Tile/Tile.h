#pragma once

#include <optional>
#include <SDL2/SDL.h>

#include "../../../../Utilities/Vec2D.h"
#include "../../../../Utilities/Ray/Ray.h"
#include "../../../../Utilities/DDA/DDA.h"
#include "../../../../Utilities/Conventions.h"

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

typedef std::optional   <std::pair<textureSlice_t, double>>      textureSliceDistPair_o;
typedef std::pair       <SDL_Texture*, SDL_Texture*>             texturePair_t;
typedef std::optional   <std::pair<SDL_Texture*, SDL_Texture*>>  texturePair_o;
typedef std::pair       <texturePair_t, texturePair_t>           texturePairsPair_t;

enum class tileType_t {
    EMPTY,
    WALL,
    DOOR,
    SPRITE
};

/*
=========================================================
    Tile base class (abstract)
=========================================================
*/

class Tile {
public:
    tileType_t tileType;

public:
    virtual                          ~Tile();
    virtual textureSliceDistPair_o   RayTileHit        (HitInfo& hitInfo, const texturePair_o textureOverride)  const = 0;
    virtual bool                     PlayerTileHit()                                                            const = 0;

protected:
    SDL_Texture*                     LightTexture      (const texturePair_t _texturePair, HitInfo& hitInfo)     const;
};
