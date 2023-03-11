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

    // Wall render buffers
    wallsReturnData.resize(multimedia->windowParams.width);
    wallRenderHeights.resize(multimedia->windowParams.width);

    // Sprite render buffers
    spriteTilesHitMap.resize(worldState->map.width);
    for (auto& tileColumn : spriteTilesHitMap)
        tileColumn.resize(worldState->map.height);
    ClearSpriteTilesHitMap();
    int numInnerTiles = worldState->map.width*worldState->map.height - 2*(worldState->map.width) - 2*(worldState->map.height-2);
    spritesReturnData.reserve(numInnerTiles);
    spritesRenderData.reserve(numInnerTiles);
}

void Renderer::RenderFrame() {
    SDL_RenderClear(multimedia->sdlRenderer);
    DrawCeilingFloor();
    RenderToBuffers();
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

void Renderer::RenderToBuffers() {
    for (int rayNum = 0; rayNum < multimedia->windowParams.width; ++rayNum) {
        RayHitMarker rayHitMarker(GetRay(rayNum));

        while (worldState->map.WithinMap(rayHitMarker.hitTile)) {
            Tile* prevTile = worldState->map.GetTile(rayHitMarker.hitTile);
            rayHitMarker.GoToNextHit();
            Tile* currTile = worldState->map.GetTile(rayHitMarker.hitTile);

            if (currTile->enemyContainer) {
                auto currEnemyContainerTile = static_cast<EnemyContainerTile*>(currTile);

                if (!currEnemyContainerTile->enemies.empty()) {
                    auto [enemyTileHitX, enemyTileHitY] = currEnemyContainerTile->tileCoord.e;
                    auto enemyTileHitOnMap = spriteTilesHitMap[enemyTileHitX][enemyTileHitX];

                    if (enemyTileHitOnMap == false) {
                        enemyTileHitOnMap = true;
                        auto spriteRayTileHitResult = currTile->RayTileHit(rayHitMarker);
                        for (const auto& textureCoordPair : spriteRayTileHitResult.second.value())
                            spritesReturnData.emplace_back(textureCoordPair);
                    }
                }
            }

            auto result = currTile->RayTileHit(rayHitMarker).first;
            if (result.has_value()) {
                auto& textureSliceDistPair = result.value();
                auto& [textureSlice, hitDistance] = textureSliceDistPair;

                // If prev tile type was a door and next is a wall, it means the next hit is a door sidewall ; thus, must swap texture
                if (prevTile->type == tileType_t::DOOR && currTile->type == tileType_t::WALL)
                    textureSlice.texture = Tile::LightTexture(gateSidewallTexture, rayHitMarker);

                wallsReturnData[rayNum] = textureSliceDistPair;
                break;
            } else
                continue;


//            if (currTile->type == tileType_t::EMPTY)
//                continue;
//            else if (currTile->type == tileType_t::ENEMY || currTile->type == tileType_t::OBJECT) {
//                SpriteTile* spriteTileHit = static_cast<SpriteTile*>(currTile);
//                auto [spriteTileHitX, spriteTileHitY] = spriteTileHit->tileCoord.e;
//                auto spriteTileOnHitMap = spriteTilesHitMap[spriteTileHitX][spriteTileHitY];
//
//                if (spriteTileOnHitMap == false) {
//                    spriteTileOnHitMap = true;
//                    auto spriteRayTileHitResult = currTile->RayTileHit(rayHitMarker);
//                    auto& textureCoordinatePair = std::get<textureCoordPair_t>(spriteRayTileHitResult.value());
//                    spritesReturnData.emplace_back(textureCoordinatePair);
//                } else
//                    continue;
//            } else {
//                auto wallRayTileHitResult = currTile->RayTileHit(rayHitMarker);
//                if (wallRayTileHitResult.has_value()) {
//                    if (std::holds_alternative<textureSliceDistPair_t>(wallRayTileHitResult.value())) {
//                        auto& textureSliceDistPair = std::get<textureSliceDistPair_t>(wallRayTileHitResult.value());
//                        auto& [textureSlice, hitDistance] = textureSliceDistPair;
//
//                        // If prev tile type was a door and next is a wall, it means the next hit is a door sidewall ; thus, must swap texture
//                        if (prevTile->type == tileType_t::DOOR && currTile->type == tileType_t::WALL)
//                            textureSlice.texture = Tile::LightTexture(gateSidewallTexture, rayHitMarker);
//
//                        wallsReturnData[rayNum] = textureSliceDistPair;
//                        break;
//                    } else
//                        continue;
//                }
//            }
        }
    }
    ClearSpriteTilesHitMap();
}

void Renderer::DrawWalls() {
    for (int i = 0; i < multimedia->windowParams.width; ++i) {
        wallRenderHeights[i] = GetRenderHeight(wallsReturnData[i].hitDistance * castingRayAngles[i].second);
        SDL_Rect wallSliceScreenRect = GetTextureSliceScreenRect(wallRenderHeights[i], i);
        SDL_RenderCopy(multimedia->sdlRenderer, wallsReturnData[i].textureSlice.texture, &(wallsReturnData[i].textureSlice.textureRect), &(wallSliceScreenRect));
    }
}

void Renderer::DrawSprites() {
    // From texture and coordinate of each sprite, calculate screen x position and render height
    for (const auto& spriteHit : spritesReturnData) {
        static const double projPlaneDist = (multimedia->windowParams.width/2.0)/std::tan(DegreesToRadians(fov/2.0));

        auto& [texture, coordinate] = spriteHit;
        Vec2 vecToSprite = coordinate - worldState->player.location;
        double spriteHitDistY = Dot(vecToSprite, worldState->player.viewDir);
        double spriteHitDistX = Dot(vecToSprite, worldState->player.eastDir);
        int screenX = (multimedia->windowParams.width/2) + ((projPlaneDist/spriteHitDistY)*spriteHitDistX);

        int renderHeight = GetRenderHeight(spriteHitDistY);

        spritesRenderData.emplace_back(spriteRenderData_t(screenX, texture, renderHeight));
    }
    spritesReturnData.clear();

    // Sort sprites by render height (we must render from back to front)
    std::sort(spritesRenderData.begin(), spritesRenderData.end(), [](const spriteRenderData_t& s1, const spriteRenderData_t& s2){
        return s1.renderHeight < s2.renderHeight;
    });

    // Render each sprite from back to front
    for (const auto& sprite : spritesRenderData) {
        SDL_Rect spriteScreenRect = GetFullTextureScreenRect(sprite.renderHeight, sprite.screenX);
        for (int x = spriteScreenRect.x; x < spriteScreenRect.x + spriteScreenRect.w; ++x) {
            if (x < 0 || x >= multimedia->windowParams.width)
                continue;

            if (wallRenderHeights[x] <= spriteScreenRect.h) {
                double textureWidthPercent = static_cast<double>(x - spriteScreenRect.x) / (spriteScreenRect.w);
                int textureX = static_cast<int>(textureWidthPercent * TEXTURE_PITCH);
                SDL_Rect textureSlice = {textureX, 0, 1, TEXTURE_PITCH};
                SDL_Rect spriteScreenSlice = {x, spriteScreenRect.y, 1, spriteScreenRect.h};
                SDL_RenderCopy(multimedia->sdlRenderer, sprite.texture, &(textureSlice), &(spriteScreenSlice));
            }
        }
    }
    spritesRenderData.clear();
}

Ray Renderer::GetRay(const int rayNum) const {
    return Ray(worldState->player.location, worldState->player.viewDir.Rotate(castingRayAngles[rayNum].first));
}

int Renderer::GetRenderHeight(const double perpHitDist) const {
    static const double proportionalityConstant = 1.15 * multimedia->windowParams.width / ((16.0 / 9.0) * (fov / 72.0));
    return static_cast<int>(proportionalityConstant / perpHitDist);
}

SDL_Rect Renderer::GetTextureSliceScreenRect(const int renderHeight, const int sliceNum) const {
    return {sliceNum, multimedia->windowParams.height/2 - renderHeight/2, 1, renderHeight};
}

SDL_Rect Renderer::GetFullTextureScreenRect(const int renderHeight, const int textureCenterX) const {
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

void Renderer::ClearSpriteTilesHitMap() {
    for (auto& tileColumn : spriteTilesHitMap)
        std::fill(tileColumn.begin(), tileColumn.end(), false);
}