#pragma once

#include "../State/GameState/GameState.h"
#include "../Multimedia/Multimedia.h"

/*
=========================================================
    Renderer
=========================================================
*/

class Renderer {
private:
    typedef std::pair<textureSlice_t, SDL_Rect> textureSliceScreenRectPair_t;

    GameState*     gameState;
    Multimedia*    multimedia;

    std::vector    <std::pair<double, double>>     castingRayAngles;  // {angle, cosine(angle)}
    std::vector    <textureSliceScreenRectPair_t>  spriteBackups;

    const double   fov = 72.0;
    texturePair_t  gateSidewallTexture;

    SDL_Rect ceilingScreenRect;
    SDL_Rect floorScreenRect;

public:
    void      Init                           (GameState* const _gameState, Multimedia* const _multimedia);
    void      RenderFrame();

private:
    void      DrawCeilingFloor()                                                                              const;
    void      DrawWalls();
    void      DrawSprites();

    Ray       GetRay                         (const int rayNum)                                               const;
    int       GetRenderHeight                (const double hitDist, const double angleCosine)                 const;
    SDL_Rect  GetScreenRect                  (double renderHeight, int rayNum)                                const;

    void      CalculateCastingRayAngles();
};
