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

struct WindowParams {
    int width;
    int height;
};

struct DisplayParams {
    int width;
    int height;
    int refreshRate;
};

/*
=========================================================
    Multimedia
=========================================================
*/

class Multimedia {
public:
    DisplayParams displayParams;
    WindowParams  windowParams;
    SDL_Window*   sdlWindow;
    SDL_Renderer* sdlRenderer;
    std::map<textureType_t, std::vector<SDL_Texture*>>  textures;

public:
    SDL_Texture*    GetTexture           (const textureType_t textureType, const int textureID)           const;
    void            AddTexture           (const textureType_t textureType, SDL_Texture* const texture);
    texturePair_t   GetTexturePair       (const textureType_t textureType, const int textureID)           const;  // Temporary ; will get removed
    texturePair_t   GetWallTexturePair   (const int textureID)                                            const;  // Performs lighting
};