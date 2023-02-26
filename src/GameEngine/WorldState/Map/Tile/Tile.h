#pragma once

#include "../../../Multimedia/Multimedia.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

enum class tileType_t {
    EMPTY,
    WALL,
    DOOR,
    OBJECT,
    ENEMY
};

/* WallTile return type related definitions */
struct textureSlice_t {
    textureSlice_t(SDL_Texture* const t, const SDL_Rect r);
    textureSlice_t();

    SDL_Texture*  texture;
    SDL_Rect      textureRect;
};

struct textureSliceDistPair_t {
    textureSliceDistPair_t(const textureSlice_t _textureSlice, const double _distance);

    textureSlice_t textureSlice;
    double         hitDistance;
};

/* SpriteTile return type definition */
struct textureVecToSpritePair_t {
    textureVecToSpritePair_t(SDL_Texture* const _texture, const Vec2& _vecToSprite);

    SDL_Texture* texture;
    Vec2         vecToSprite;
};

/* RayTileHit virtual function return type definition */
typedef std::optional<std::variant<textureSliceDistPair_t, textureVecToSpritePair_t>> rayTileHitVariant_o;

/*
=========================================================
    Tile base class (abstract)
=========================================================
*/

class Tile {
public:
    tileType_t type;

public:
    static   SDL_Texture*          LightTexture      (const texturePair_t texturePair, RayHitMarker& hitInfo);
    virtual                        ~Tile();
    virtual  rayTileHitVariant_o   RayTileHit        (RayHitMarker& hitInfo)                                   const = 0;
    virtual  bool                  PlayerTileHit()                                                             const = 0;
};
