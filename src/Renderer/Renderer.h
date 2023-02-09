#pragma once

#include <vector>
#include <utility>

#include "../State/GameState/GameState.h"
#include "../Multimedia/Multimedia.h"
#include "../State/GameState/Map/Tile/Tile.h"

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
    void Init            (GameState* const _gameState, Multimedia* const _multimedia);
    void RenderFrame();

private:
    void      DrawCeilingFloor()                                                                   const;
    void      DrawWalls();
    void      DrawSprites();

    Ray       GetRay                         (const int& ray_num)                                  const;
    int       GetRenderHeight                (const double& hit_dist, const double& angle_cosine)  const;
    SDL_Rect  GetScreenRect                  (double renderHeight, int rayNum)                     const;

    void      CalculateCastingRayAngles();
};
