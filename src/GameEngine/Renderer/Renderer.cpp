#include "Renderer.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

spriteRenderData_t::spriteRenderData_t(const int _screenX, SDL_Texture* const _texture, const int _renderHeight) :
    screenX(_screenX), texture(_texture), renderHeight(_renderHeight) {}

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
    wallHitReturnDataVec.resize(multimedia->windowParams.width);
    wallRenderHeightVec.resize(multimedia->windowParams.width);

    spritesRenderHeightVec.resize(multimedia->windowParams.width);


    spriteTilesHitMap = new bool*[worldState->map.width];
    for (int i = 0; i < worldState->map.width; ++i)
        spriteTilesHitMap[i] = new bool[worldState->map.height];

    for (int i = 0; i < worldState->map.width; ++i) {
        for (int cell = 0; cell < worldState->map.height; ++cell)
            spriteTilesHitMap[i][cell] = false;
    }

    spriteHitReturnDataVec.reserve(worldState->map.width*worldState->map.height - 2*(worldState->map.width) - 2*(worldState->map.height-2));
    spriteRenderDataVec.reserve(worldState->map.width*worldState->map.height - 2*(worldState->map.width) - 2*(worldState->map.height-2));

    spritesBackBuffer = SDL_CreateTexture(multimedia->sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, multimedia->windowParams.width, multimedia->windowParams.height);
    spritesBackBuffer_clear = SDL_CreateTexture(multimedia->sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, multimedia->windowParams.width, multimedia->windowParams.height);
    SDL_SetTextureBlendMode(spritesBackBuffer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(spritesBackBuffer_clear, SDL_BLENDMODE_BLEND);
}

void Renderer::Exit() const {
    SDL_DestroyTexture(spritesBackBuffer);
    SDL_DestroyTexture(spritesBackBuffer_clear);

    for (int i = 0; i < worldState->map.width; ++i)
            delete[] spriteTilesHitMap[i];
    delete[] spriteTilesHitMap;
}

void Renderer::RenderFrame() {
    SDL_RenderClear(multimedia->sdlRenderer);
    DrawCeilingFloor();
    FullRenderToBuffers();
    DrawWalls();
    DrawSprites();
    SDL_RenderPresent(multimedia->sdlRenderer);

    // Clear sprites backbuffer
    SDL_SetRenderTarget(multimedia->sdlRenderer, spritesBackBuffer);
    SDL_RenderClear(multimedia->sdlRenderer);
    SDL_SetRenderTarget(multimedia->sdlRenderer, nullptr);
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

void Renderer::PartialRenderToBuffers(const int startRayNum, const int endRayNum) {
    for (int rayNum = startRayNum; rayNum < endRayNum; ++rayNum) {
        RayHitMarker rayHitMarker(GetRay(rayNum));

        while (worldState->map.WithinMap(rayHitMarker.hitTile)) {
            Tile* prevTile = worldState->map.GetTile(rayHitMarker.hitTile);
            rayHitMarker.GoToNextHit();
            Tile* currTile = worldState->map.GetTile(rayHitMarker.hitTile);

            auto rayTileHitResult = currTile->RayTileHit(rayHitMarker);
            if (rayTileHitResult.has_value()) {

                // WallTile or DoorTile hit (returns textureSliceDistPair_t)
                if (std::holds_alternative<textureSliceDistPair_t>(rayTileHitResult.value())) {

                    auto& textureSliceDistPair = std::get<textureSliceDistPair_t>(rayTileHitResult.value());
                    auto& [textureSlice, hitDistance] = textureSliceDistPair;

                    // If prev tile type was a door and next is a wall, it means the next hit is a door sidewall ; thus, must swap texture
                    if (prevTile->type == tileType_t::DOOR && currTile->type == tileType_t::WALL)
                        textureSlice.texture = Tile::LightTexture(gateSidewallTexture, rayHitMarker);

                    wallHitReturnDataVec[rayNum] = textureSliceDistPair;

                    break;

                  // SpriteTile hit (returns textureCoordinatePair_t)
                } else {
                    SpriteTile* spriteTileHit = static_cast<SpriteTile*>(currTile);
                    auto& spriteTileHitOnMap = spriteTilesHitMap[spriteTileHit->tileCoord.x()][spriteTileHit->tileCoord.y()];

                    if (spriteTileHitOnMap == false) {
                        spriteTileHitOnMap = true;
                        auto& textureCoordinatePair = std::get<textureCoordinatePair_t>(rayTileHitResult.value());
                        spriteHitReturnDataVec.emplace_back(textureCoordinatePair);
                    }
                    continue;
                }
            } else
                continue;
        }
    }
}

void Renderer::FullRenderToBuffers() {
//    std::thread renderThreads[cores];
//
//    // Launch a rendering thread for each CPU core
//    for (int i = 0; i < cores; ++i)
//        renderThreads[i] = std::thread(&Renderer::PartialRenderToBuffers, this, startingPixels[i], endingPixels[i]);
//
//    // Wait for all threads to finish rendering their sections
//    for (int i = 0; i < cores; ++i)
//        renderThreads[i].join();


    //auto t = std::thread(&Renderer::PartialRenderToBuffers, this, 0, multimedia->windowParams.width);
    //t.join();

    PartialRenderToBuffers(0, multimedia->windowParams.width);

    // Clear sprite tiles hit map
    for (int i = 0; i < worldState->map.width; ++i) {
        for (int cell = 0; cell < worldState->map.height; ++cell)
            spriteTilesHitMap[i][cell] = false;
    }

}

void Renderer::DrawWalls() {
    for (int i = 0; i < multimedia->windowParams.width; ++i) {
        wallRenderHeightVec[i] = GetRenderHeight(wallHitReturnDataVec[i].hitDistance * castingRayAngles[i].second);
        SDL_Rect wallSliceScreenRect = GetTextureSliceScreenRect(wallRenderHeightVec[i], i);
        SDL_RenderCopy(multimedia->sdlRenderer, wallHitReturnDataVec[i].textureSlice.texture, &(wallHitReturnDataVec[i].textureSlice.textureRect), &(wallSliceScreenRect));
    }
}

void Renderer::DrawSprites() {
    // From texture and coordinate of each sprite, calculate screen x position and render height
    for (const auto& spriteHit : spriteHitReturnDataVec) {
        auto& [texture, coordinate] = spriteHit;

        static const double projPlaneDist = (multimedia->windowParams.width/2.0)/std::tan(DegreesToRadians(fov/2.0));

        Vec2 vecToSprite = coordinate - worldState->player.location;
        double spriteHitDistY = Dot(vecToSprite, worldState->player.viewDir);
        double spriteHitDistX = Dot(vecToSprite, worldState->player.eastDir);
        int screenX = (multimedia->windowParams.width/2) + ((projPlaneDist/spriteHitDistY)*spriteHitDistX);

        int renderHeight = GetRenderHeight(spriteHitDistY);

        spriteRenderDataVec.emplace_back(spriteRenderData_t(screenX, texture, renderHeight));
    }
    spriteHitReturnDataVec.clear();

    // Sort sprites by render height (we must render from back to front)
    std::sort(spriteRenderDataVec.begin(), spriteRenderDataVec.end(), [](const spriteRenderData_t s1, const spriteRenderData_t s2){
        return s1.renderHeight < s2.renderHeight;
    });

    // Render each sprite from back to front
    for (const auto& sprite : spriteRenderDataVec) {
        SDL_Rect spriteScreenRect = GetFullTextureScreenRect(sprite.renderHeight, sprite.screenX);
        for (int x = spriteScreenRect.x; x < spriteScreenRect.x + spriteScreenRect.w; ++x) {
            if (x < 0 || x >= multimedia->windowParams.width)
                continue;

            if (wallRenderHeightVec[x] < spriteScreenRect.h) {
                double widthPercent = static_cast<double>(x - spriteScreenRect.x)/(spriteScreenRect.w);
                int textureX = static_cast<int>(widthPercent*TEXTURE_PITCH);
                SDL_Rect textureSlice = {textureX, 0, 1, TEXTURE_PITCH};
                SDL_Rect spriteScreenSlice = {x, spriteScreenRect.y, 1, spriteScreenRect.h};
                SDL_RenderCopy(multimedia->sdlRenderer, sprite.texture, &(textureSlice), &(spriteScreenSlice));
            }
        }
    } // if sprite starts off off screen, rendering the pixels not visible is wasteful
    spriteRenderDataVec.clear();
    for (auto& w : wallRenderHeightVec) w = 0;


//    for (auto& h : spritesRenderHeightVec) h = 0;
//    SDL_SetRenderTarget(multimedia->sdlRenderer, spritesBackBuffer);
//    for (auto sprite = spriteRenderDataVec.rbegin(); sprite != spriteRenderDataVec.rend(); ++sprite) {
//        SDL_Rect spriteScreenRect = GetFullTextureScreenRect(sprite->renderHeight, sprite->screenX);
//        FillSpriteRenderHeights(GetFullTextureScreenRect(sprite->renderHeight, sprite->screenX).x, sprite->renderHeight);
//        SDL_RenderCopy(multimedia->sdlRenderer, sprite->texture, nullptr, &(spriteScreenRect));
//    }
//    spriteRenderDataVec.clear();
//
//    // For each column of pixels, if sprite render height is greater than wall's, render out that column from the sprite backbuffer
//    SDL_SetRenderTarget(multimedia->sdlRenderer, nullptr);
//    for (int i = 0; i < multimedia->windowParams.width; ++i) {
//        if (wallRenderHeightVec[i] < spritesRenderHeightVec[i]) {
//            SDL_Rect spriteSlice = {i, 0, 1, multimedia->windowParams.height};
//            SDL_RenderCopy(multimedia->sdlRenderer, spritesBackBuffer, &(spriteSlice), &(spriteSlice));
//        }
//    }
//    for (auto& w : wallRenderHeightVec) w = 0;
}

Ray Renderer::GetRay(const int rayNum) const {
    return Ray(worldState->player.location, worldState->player.viewDir.Rotate(castingRayAngles[rayNum].first));
}

int Renderer::GetRenderHeight(const double perpHitDist) const {
    static const double proportionalityConstant = 1.3 * multimedia->windowParams.width / ((16.0 / 9.0) * (fov / 72.0));
    return static_cast<int>(proportionalityConstant / perpHitDist);
}

SDL_Rect Renderer::GetTextureSliceScreenRect(const int renderHeight, const int sliceNum) const {
    return {sliceNum, multimedia->windowParams.height/2 - renderHeight/2, 1, renderHeight};
}

SDL_Rect Renderer::GetFullTextureScreenRect(const int renderHeight, const int textureCenterX) const {
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

void Renderer::FillSpriteRenderHeights(const int index, const int renderHeight) {
    if (index + renderHeight > multimedia->windowParams.width)
        std::fill(spritesRenderHeightVec.begin()+index, spritesRenderHeightVec.end(), renderHeight);
    else if (index < 0)
        std::fill_n(spritesRenderHeightVec.begin(), renderHeight+index, renderHeight);
    else
        std::fill_n(spritesRenderHeightVec.begin()+index, renderHeight, renderHeight);
}