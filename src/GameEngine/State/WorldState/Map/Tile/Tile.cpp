#include "Tile.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

textureSlice_t::textureSlice_t(SDL_Texture *const t, SDL_Rect r): texture(t), textureRect(r) {}
textureSlice_t::textureSlice_t() {}

/*
=========================================================
    Destructors (virtual)
=========================================================
*/

Tile::~Tile() {};

/*
=========================================================
    Protected methods
=========================================================
*/

SDL_Texture* Tile::LightTexture(const texturePair_t _texturePair, RayHitMarker& hitInfo) const {
    if (hitInfo.GetWallType() == wallType_t::VERTICAL)
        return _texturePair.first;
    else
        return _texturePair.second;
}