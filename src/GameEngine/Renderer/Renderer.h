#pragma once

#include "../WorldState/WorldState.h"
#include "../Multimedia/Multimedia.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

struct spriteRenderData_t {
    spriteRenderData_t(const int _screenX, SDL_Texture* const _texture, const int _renderHeight);
    spriteRenderData_t();

    int screenX;
    SDL_Texture* texture;
    int renderHeight;
};

/*
=========================================================
    Renderer
=========================================================
*/

class Renderer {
private:
    WorldState*  worldState;
    Multimedia*  multimedia;

    std::vector<std::pair<double, double>>  castingRayAngles;  // { angle, cosine(angle) }

    std::vector<textureSliceDistPair_t>     wallsReturnData;
    std::vector<int>                        wallRenderHeights;

    std::vector<std::vector<bool>>          spriteTilesHitMap;  // Marking hits by tile coord gives O(1) complexity
    std::vector<textureCoordinatePair_t>    spritesReturnData;
    std::vector<spriteRenderData_t>         spritesRenderData;

    const double fov = 72.0;
    texturePair_t gateSidewallTexture;

    SDL_Rect ceilingScreenRect;
    SDL_Rect floorScreenRect;

public:
    void      Init                             (WorldState* const _worldState, Multimedia* const _multimedia);
    void      RenderFrame();

private:
    void      DrawCeilingFloor()                                                                                 const;
    void      RenderToBuffers();
    void      DrawWalls();
    void      DrawSprites();

    Ray       GetRay                           (const int rayNum)                                                const;
    int       GetRenderHeight                  (const double perpHitDist)                                        const;
    SDL_Rect  GetTextureSliceScreenRect        (const int renderHeight, const int sliceNum)                      const;
    SDL_Rect  GetFullTextureScreenRect         (const int renderHeight, const int textureCenterX)                const;

    void      CalculateCastingRayAngles();
    void      ClearSpriteTilesHitMap();

};
