#include "Multimedia.h"

/*
=========================================================
    Public methods
=========================================================
*/

SDL_Texture* Multimedia::GetTexture(const textureType_t textureType, const int textureID) const {
    return textures.at(textureType)[textureID - 1];
}

void Multimedia::AddTexture(const textureType_t textureType, SDL_Texture* const texture) {
    textures[textureType].push_back(texture);
}

texturePair_t Multimedia::GetWallTexturePair(const int wallTextureID) const {
    static const std::array<int, 10> noLightingIDList = {31, 32, 41, 42, 43, 44, 107, 108, 109, 110};

    // Check if textureID is part of no lighting list ; if so, make pair have same texture
    for (const auto NoLightingID : noLightingIDList) {
        if (wallTextureID == NoLightingID) {
            SDL_Texture* t = GetTexture(textureType_t::WALLS, wallTextureID);
            return texturePair_t(t, t);
        }
    }

    // If not, put lit followed by unlit texture in pair
    SDL_Texture* litTexture   = GetTexture(textureType_t::WALLS, wallTextureID);
    SDL_Texture* unlitTexture = GetTexture(textureType_t::WALLS, wallTextureID + 1);

    return {litTexture, unlitTexture};
}