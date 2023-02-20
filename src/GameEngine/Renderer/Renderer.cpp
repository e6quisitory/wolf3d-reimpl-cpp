#include "Renderer.h"

/*
=========================================================
    Public methods
=========================================================
*/

void Renderer::Init(WorldState* const _worldState, Multimedia* const _multimedia) {
    worldState = _worldState;
    multimedia = _multimedia;

    // Pre-calculate the ray angles and their cosines, as they do not change
    castingRayAngles.reserve(multimedia->windowParams.width);
    CalculateCastingRayAngles();

    // Grab copy of gate sidewall texture (used for injecting into rayMarker when prev hit tile was a door tile
    gateSidewallTexture = multimedia->GetTexturePair(textureType_t::WALLS, 101);

    // These do not change, so calculate once in advance
    ceilingScreenRect = {0, 0, multimedia->windowParams.width, multimedia->windowParams.height / 2};
    floorScreenRect   = {0, multimedia->windowParams.height / 2, multimedia->windowParams.width, multimedia->windowParams.height / 2};

    // Multithreaded render related
    assert(multimedia->windowParams.width % cores == 0);
    int renderSecWidth = multimedia->windowParams.width / cores;
    for (int i = 0; i < cores; ++i) {
        startingPixels.push_back(i*renderSecWidth);
        endingPixels.push_back(startingPixels[i] + renderSecWidth);
    }
    wallBackbuffer.resize(multimedia->windowParams.width);
    spriteBackbuffers.resize(cores);

    // Pre-allocate sprite back buffers (rough estimate)
    for (int i = 0; i < cores; ++i)
        spriteBackbuffers[i].reserve(renderSecWidth);
}

void Renderer::RenderFrame() {
    SDL_RenderClear(multimedia->sdlRenderer);
    DrawCeilingFloor();
    FullRender();
    FlipWallBackbuffer();
    FlipSpriteBackbuffers();
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

void Renderer::PartialRender(const int startRayNum, const int endRayNum, const int renderSectionNum) {
    for (int rayNum = startRayNum; rayNum < endRayNum; ++rayNum) {
        RayHitMarker rayCursor(GetRay(rayNum));

        while (worldState->map.WithinMap(rayCursor.hitTile)) {
            Tile* prevTile = worldState->map.GetTile(rayCursor.hitTile);
                rayCursor.GoToNextHit();
            Tile* currTile = worldState->map.GetTile(rayCursor.hitTile);

            texturePair_o textureOverride;
            if (prevTile->type == tileType_t::DOOR)
                textureOverride = gateSidewallTexture;
            else
                textureOverride = std::nullopt;

            auto rayTileHitResult = currTile->RayTileHit(rayCursor, textureOverride);
            if (rayTileHitResult.has_value()) {
                auto [textureSlice, hitDistance] = rayTileHitResult.value();

                SDL_Rect textureRect = textureSlice.textureRect;
                int     renderHeight = GetRenderHeight(hitDistance, castingRayAngles[rayNum].second);
                SDL_Rect screenRect  = GetScreenRect(renderHeight, rayNum);

                if (worldState->map.GetTile(rayCursor.hitTile)->type == tileType_t::SPRITE) {
                    spriteBackbuffers[renderSectionNum].emplace_back(std::pair(textureSlice, screenRect));
                    continue;
                } else {
                    wallBackbuffer[rayNum] = std::pair(textureSlice, screenRect);
                    break;
                }
            } else
                continue;
        }
    }
}

void Renderer::FullRender() {
    std::thread renderThreads[cores];

    // Launch a rendering thread for each CPU core
    for (int i = 0; i < cores; ++i)
        renderThreads[i] = std::thread(&Renderer::PartialRender, this, startingPixels[i], endingPixels[i], i);

    // Wait for all threads to finish rendering their sections
    for (int i = 0; i < cores; ++i)
        renderThreads[i].join();
}

void Renderer::FlipWallBackbuffer() const {
    for (const auto& w : wallBackbuffer) {
        const auto& [textureSlice, screenRect] = w;
        SDL_RenderCopy(multimedia->sdlRenderer, textureSlice.texture, &(textureSlice.textureRect), &(screenRect));
    }
}

void Renderer::FlipSpriteBackbuffers() {
    for (int i = 0; i < cores; ++i) {
        // Flip sprite back buffers
        if (!spriteBackbuffers[i].empty()) {
            // Must render sprites in reverse of the order in which they were encountered
            for (auto s = spriteBackbuffers[i].rbegin(); s != spriteBackbuffers[i].rend(); ++s) {
                const auto& [textureSlice, screenRect] = *s;
                SDL_RenderCopy(multimedia->sdlRenderer, textureSlice.texture, &(textureSlice.textureRect), &(screenRect));
            }
            spriteBackbuffers[i].clear();
        }
    }
}

Ray Renderer::GetRay(const int rayNum) const {
    return Ray(worldState->player.location, worldState->player.viewDir.Rotate(castingRayAngles[rayNum].first));
}

int Renderer::GetRenderHeight(const double hitDist, const double angleCosine) const {
    static const double proportionalityConstant = 1.3 * multimedia->windowParams.width / ((16.0 / 9.0) * (fov / 72.0));
    return static_cast<int>(proportionalityConstant / (hitDist * angleCosine));
}

SDL_Rect Renderer::GetScreenRect(double renderHeight, int rayNum) const {
    return {rayNum, static_cast<int>(multimedia->windowParams.height / 2 - renderHeight / 2), 1, static_cast<int>(renderHeight)};
}

void Renderer::CalculateCastingRayAngles() {
    double projectionPlaneWidth = 2 * std::tan(DegreesToRadians(fov / 2));
    double segmentLength = projectionPlaneWidth / multimedia->windowParams.width;

    castingRayAngles.reserve(multimedia->windowParams.width);
    for (int i = 0; i < multimedia->windowParams.width; ++i) {
        double angle = std::atan(-(i * segmentLength - (projectionPlaneWidth / 2)));
        castingRayAngles.emplace_back(std::pair(angle, std::cos(angle)));
    }
}