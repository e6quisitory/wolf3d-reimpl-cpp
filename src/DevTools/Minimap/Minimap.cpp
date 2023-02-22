#include "Minimap.h"

/*
=========================================================
    Constructors
=========================================================
*/

Minimap::Minimap(WorldState* const _worldState, Multimedia* const _multimedia, const int _tileSize) {
    worldState = _worldState;
    multimedia = _multimedia;
    tileSize = _tileSize;

    // Only create minimap if main window is NOT fullscreen
    auto mainWindowFlags = SDL_GetWindowFlags(multimedia->sdlWindow);
    bool mainWindowIsFullScreen = mainWindowFlags & SDL_WINDOW_FULLSCREEN == SDL_WINDOW_FULLSCREEN;
    assert(!mainWindowIsFullScreen);

    // Create window and renderer
    minimapWindowWidth  = tileSize * worldState->map.width + 2 * tileSize;
    minimapWindowHeight = tileSize * worldState->map.height + 2 * tileSize;
    minimapWindow    = SDL_CreateWindow("Minimap", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, minimapWindowWidth, minimapWindowHeight, SDL_WINDOW_SHOWN);
    minimapRenderer  = SDL_CreateRenderer(minimapWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Set windows locations (avoid getting one on top of the other)
    int mainWindowX = (multimedia->displayParams.width / 2 - multimedia->windowParams.width / 2) / 4;
    int mainWindowY = multimedia->displayParams.height / 2 - multimedia->windowParams.height / 2;
    int mainWindowWidth = multimedia->windowParams.width;
    SDL_SetWindowPosition(multimedia->sdlWindow, mainWindowX, mainWindowY);

    int minimapWindowX = mainWindowX + mainWindowWidth + 20;
    int minimapWindowY = multimedia->displayParams.height / 2 - minimapWindowHeight / 2;
    SDL_SetWindowPosition(minimapWindow, minimapWindowX, minimapWindowY);

    // Define major map coordinates relative to screen
    origin = Pixel(tileSize, tileSize*(worldState->map.height + 1));
    up   = iVec2(0, -tileSize);
    right  = iVec2(tileSize, 0);
    topLeftCorner = origin + worldState->map.height*up;
    bottomRightCorner = origin + worldState->map.width*right;

    // Collect all non-empty tiles
    CollectTileRectsFromMap(tileType_t::WALL, wallTileRects);
    CollectTileRectsFromMap(tileType_t::DOOR, doorTileRects);
    CollectTileRectsFromMap(tileType_t::SPRITE, spriteTileRects);
}

/*
=========================================================
    Destructors
=========================================================
*/

Minimap::~Minimap() {
    SDL_DestroyRenderer(minimapRenderer);
    SDL_DestroyWindow(minimapWindow);

    delete wallTileRects.first;
    delete doorTileRects.first;
}

/*
=========================================================
    Public methods
=========================================================
*/

void Minimap::Update() const {
    DrawBackground();
    DrawGridlines();
    DrawNonEmptyTiles();
    DrawPlayerTile();
    DrawPlayerRaycasts();
    SDL_RenderPresent(minimapRenderer);
}

/*
=========================================================
    Private methods
=========================================================
*/

void Minimap::DrawBackground() const {
    SDL_SetRenderDrawColor(minimapRenderer, 22, 22, 22, 255);
    SDL_Rect background = {0, 0, minimapWindowWidth, minimapWindowHeight};
    SDL_RenderFillRect(minimapRenderer, &background);
}

void Minimap::DrawGridlines() const {
    SDL_SetRenderDrawColor(minimapRenderer, 44, 44, 44, 255);
    for (Pixel p = origin; p.x() <= bottomRightCorner.x(); p += right)
        SDL_RenderDrawLine(minimapRenderer, p.x(), origin.y(), p.x(), topLeftCorner.y());

    for (Pixel p = origin; p.y() >= topLeftCorner.y(); p += up)
        SDL_RenderDrawLine(minimapRenderer, origin.x(), p.y(), bottomRightCorner.x(), p.y());
}

void Minimap::DrawNonEmptyTiles() const {
    // Draw wall tiles
    SDL_SetRenderDrawColor(minimapRenderer, 150, 0, 0, 255);
    SDL_RenderFillRects(minimapRenderer, wallTileRects.first, wallTileRects.second);

    // Draw door tiles
    SDL_SetRenderDrawColor(minimapRenderer, 150, 118, 2, 255);
    SDL_RenderFillRects(minimapRenderer, doorTileRects.first, doorTileRects.second);

    // Draw sprite tiles
    SDL_SetRenderDrawColor(minimapRenderer, 176, 4, 164, 255);
    SDL_RenderFillRects(minimapRenderer, spriteTileRects.first, spriteTileRects.second);
}

void Minimap::DrawPlayerTile() const {
    iPoint2  playerTile  = iPoint2(worldState->player.location);
    SDL_Rect playerRect  = TileToRect(playerTile);
    SDL_SetRenderDrawColor(minimapRenderer, 255, 255, 255, 255);
    SDL_RenderFillRect(minimapRenderer, &playerRect);
}

void Minimap::DrawPlayerRaycasts() const {
    Point2 playerLocCentered = iPoint2(worldState->player.location) + Point2(0.5, 0.5);
    Pixel raycastStart = MapCoordToMinimapCoord(playerLocCentered);
    Pixel raycastMiddleEnd   = MapCoordToMinimapCoord(playerLocCentered + 2*worldState->player.viewDir);

    SDL_SetRenderDrawColor(minimapRenderer, 0, 233, 250, 255);
    for (int angle = -36; angle <= 36; ++angle) {
        Pixel raycastEnd = (raycastMiddleEnd - raycastStart).Rotate(DegreesToRadians(angle)) + raycastStart;
        SDL_RenderDrawLine(minimapRenderer, raycastStart.x(), raycastStart.y(), raycastEnd.x(), raycastEnd.y());
    }
}

SDL_Rect Minimap::TileToRect(const iPoint2 &tileCoord) const {
    auto minimapGridRectTopLeft = MapCoordToMinimapCoord(tileCoord) + up;
    return {minimapGridRectTopLeft.x(), minimapGridRectTopLeft.y(), tileSize, tileSize};
}

Pixel Minimap::MapCoordToMinimapCoord(const Point2& mapCoord) const {
    return Pixel((origin + right*mapCoord.x()).x(), (origin + up*mapCoord.y()).y());
}

void Minimap::CollectTileRectsFromMap(const tileType_t tileType, tileRects_t& tileRects) {
    // First collect tiles of specified type from map
    std::vector<iPoint2> tileCoords;
    for (const auto& mapColumn : worldState->map.tiles)
        for (const auto& mapTile : mapColumn)
            if (mapTile->type == tileType)
                tileCoords.push_back(mapTile->coordinate);

    // Then fill in corresponding SDL_Rects
    auto& [rects, count] = tileRects;
    count = tileCoords.size();
    rects = new SDL_Rect[count];
    for (int i = 0; i < count; ++i)
        rects[i] = TileToRect(tileCoords[i]);
}