#include "Renderer.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

textureRenderData_t::textureRenderData_t(SDL_Texture* const _texture, const SDL_Rect _textureRect, const SDL_Rect _screenRect) :
    texture(_texture), textureRect(_textureRect), screenRect(_screenRect) {}

textureRenderData_t::textureRenderData_t() {}

spriteRenderData_t::spriteRenderData_t(SpriteTile* const _spriteID, const textureRenderData_t _textureRenderData) :
    spriteID(_spriteID), textureRenderData(_textureRenderData) {}

spriteRenderData_t::spriteRenderData_t() {}

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
    gateSidewallTexture = multimedia->GetWallTexturePair(101);

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
        RayHitMarker rayHitMarker(GetRay(rayNum));

        while (worldState->map.WithinMap(rayHitMarker.hitTile)) {
            Tile* prevTile = worldState->map.GetTile(rayHitMarker.hitTile);
            rayHitMarker.GoToNextHit();
            Tile* currTile = worldState->map.GetTile(rayHitMarker.hitTile);

            auto rayTileHitResult = currTile->RayTileHit(rayHitMarker);
            if (rayTileHitResult.has_value()) {

                // WallTile hit
                if (std::holds_alternative<textureSliceDistPair_t>(rayTileHitResult.value())) {

                    auto& [textureSlice, hitDistance] = std::get<textureSliceDistPair_t>(rayTileHitResult.value());

                    // If prev tile type was a door and next is a wall, it means the next hit is a door sidewall ; thus, must swap texture
                    if (prevTile->type == tileType_t::DOOR && currTile->type == tileType_t::WALL)
                        textureSlice.texture = Tile::LightTexture(gateSidewallTexture, rayHitMarker);

                    wallBackbuffer[rayNum] = textureRenderData_t(textureSlice.texture, textureSlice.textureRect, GetScreenRect(GetRenderHeight(hitDistance, castingRayAngles[rayNum].second), rayNum));

                    break;

                  // SpriteTile hit
                } else {

                    auto& [texture, coordinate] = std::get<textureCoordinatePair_t>(rayTileHitResult.value());
                    SpriteTile* currSpriteID = static_cast<SpriteTile*>(currTile);

                    auto& currSpriteBackbuffer = spriteBackbuffers[renderSectionNum];

                    if (currSpriteBackbuffer.empty() || (!currSpriteBackbuffer.empty() && currSpriteBackbuffer.back().spriteID != currSpriteID)) {
                        Vec2 vecToSprite = coordinate - worldState->player.location;
                        double verticalHitDist = Dot(vecToSprite, worldState->player.viewDir);

                        double deg = AngleBetween(vecToSprite, worldState->player.viewDir)*(180.0/PI);


                        int horizontalHitDistDirection = Dot(vecToSprite, worldState->player.eastDir) > 0 ? 1 : -1;
                        int horizontalHitDist = (multimedia->windowParams.width/2) + horizontalHitDistDirection*((AngleBetween(vecToSprite, worldState->player.viewDir)*(180.0/PI)) / (fov/2))*(multimedia->windowParams.width/2);

                        int renderHeight = static_cast<int>((1.3 * multimedia->windowParams.width / ((16.0 / 9.0) * (fov / 72.0))) / verticalHitDist);
                        int renderWidth = renderHeight;

                        int textureMiddleScreenX = multimedia->windowParams.width/2 + horizontalHitDist;

                        SDL_Rect screenRect = {horizontalHitDist - renderWidth/2, multimedia->windowParams.height/2 - renderHeight/2, renderWidth, renderHeight};

                        spriteBackbuffers[renderSectionNum].emplace_back(spriteRenderData_t(currSpriteID, textureRenderData_t(texture, {0,0,TEXTURE_PITCH,TEXTURE_PITCH}, screenRect)));
                    }

                    continue;
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


    //PartialRender(startingPixels[0], endingPixels[0], 0);


}

void Renderer::FlipWallBackbuffer() const {
    for (const auto& w : wallBackbuffer) {
        const auto& [texture, textureRect, screenRect] = w;
        SDL_RenderCopy(multimedia->sdlRenderer, texture, &(textureRect), &(screenRect));
    }
}

void Renderer::FlipSpriteBackbuffers() {
    for (auto& sVec : spriteBackbuffers) {
        if (!sVec.empty()) {
            for (auto s = sVec.rbegin(); s != sVec.rend(); ++s) {
                const auto& [spriteID, textureRenderData] = *s;
                const auto& [spriteTexture, textureRect, screenRect] = textureRenderData;
                SDL_RenderCopy(multimedia->sdlRenderer, spriteTexture, &(textureRect), &(screenRect));
            }
            sVec.clear();
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