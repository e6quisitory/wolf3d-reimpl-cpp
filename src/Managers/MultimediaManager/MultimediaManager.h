#pragma once

#include "../../GameData.h"

class MultimediaManager {
private:
    GameData* gameData;

public:
    void         Init                 (GameData* _gameData);
    void         Exit()                                                                                                                                    const;
    void         CreateWindowRenderer (const int& screenWidth, const int& screenHeight)                                 const;
    void         LoadTextures         (const textureType_t& texturesType, const spriteSheetParams_t& spriteSheetParams) const;

private:
    SDL_Surface* BmpToSurface         (const char* const fileName) const;
    SDL_Texture* ExtractTexture       (SDL_Surface* const textureSheet, const int& textureSheetPitch, int textureID) const;
};
