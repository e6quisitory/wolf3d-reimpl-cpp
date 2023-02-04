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
#include "GameData.h"
#include "Utilities/DDA/DDA.h"
#include "Utilities/Conventions.h"
#include "Utilities/MiscMath/MiscMath.h"

class Renderer {
private:
    typedef std::pair<textureSlice_t, SDL_Rect> textureSliceScreenRectPair_t;

    GameData* gameData;
    std::vector<std::pair<double, double>> castingRayAngles;  // First element in each pair is the angle, second is the cosine of it
    std::vector<textureSliceScreenRectPair_t> spriteBackups;
    const double fov = 72.0;

    textureOverride_o gateSidewallTexture_o;

public:
    void Init(GameData* _game_data) {
        gameData = _game_data;

        // Pre-calculate the ray angles and their cosines, as they do not change
        CalculateCastingRayAngles();

        spriteBackups.reserve(gameData->multimedia.screenWidth);
            // One sprite encountered per casted ray is a good initial assumption to allocate memory according to.
            // However, it's entirely possible to have a single casted ray encounter more than one sprite, in which case
            // sprite_backups vector may need to be extended.

        gateSidewallTexture_o = gameData->multimedia.get_wall_texture_pair(101);
    }

    // Renders a single frame and outputs it to the window/screen
    void RenderFrame() {
        SDL_RenderClear(gameData->multimedia.sdlRenderer);
        DrawCeilingFloor();
        DrawWalls();
        DrawSprites();
        SDL_RenderPresent(gameData->multimedia.sdlRenderer);
    }

private:
    void DrawCeilingFloor() {
        SDL_Rect ceiling = {0, 0, gameData->multimedia.screenWidth, gameData->multimedia.screenHeight / 2};
        SDL_Rect floor = {0, gameData->multimedia.screenHeight / 2, gameData->multimedia.screenWidth, gameData->multimedia.screenHeight / 2};

        SDL_SetRenderDrawColor(gameData->multimedia.sdlRenderer, 50, 50, 50, 0 );
        SDL_RenderFillRect(gameData->multimedia.sdlRenderer, &ceiling );

        SDL_SetRenderDrawColor(gameData->multimedia.sdlRenderer, 96, 96, 96, 0 );
        SDL_RenderFillRect(gameData->multimedia.sdlRenderer, &floor );
    }

    void DrawWalls() {
        for (int i = 0; i < gameData->multimedia.screenWidth; ++i) {
            HitInfo rayCursor(GetRay(i));

            while (gameData->map.WithinMap(rayCursor.hitTile)) {
                Tile* prevTile = gameData->map.GetTile(rayCursor.hitTile);
                    rayCursor.GoToNextHit();
                Tile* nextTile = gameData->map.GetTile(rayCursor.hitTile);

                auto textureOverride = prevTile->tileType == tileType_t::DOOR ? gateSidewallTexture_o : std::nullopt;
                auto rayTileHitResult = nextTile->RayTileHit(rayCursor, textureOverride);

                if (rayTileHitResult.has_value()) {
                    auto [textureSlice, hitDistance] = rayTileHitResult.value();

                    SDL_Rect textureRect = textureSlice.textureRect;
                    int renderHeight = GetRenderHeight(hitDistance, castingRayAngles[i].second);
                    SDL_Rect screenRect = {i, gameData->multimedia.screenHeight / 2 - renderHeight / 2, 1, renderHeight};

                    // If sprite is hit, store the rendering info for later, as we first need to render the walls + doors behind the sprites.
                    if (gameData->map.GetTile(rayCursor.hitTile)->tileType == tileType_t::SPRITE) {
                        spriteBackups.push_back({textureSlice_t(textureSlice.texture, textureRect), screenRect});
                        continue;
                    } else {
                        // If wall or door is hit, render immediately.
                        SDL_RenderCopy(gameData->multimedia.sdlRenderer, textureSlice.texture, &textureRect, &screenRect);
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
                SDL_RenderCopy(gameData->multimedia.sdlRenderer, textureSlice.texture, &(textureSlice.textureRect), &(screenRect));
            }
            spriteBackups.clear();
        }
    }

    // Returns ray corresponding to a vertical column of pixels (0 is leftmost column of pixels)
    Ray GetRay(const int& ray_num) {
        return Ray(gameData->player.location, gameData->player.viewDir.Rotate(castingRayAngles[ray_num].first));
    }

    // Given hit distance and ray angle cosine, calculate how high the column of pixels to render should be
    int GetRenderHeight(const double& hit_dist, const double& angle_cosine) {
        static double coeff = 1.3 * gameData->multimedia.screenWidth / ((16.0 / 9.0) * (fov / 72.0));  // must account for screen aspect ratio
        return static_cast<int>(coeff/(hit_dist*angle_cosine));
    }

    void CalculateCastingRayAngles() {
        double proj_plane_width = 2*std::tan(DegreesToRadians(fov / 2));
        double segment_len = proj_plane_width / gameData->multimedia.screenWidth;

        castingRayAngles.reserve(gameData->multimedia.screenWidth);
        for (int i = 0; i < gameData->multimedia.screenWidth; ++i) {
            double angle = std::atan(-(i*segment_len-(proj_plane_width/2)));
            castingRayAngles.push_back({angle, std::cos(angle)});
        }
    }
};
