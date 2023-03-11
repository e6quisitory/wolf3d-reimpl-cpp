#include "Tile.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

/* WallTile return type related definitions */
textureSlice_t::textureSlice_t(SDL_Texture *const t, const int sliceNum): texture(t) {
    textureRect = {sliceNum, 0, 1, TEXTURE_PITCH};
}

textureSlice_t::textureSlice_t(SDL_Texture *const t, const double widthPercent): texture(t) {
    textureRect = {static_cast<int>(widthPercent*TEXTURE_PITCH), 0, 1, TEXTURE_PITCH};
}

textureSlice_t::textureSlice_t() {}

textureSliceDistPair_t::textureSliceDistPair_t(const textureSlice_t _textureSlice, const double _distance) :
    textureSlice(_textureSlice), hitDistance(_distance) {}

textureSliceDistPair_t::textureSliceDistPair_t() {}

/* SpriteTile return type definition */
textureCoordPair_t::textureCoordPair_t(SDL_Texture* const _texture, const Point2& _coordinate) :
    texture(_texture), coordinate(_coordinate) {}

/*
=========================================================
    Static methods
=========================================================
*/

SDL_Texture* Tile::LightTexture(const texturePair_t texturePair, RayHitMarker& hitInfo) {
    if (hitInfo.GetWallType() == wallType_t::VERTICAL)
        return texturePair.first;
    else
        return texturePair.second;
}

/*
=========================================================
    Destructors (virtual)
=========================================================
*/

Tile::~Tile() {};