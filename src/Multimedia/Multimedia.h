#pragma once

#include <map>
#include <SDL2/SDL.h>

#include "../Tile.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

enum class textureType_t {
    WALLS,
    GUARD,
    OBJECTS,
    WEAPONS,
};

#define NUM_TEXTURE_TYPES 4

struct WindowParams {
    int screenWidth;
    int screenHeight;
    int refreshRate;
};

/*
=========================================================
    Multimedia (state)
=========================================================
*/

class Multimedia {
public:
    SDL_Window*     sdlWindow;
    WindowParams    windowParams;
    SDL_Renderer*   sdlRenderer;
    std::map<textureType_t, std::vector<SDL_Texture*>>  textures;

public:
    SDL_Texture*    GetTexture           (const textureType_t& textureType, const int& textureID)         const;
    void            AddTexture           (const textureType_t& textureType, SDL_Texture* const texture);
    texturePair_t   GetTexturePair       (const textureType_t& textureType, const int& textureID)         const;  // Temporary ; will get removed
    texturePair_t   GetWallTexturePair   (int textureID)                                                  const;  // Performs lighting
};