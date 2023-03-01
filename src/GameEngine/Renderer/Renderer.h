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

    std::vector<std::pair<double, double>> castingRayAngles;  // { angle, cosine(angle) }

    //std::vector<std::vector<bool>> spriteTilesHitMap;

    bool** spriteTilesHitMap;

    //std::vector<std::vector<std::atomic<bool>>> spritesHitMap;

    std::vector<textureSliceDistPair_t> wallHitReturnDataVec;
    std::vector<textureCoordinatePair_t> spriteHitReturnDataVec;

    std::vector<int> wallRenderHeightVec;
    std::vector<spriteRenderData_t> spriteRenderDataVec;

    std::vector<int> spritesRenderHeightVec;
    SDL_Texture* spritesBackBuffer;
    SDL_Texture* spritesBackBuffer_clear;

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
    void      PartialRenderToBuffers           (const int startRayNum, const int endRayNum);        // Single-threaded
    void      FullRenderToBuffers();                                                                // Multithreaded
    void      DrawWalls();
    void      DrawSprites();


    Ray       GetRay                           (const int rayNum)                                                         const;
    int       GetRenderHeight                  (const double perpHitDist)                                                 const;
    SDL_Rect  GetTextureSliceScreenRect        (const int renderHeight, const int sliceNum)                                     const;
    SDL_Rect  GetFullTextureScreenRect         (const int renderHeight, const int textureCenterX)                               const;

    void      CalculateCastingRayAngles();

    void FillSpriteRenderHeights(const int index, const int renderHeight);

};
