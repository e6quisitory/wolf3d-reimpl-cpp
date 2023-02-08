/*
 * renderer.h:
 *
 * Uses game_data struct to turn the game world from a data representation into a graphical representation, to be outputted to the screen.
 *
 */

#pragma once

#include <cmath>
#include <vector>
#include <utility>

#include "Utilities/Ray/Ray.h"
#include "State/GameState/GameState.h"
#include "Utilities/DDA/DDA.h"
#include "Utilities/Conventions.h"
#include "Utilities/MiscMath/MiscMath.h"

class Renderer {
private:
    typedef std::pair<textureSlice_t, SDL_Rect> textureSliceScreenRectPair_t;

    GameState*   gameState;
    Multimedia*  multimedia; 
    std::vector<std::pair<double, double>> castingRayAngles;  // First element in each pair is the angle, second is the cosine of it
    std::vector<textureSliceScreenRectPair_t> spriteBackups;
    const double fov = 72.0;

    texturePair_t gateSidewallTexture;

public:
    void Init(GameState* const _gameState, Multimedia* const _multimedia) {
        gameState = _gameState;
        multimedia = _multimedia;

        // Pre-calculate the ray angles and their cosines, as they do not change
        castingRayAngles.reserve(multimedia->windowParams.screenWidth);
        CalculateCastingRayAngles();

        spriteBackups.reserve(multimedia->windowParams.screenWidth);
            // One sprite encountered per casted ray is a good initial assumption to allocate memory according to.
            // However, it's entirely possible to have a single casted ray encounter more than one sprite, in which case
            // sprite_backups vector may need to be extended.

        gateSidewallTexture = multimedia->GetTexturePair(textureType_t::WALLS, 101);
    }

    // Renders a single frame and outputs it to the window/screen
    void RenderFrame() {
        SDL_RenderClear(multimedia->sdlRenderer);
        DrawCeilingFloor();
        DrawWalls();
        DrawSprites();
        SDL_RenderPresent(multimedia->sdlRenderer);
    }

private:
    void DrawCeilingFloor() {
        SDL_Rect ceiling = {0, 0, multimedia->windowParams.screenWidth, multimedia->windowParams.screenHeight / 2};
        SDL_Rect floor = {0, multimedia->windowParams.screenHeight / 2, multimedia->windowParams.screenWidth, multimedia->windowParams.screenHeight / 2};

        SDL_SetRenderDrawColor(multimedia->sdlRenderer, 50, 50, 50, 0 );
        SDL_RenderFillRect(multimedia->sdlRenderer, &ceiling );

        SDL_SetRenderDrawColor(multimedia->sdlRenderer, 96, 96, 96, 0 );
        SDL_RenderFillRect(multimedia->sdlRenderer, &floor );
    }

    void DrawWalls() {
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
                        spriteBackups.emplace_back(std::pair(textureSlice_t(textureSlice.texture, textureRect), screenRect));
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

    void DrawSprites() {
        if (!spriteBackups.empty()) {
            // Must render sprites in reverse of the order in which they were encountered
            for (auto s = spriteBackups.rbegin(); s != spriteBackups.rend(); ++s) {
                auto [textureSlice, screenRect] = *s;
                SDL_RenderCopy(multimedia->sdlRenderer, textureSlice.texture, &(textureSlice.textureRect), &(screenRect));
            }
            spriteBackups.clear();
        }
    }

    // Returns ray corresponding to a vertical column of pixels (0 is leftmost column of pixels)
    Ray GetRay(const int& ray_num) const {
        return Ray(gameState->player.location, gameState->player.viewDir.Rotate(castingRayAngles[ray_num].first));
    }

    // Given hit distance and ray angle cosine, calculate how high the column of pixels to render should be
    int GetRenderHeight(const double& hit_dist, const double& angle_cosine) const {
        static double coeff = 1.3 * multimedia->windowParams.screenWidth / ((16.0 / 9.0) * (fov / 72.0));  // must account for screen aspect ratio
        return static_cast<int>(coeff/(hit_dist*angle_cosine));
    }

    SDL_Rect GetScreenRect(double renderHeight, int rayNum) const {
        SDL_Rect screenRect = {rayNum, static_cast<int>(multimedia->windowParams.screenHeight / 2 - renderHeight / 2), 1, static_cast<int>(renderHeight)};
        return screenRect;
    }

    void CalculateCastingRayAngles() {
        double proj_plane_width = 2*std::tan(DegreesToRadians(fov / 2));
        double segment_len = proj_plane_width / multimedia->windowParams.screenWidth;

        castingRayAngles.reserve(multimedia->windowParams.screenWidth);
        for (int i = 0; i < multimedia->windowParams.screenWidth; ++i) {
            double angle = std::atan(-(i*segment_len-(proj_plane_width/2)));
            castingRayAngles.emplace_back(std::pair(angle, std::cos(angle)));
        }
    }
};
