#pragma once

/*
=========================================================
    Relevant type definitions
=========================================================
*/

enum class textureType_t {
    WALLS,
    GUARD,
    OBJECTS,
    WEAPONS
};

#define NUM_TEXTURE_TYPES 4

typedef std::pair      <SDL_Texture*, SDL_Texture*>             texturePair_t;
typedef std::optional  <std::pair<SDL_Texture*, SDL_Texture*>>  texturePair_o;
typedef std::pair      <texturePair_t, texturePair_t>           texturePairsPair_t;

struct WindowParams {
    int screenWidth;
    int screenHeight;
    int refreshRate;
};

/*
=========================================================
    Multimedia
=========================================================
*/

class Multimedia {
public:
    SDL_Window*     sdlWindow;
    WindowParams    windowParams;
    SDL_Renderer*   sdlRenderer;
    std::map<textureType_t, std::vector<SDL_Texture*>>  textures;

public:
    SDL_Texture*    GetTexture           (const textureType_t textureType, const int textureID)           const;
    void            AddTexture           (const textureType_t textureType, SDL_Texture* const texture);
    texturePair_t   GetTexturePair       (const textureType_t textureType, const int textureID)           const;  // Temporary ; will get removed
    texturePair_t   GetWallTexturePair   (const int textureID)                                            const;  // Performs lighting
};