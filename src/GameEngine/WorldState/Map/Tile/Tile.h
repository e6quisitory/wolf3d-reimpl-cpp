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
    textureSlice_t(SDL_Texture* const t, const int sliceNum);
    textureSlice_t(SDL_Texture* const t, const double widthPercent);
    textureSlice_t();

    SDL_Texture*  texture;
    SDL_Rect      textureRect;
};

struct textureSliceDistPair_t {
    textureSliceDistPair_t(const textureSlice_t _textureSlice, const double _distance);
    textureSliceDistPair_t();

    textureSlice_t textureSlice;
    double         hitDistance;
};

/* SpriteTile return type definition */
struct textureCoordinatePair_t {
    textureCoordinatePair_t(SDL_Texture* const _texture, const Point2& _coordinate);

    SDL_Texture* texture;
    Point2       coordinate;
};

/* RayTileHit virtual function return type definition */
typedef std::optional<std::variant<textureSliceDistPair_t, textureCoordinatePair_t>> rayTileHitVariant_o;

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
