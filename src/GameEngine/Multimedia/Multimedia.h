#pragma once

/*
=========================================================
    Relevant type definitions
=========================================================
*/

enum class textureType_t {
    WALLS,
    OBJECTS,
    GUARD,
    OFFICER,
    SS,
    WEAPONS
};

#define NUM_TEXTURE_TYPES 4

typedef std::pair<SDL_Texture*, SDL_Texture*> texturePair_t;

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
    bool mainWindowIsFullScreen;
    std::map<textureType_t, std::vector<SDL_Texture*>>  textures;

public:
    SDL_Texture*    GetTexture           (const textureType_t textureType, const int textureID)           const;
    void            AddTexture           (const textureType_t textureType, SDL_Texture* const texture);
    texturePair_t   GetWallTexturePair   (const int wallTextureID)                                        const;
};