#pragma once

#include "../../State/GameState/GameState.h"
#include "../../Multimedia/Multimedia.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

struct spriteSheetParams_t {
    std::string  fileName;
    int          pitch;
    int          numTextures;
};

/*
=========================================================
    MultimediaManager
=========================================================
*/

class MultimediaManager {
private:
    Multimedia*  multimedia;

public:
    void         Init                 (Multimedia* const _multimedia);
    void         Exit()                                                                                                 const;
    void         CreateWindowRenderer (const int& screenWidth, const int& screenHeight)                                 const;
    void         LoadTextures         (const textureType_t& texturesType, const spriteSheetParams_t& spriteSheetParams) const;

private:
    SDL_Surface* BmpToSurface         (const char* const fileName) const;
    SDL_Texture* ExtractTexture       (SDL_Surface* const textureSheet, const int& textureSheetPitch, int textureID) const;
};
