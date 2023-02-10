#include "Renderer.h"

#include <cmath>
#include <vector>
#include <utility>

#include "../Utilities/Ray/Ray.h"
#include "../State/GameState/GameState.h"
#include "../Utilities/DDA/DDA.h"
#include "../Utilities/Conventions.h"
#include "../Utilities/MiscMath/MiscMath.h"

/*
=========================================================
    Public methods
=========================================================
*/

void Renderer::Init(GameState* const _gameState, Multimedia* const _multimedia) {
    gameState = _gameState;
    multimedia = _multimedia;

    // Pre-calculate the ray angles and their cosines, as they do not change
    castingRayAngles.reserve(multimedia->windowParams.screenWidth);
    CalculateCastingRayAngles();

    // Pre-allocate spriteBackups (so that array does not need to be extended / reallocated)
    spriteBackups.reserve(multimedia->windowParams.screenWidth);
        // One sprite encountered per cast ray is a good initial assumption to allocate memory according to.
        // However, it's entirely possible to have a single cast ray encounter more than one sprite, in which case
        // sprite_backups vector may need to be extended.

    // Grab copy of gate sidewall texture (used for injecting into rayMarker when prev hit tile was a door tile
    gateSidewallTexture = multimedia->GetTexturePair(textureType_t::WALLS, 101);

    // These do not change, so calculate once in advance
    ceilingScreenRect = {0, 0, multimedia->windowParams.screenWidth, multimedia->windowParams.screenHeight / 2};
    floorScreenRect   = {0, multimedia->windowParams.screenHeight / 2, multimedia->windowParams.screenWidth, multimedia->windowParams.screenHeight / 2};

}

void Renderer::RenderFrame() {
    SDL_RenderClear(multimedia->sdlRenderer);
    DrawCeilingFloor();
    DrawWalls();
    DrawSprites();
    SDL_RenderPresent(multimedia->sdlRenderer);
}

/*
=========================================================
    Private methods
=========================================================
*/

void Renderer::DrawCeilingFloor() const {
    SDL_SetRenderDrawColor(multimedia->sdlRenderer, 50, 50, 50, 0 );
    SDL_RenderFillRect(multimedia->sdlRenderer, &ceilingScreenRect );

    SDL_SetRenderDrawColor(multimedia->sdlRenderer, 96, 96, 96, 0 );
    SDL_RenderFillRect(multimedia->sdlRenderer, &floorScreenRect );
}

void Renderer::DrawWalls() {
    for (int rayNum = 0; rayNum < multimedia->windowParams.screenWidth; ++rayNum) {
        HitInfo rayCursor(GetRay(rayNum));

        while (gameState->map.WithinMap(rayCursor.hitTile)) {
            Tile* prevTile = gameState->map.GetTile(rayCursor.hitTile);
                rayCursor.GoToNextHit();
            Tile* currTile = gameState->map.GetTile(rayCursor.hitTile);

            texturePair_o textureOverride;
            if (prevTile->tileType == tileType_t::DOOR)
                textureOverride = gateSidewallTexture;
            else
                textureOverride = std::nullopt;

            auto rayTileHitResult = currTile->RayTileHit(rayCursor, textureOverride);

            if (rayTileHitResult.has_value()) {
                auto [textureSlice, hitDistance] = rayTileHitResult.value();

                SDL_Rect textureRect  =  textureSlice.textureRect;
                int      renderHeight =  GetRenderHeight(hitDistance, castingRayAngles[rayNum].second);
                SDL_Rect screenRect   =  GetScreenRect(renderHeight, rayNum);

                // If sprite is hit, store the rendering info for later, as we first need to render the walls + doors behind the sprites.
                if (gameState->map.GetTile(rayCursor.hitTile)->tileType == tileType_t::SPRITE) {
                    spriteBackups.emplace_back(textureSlice, screenRect);
                    continue;
                } else {
                    // If wall or door is hit, render immediately.
                    SDL_RenderCopy(multimedia->sdlRenderer, textureSlice.texture, &textureRect, &screenRect);
                    break;
                }
            } else
                continue;
        }
    }
}

void Renderer::DrawSprites() {
    if (!spriteBackups.empty()) {
        // Must render sprites in reverse of the order in which they were encountered
        for (auto s = spriteBackups.rbegin(); s != spriteBackups.rend(); ++s) {
            auto [textureSlice, screenRect] = *s;
            SDL_RenderCopy(multimedia->sdlRenderer, textureSlice.texture, &(textureSlice.textureRect), &(screenRect));
        }
        spriteBackups.clear();
    }
}

Ray Renderer::GetRay(const int rayNum) const {
    return Ray(gameState->player.location, gameState->player.viewDir.Rotate(castingRayAngles[rayNum].first));
}

int Renderer::GetRenderHeight(const double hitDist, const double angleCosine) const {
    static double proportionalityConstant = 1.3 * multimedia->windowParams.screenWidth / ((16.0 / 9.0) * (fov / 72.0));
    return static_cast<int>(proportionalityConstant / (hitDist * angleCosine));
}

SDL_Rect Renderer::GetScreenRect(double renderHeight, int rayNum) const {
    return {rayNum, static_cast<int>(multimedia->windowParams.screenHeight / 2 - renderHeight / 2), 1, static_cast<int>(renderHeight)};
}

void Renderer::CalculateCastingRayAngles() {
    double projectionPlaneWidth = 2 * std::tan(DegreesToRadians(fov / 2));
    double segmentLength = projectionPlaneWidth / multimedia->windowParams.screenWidth;

    castingRayAngles.reserve(multimedia->windowParams.screenWidth);
    for (int i = 0; i < multimedia->windowParams.screenWidth; ++i) {
        double angle = std::atan(-(i * segmentLength - (projectionPlaneWidth / 2)));
        castingRayAngles.emplace_back(std::pair(angle, std::cos(angle)));
    }
}