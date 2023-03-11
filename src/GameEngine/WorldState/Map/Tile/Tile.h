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
    OBJECT
};

/* Walls return type related definitions */
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

typedef std::optional<textureSliceDistPair_t> textureSliceDistPair_o;

/* Sprite return type related definitions */
struct textureCoordPair_t {
    textureCoordPair_t(SDL_Texture* const _texture, const Point2& _coordinate);

    SDL_Texture* texture;
    Point2       coordinate;
};

typedef std::vector<textureCoordPair_t>       textureCoordPairVec_t;
typedef std::optional<textureCoordPairVec_t>  textureCoordPairVec_o;

/* RayTileHit virtual function return type definition */
typedef std::pair<textureSliceDistPair_o, textureCoordPairVec_o> rayTileHitReturn_t;

/*
=========================================================
    Tile base class (abstract)
=========================================================
*/

class Tile {
public:
    tileType_t type;
    bool       enemyContainer;

public:
    static   SDL_Texture*          LightTexture      (const texturePair_t texturePair, RayHitMarker& hitInfo);
    virtual                        ~Tile();
    virtual  rayTileHitReturn_t    RayTileHit        (RayHitMarker& hitInfo)                                   const = 0;
    virtual  bool                  PlayerTileHit()                                                             const = 0;
};
