#pragma once

#include "../WorldState/WorldState.h"
#include "../Multimedia/Multimedia.h"

/*
=========================================================
    Renderer
=========================================================
*/

class Renderer {
private:
    typedef std::pair<textureSlice_t, SDL_Rect> textureSliceScreenRectPair_t;

    WorldState*  worldState;
    Multimedia*  multimedia;

    std::vector  <std::pair<double, double>>                  castingRayAngles;  // { angle, cosine(angle) }
    std::vector  <textureSliceScreenRectPair_t>               wallBackbuffer;
    std::vector  <std::vector<textureSliceScreenRectPair_t>>  spriteBackbuffers;

    const double fov = 72.0;
    texturePair_t gateSidewallTexture;

    SDL_Rect ceilingScreenRect;
    SDL_Rect floorScreenRect;

    const int cores = std::thread::hardware_concurrency();
    std::vector<int> startingPixels;
    std::vector<int> endingPixels;

public:
    void      Init                           (WorldState* const _worldState, Multimedia* const _multimedia);
    void      RenderFrame();

private:
    void      DrawCeilingFloor()                                                                                       const;
    void      PartialRender                  (const int startRayNum, const int endRayNum, const int renderSectionNum);         // Single-threaded
    void      FullRender();                                                                                                    // Multithreaded
    void      FlipWallBackbuffer()                                                                                     const;
    void      FlipSpriteBackbuffers();

    Ray       GetRay                         (const int rayNum)                                                        const;
    int       GetRenderHeight                (const double hitDist, const double angleCosine)                          const;
    SDL_Rect  GetScreenRect                  (double renderHeight, int rayNum)                                         const;

    void      CalculateCastingRayAngles();
};
