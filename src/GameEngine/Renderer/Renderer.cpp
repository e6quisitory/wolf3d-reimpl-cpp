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

spriteRenderData_t::spriteRenderData_t(SpriteTile* const _spriteID, SDL_Texture* const _texture, const SDL_Rect _screenRect) {
    spriteID = _spriteID;
    textureRenderData = textureRenderData_t(_texture, {0,0,TEXTURE_PITCH, TEXTURE_PITCH}, _screenRect);
}

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
    wallsRenderDataBuffer.resize(multimedia->windowParams.width);
    spritesRenderDataBuffers.resize(cores);

    // Sprites back buffer (for rendering into before merging with already-drawn walls on screen)
    spritesBackBuffer       = SDL_CreateTexture(multimedia->sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, multimedia->windowParams.width, multimedia->windowParams.height);
    spritesBackBuffer_clear = SDL_CreateTexture(multimedia->sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, multimedia->windowParams.width, multimedia->windowParams.height);
    SDL_SetTextureBlendMode(spritesBackBuffer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(spritesBackBuffer_clear, SDL_BLENDMODE_BLEND);

    spritesHitMap.resize(worldState->map.width);
    for (auto& column : spritesHitMap) {
        column.resize(worldState->map.height);
        std::fill(column.begin(), column.end(), false);
    }
}

void Renderer::Exit() const {
    SDL_DestroyTexture(spritesBackBuffer);
    SDL_DestroyTexture(spritesBackBuffer_clear);
}

void Renderer::RenderFrame() {
    SDL_RenderClear(multimedia->sdlRenderer);
    DrawCeilingFloor();
    FullRenderToBuffers();
    RenderSpritesIntoBackBuffer();
    DrawWalls();
    DrawSprites();
    //SDL_RenderPresent(multimedia->sdlRenderer);
}

/*
=========================================================
    Private methods
=========================================================
*/

void Renderer::DrawCeilingFloor() const {
    SDL_SetRenderTarget(multimedia->sdlRenderer, nullptr);

    SDL_SetRenderDrawColor(multimedia->sdlRenderer, 50, 50, 50, 0 );
    SDL_RenderFillRect(multimedia->sdlRenderer, &ceilingScreenRect );

    SDL_SetRenderDrawColor(multimedia->sdlRenderer, 96, 96, 96, 0 );
    SDL_RenderFillRect(multimedia->sdlRenderer, &floorScreenRect );
}

void Renderer::PartialRenderToBuffers(const int startRayNum, const int endRayNum, const int renderSectionNum) {
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

                    double    renderHeight = GetRenderHeight(hitDistance*castingRayAngles[rayNum].second);
                    SDL_Rect  screenRect  = GetTextureSliceScreenRect(renderHeight, rayNum);

                    wallsRenderDataBuffer[rayNum] = textureRenderData_t(textureSlice.texture, textureSlice.textureRect, screenRect);

                    break;

                  // SpriteTile hit
                } else {
                    SpriteTile* currSpriteID = static_cast<SpriteTile*>(currTile);
                    auto& currSpriteTileCoord = currSpriteID->tileCoord;
                    auto currSpriteTileHit = spritesHitMap[currSpriteTileCoord.x()][currSpriteTileCoord.y()];
                    if (currSpriteTileHit == true)
                        continue;
                    else {
                        currSpriteTileHit = true;

                        auto& [texture, coordinate] = std::get<textureCoordinatePair_t>(rayTileHitResult.value());

                        auto& currSpriteBackbuffer = spritesRenderDataBuffers[renderSectionNum];

                        static const double projPlaneDist = (multimedia->windowParams.width/2.0)/std::tan(DegreesToRadians(fov/2.0));

                        Vec2 vecToSprite = coordinate - worldState->player.location;
                        double spriteHitDistY = Dot(vecToSprite, worldState->player.viewDir);
                        double spriteHitDistX = Dot(vecToSprite, worldState->player.eastDir);
                        int screenX = (multimedia->windowParams.width/2) + ((projPlaneDist/spriteHitDistY)*spriteHitDistX);

                        int renderHeight = GetRenderHeight(spriteHitDistY);
                        int renderWidth  = renderHeight;

                        SDL_Rect screenRect = GetFullTextureScreenRect(renderHeight, screenX);

                        spritesRenderDataBuffers[renderSectionNum].emplace_back(currSpriteID, texture, screenRect);

                        continue;
                    }
                }

            } else
                continue;
        }
    }
}

void Renderer::FullRenderToBuffers() {
    std::thread renderThreads[cores];

    // Launch a rendering thread for each CPU core
    for (int i = 0; i < cores; ++i)
        renderThreads[i] = std::thread(&Renderer::PartialRenderToBuffers, this, startingPixels[i], endingPixels[i], i);

    // Wait for all threads to finish rendering their sections
    for (int i = 0; i < cores; ++i)
        renderThreads[i].join();
}

void Renderer::DrawWalls() const {
    SDL_SetRenderTarget(multimedia->sdlRenderer, nullptr);
    for (const auto& w : wallsRenderDataBuffer) {
        const auto& [texture, textureRect, screenRect] = w;
        SDL_RenderCopy(multimedia->sdlRenderer, texture, &(textureRect), &(screenRect));
    }
}

void Renderer::RenderSpritesIntoBackBuffer() {
//    for (int backbufferNum = 0; backbufferNum < spritesRenderDataBuffers.size() - 1; ++backbufferNum) {
//        auto& currentBuffer = spritesRenderDataBuffers[backbufferNum];
//        auto& nextBuffer = spritesRenderDataBuffers[backbufferNum + 1];
//        if (!currentBuffer.empty() && !nextBuffer.empty()) {
//            if (currentBuffer.back().spriteID == nextBuffer.front().spriteID)
//                nextBuffer.erase(nextBuffer.begin());
//        }
//    }

    for (auto& spritesVec : spritesRenderDataBuffers) {
        for (auto& spriteData : spritesVec)
            allspritesbuffer.emplace_back(spriteData);
        spritesVec.clear();
    }



        std::sort(allspritesbuffer.begin(), allspritesbuffer.end(), [](const spriteRenderData_t s1, const spriteRenderData_t s2){
            return s1.textureRenderData.screenRect.h > s2.textureRenderData.screenRect.h;
        });

    SDL_SetRenderTarget(multimedia->sdlRenderer, spritesBackBuffer);
            for (auto sprite = allspritesbuffer.rbegin(); sprite != allspritesbuffer.rend(); ++sprite) {
                const auto& [spriteID, textureRenderData] = *sprite;
                const auto& [spriteTexture, textureRect, screenRect] = textureRenderData;
                SDL_RenderCopy(multimedia->sdlRenderer, spriteTexture, &(textureRect), &(screenRect));
            }
}

void Renderer::DrawSprites() {
    SDL_SetRenderTarget(multimedia->sdlRenderer, nullptr);
            for (auto sprite = allspritesbuffer.rbegin(); sprite != allspritesbuffer.rend(); ++sprite) {
                const auto& [spriteID, textureRenderData] = *sprite;
                const auto& [spriteTexture, textureRect, screenRect] = textureRenderData;

                for (int x = screenRect.x; x < screenRect.x + screenRect.w; ++x) {
                    if (wallsRenderDataBuffer[x].screenRect.h < screenRect.h) {
                        SDL_Rect spriteScreenSlice = {x, 0, 1, multimedia->windowParams.height};
                        SDL_RenderCopy(multimedia->sdlRenderer, spritesBackBuffer, &(spriteScreenSlice), &(spriteScreenSlice));
                    }
                }

            }


        SDL_RenderPresent(multimedia->sdlRenderer);

    // need to clear sprite back buffer (SDL_Texture). Below code doesn't work.
    SDL_SetRenderTarget(multimedia->sdlRenderer, spritesBackBuffer);
    SDL_RenderClear(multimedia->sdlRenderer);

    for (auto& column : spritesHitMap) {
        column.resize(worldState->map.height);
        std::fill(column.begin(), column.end(), false);
    }

    allspritesbuffer.clear();
}

Ray Renderer::GetRay(const int rayNum) const {
    return Ray(worldState->player.location, worldState->player.viewDir.Rotate(castingRayAngles[rayNum].first));
}

int Renderer::GetRenderHeight(const double perpHitDist) const {
    static const double proportionalityConstant = 1.3 * multimedia->windowParams.width / ((16.0 / 9.0) * (fov / 72.0));
    return static_cast<int>(proportionalityConstant / perpHitDist);
}

SDL_Rect Renderer::GetTextureSliceScreenRect(const int renderHeight, int sliceNum) const {
    return {sliceNum, multimedia->windowParams.height/2 - renderHeight/2, 1, renderHeight};
}

SDL_Rect Renderer::GetFullTextureScreenRect(const int renderHeight, int textureCenterX) const {
    // Assumed renderHeight = renderWidth
    return {textureCenterX - renderHeight/2, multimedia->windowParams.height/2 - renderHeight/2, renderHeight, renderHeight};
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