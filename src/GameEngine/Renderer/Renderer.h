#pragma once

#include "../WorldState/WorldState.h"
#include "../Multimedia/Multimedia.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

struct textureRenderData_t {
    textureRenderData_t(SDL_Texture* const _texture, const SDL_Rect _textureRect, const SDL_Rect _screenRect);
    textureRenderData_t();

    SDL_Texture* texture;
    SDL_Rect     textureRect;
    SDL_Rect     screenRect;
};

struct spriteRenderData_t {
    spriteRenderData_t(SpriteTile* const _spriteID, const textureRenderData_t _textureRenderData);
    spriteRenderData_t(SpriteTile* const _spriteID, SDL_Texture* const _texture, const SDL_Rect _screenRect);
    spriteRenderData_t();

    SpriteTile*               spriteID;
    textureRenderData_t       textureRenderData;
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

    std::vector  <std::pair<double, double>>         castingRayAngles;  // { angle, cosine(angle) }
    std::vector  <textureRenderData_t>               wallsRenderDataBuffer;
    std::vector  <std::vector<spriteRenderData_t>>   spritesRenderDataBuffers;

    std::vector  <spriteRenderData_t> allspritesbuffer;

    SDL_Texture* spritesBackBuffer;
    SDL_Texture* spritesBackBuffer_clear;

    std::vector<std::vector<bool>> spritesHitMap;

    const double fov = 72.0;
    texturePair_t gateSidewallTexture;

    SDL_Rect ceilingScreenRect;
    SDL_Rect floorScreenRect;

    const int cores = std::thread::hardware_concurrency();
    std::vector<int> startingPixels;
    std::vector<int> endingPixels;

public:
    void      Init                             (WorldState* const _worldState, Multimedia* const _multimedia);
    void      Exit()                                                                                                      const;
    void      RenderFrame();

private:
    void      DrawCeilingFloor()                                                                                          const;
    void      PartialRenderToBuffers           (const int startRayNum, const int endRayNum, const int renderSectionNum);         // Single-threaded
    void      FullRenderToBuffers();                                                                                             // Multithreaded
    void      DrawWalls()                                                                                                 const;
    void      RenderSpritesIntoBackBuffer()                                                                               const;
    void      DrawSprites();

    Ray       GetRay                           (const int rayNum)                                                         const;
    int       GetRenderHeight                  (const double perpHitDist)                                                 const;
    SDL_Rect  GetTextureSliceScreenRect        (const int renderHeight, int sliceNum)                                     const;
    SDL_Rect  GetFullTextureScreenRect         (const int renderHeight, int textureCenterX)                               const;

    void      CalculateCastingRayAngles();

    void RenderSpritesIntoBackBuffer();
};
