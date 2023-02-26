#include "Tile.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

/* WallTile return type related definitions */
textureSlice_t::textureSlice_t(SDL_Texture *const t, SDL_Rect r): texture(t), textureRect(r) {}
textureSlice_t::textureSlice_t() {}

textureSliceDistPair_t::textureSliceDistPair_t(const textureSlice_t _textureSlice, const double _distance) :
    textureSlice(_textureSlice), hitDistance(_distance) {}

/* SpriteTile return type definition */
textureVecToSpritePair_t::textureVecToSpritePair_t(SDL_Texture* const _texture, const Vec2& _vecToSprite) :
    texture(_texture), vecToSprite(_vecToSprite) {}

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