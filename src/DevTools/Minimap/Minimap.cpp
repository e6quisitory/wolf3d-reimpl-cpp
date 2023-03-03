#include "Minimap.h"

/*
=========================================================
    Constructors
=========================================================
*/

Minimap::Minimap(WorldState* const _worldState, Multimedia* const _multimedia, InputsBuffer* const _inputsBuffer, const int _tileSize) {
    worldState   = _worldState;
    multimedia   = _multimedia;
    inputsBuffer = _inputsBuffer;
    tileSize     = _tileSize;

    // Only create minimap if main window is NOT fullscreen
    if (multimedia->mainWindowIsFullScreen) return;

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
    CollectTileRectsFromMap(tileType_t::OBJECT, objectTileRects);
    CollectTileRectsFromMap(tileType_t::ENEMY, enemyTileRects);

    // Give back window focus to main game window
    SDL_RaiseWindow(multimedia->sdlWindow);
    SDL_SetWindowInputFocus(multimedia->sdlWindow);
}

/*
=========================================================
    Destructors
=========================================================
*/

Minimap::~Minimap() {
    if (!multimedia->mainWindowIsFullScreen) {
        SDL_DestroyRenderer(minimapRenderer);
        SDL_DestroyWindow(minimapWindow);
        delete[] wallTileRects.first;
        delete[] doorTileRects.first;
    }
}

/*
=========================================================
    Public methods
=========================================================
*/

void Minimap::Update() const {
    if (!multimedia->mainWindowIsFullScreen) {
        DrawBackground();
        DrawGridlines();
        DrawNonEmptyTiles();
        DrawPlayerTile();
        DrawPlayerRaycasts();
        SDL_RenderPresent(minimapRenderer);
        HandleInputs();
    }
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
    SDL_SetRenderDrawColor(minimapRenderer, 100, 0, 0, 255);
    SDL_RenderFillRects(minimapRenderer, wallTileRects.first, wallTileRects.second);

    // Draw door tiles
    SDL_SetRenderDrawColor(minimapRenderer, 150, 118, 2, 255);
    SDL_RenderFillRects(minimapRenderer, doorTileRects.first, doorTileRects.second);

    // Draw object tiles
    SDL_SetRenderDrawColor(minimapRenderer, 176, 4, 164, 255);
    SDL_RenderFillRects(minimapRenderer, objectTileRects.first, objectTileRects.second);

    // Draw enemy tiles
    SDL_SetRenderDrawColor(minimapRenderer, 255, 0, 0, 255);
    SDL_RenderFillRects(minimapRenderer, enemyTileRects.first, enemyTileRects.second);
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

void Minimap::HandleInputs() const {

    /***************************** Mouse clicks ******************************/

    const auto& mouseClick = inputsBuffer->nonGameWindowMouseClick;
    const auto  windowInFocus = SDL_GetMouseFocus();

    if (mouseClick != mouseClickType_t::NONE && windowInFocus == minimapWindow) {
        auto mapCoord = WindowCoordToMapCoord(inputsBuffer->mouseClickLocation);
        if (mapCoord.has_value()) {
            if (mouseClick == mouseClickType_t::LEFT) {
                auto mapTile = worldState->map.GetTile(mapCoord.value());
                if (!mapTile->PlayerTileHit())
                    worldState->player.location = mapCoord.value();
            } else if (mouseClick == mouseClickType_t::RIGHT) {
                Vec2 newViewDir = UnitVector(mapCoord.value() - worldState->player.location);
                worldState->player.viewDir = newViewDir;
                worldState->player.eastDir = newViewDir.Rotate(-PI/2);
            }
        }
    }
}

SDL_Rect Minimap::TileToRect(const iPoint2 &tileCoord) const {
    auto minimapGridRectTopLeft = MapCoordToMinimapCoord(tileCoord) + up;
    return {minimapGridRectTopLeft.x(), minimapGridRectTopLeft.y(), tileSize, tileSize};
}

Pixel Minimap::MapCoordToMinimapCoord(const Point2& mapCoord) const {
    return Pixel((origin + right*mapCoord.x()).x(), (origin + up*mapCoord.y()).y());
}

Point2_o Minimap::WindowCoordToMapCoord(const Pixel& windowCoord) const {
    static const std::pair<int, int> xBounds = {topLeftCorner.x(), bottomRightCorner.x()};
    static const std::pair<int, int> yBounds = {topLeftCorner.y(), bottomRightCorner.y()};
    if ((windowCoord.x() < xBounds.first || windowCoord.x() > xBounds.second) || (windowCoord.y() < yBounds.first || windowCoord.y() > yBounds.second))
        return std::nullopt;
    else {
        int minimapX = windowCoord.x() - origin.x();
        int minimapY = bottomRightCorner.y() - (windowCoord.y() - topLeftCorner.y()) - tileSize;
        double mapX = static_cast<double>(minimapX)/tileSize;
        double mapY = static_cast<double>(minimapY)/tileSize;

        return Point2(mapX, mapY);
    }
}

void Minimap::CollectTileRectsFromMap(const tileType_t tileType, tileRects_t& tileRects) {
    // First collect tiles of specified type from map
    std::vector<iPoint2> tileCoords;
    for (int y = 0; y < worldState->map.height; ++y) {
        for (int x = 0; x < worldState->map.width; ++x) {
            iPoint2 tileCoord(x, y);
            if (worldState->map.GetTile(tileCoord)->type == tileType)
                tileCoords.push_back(tileCoord);
        }
    }

    // Then fill in corresponding SDL_Rects
    auto& [rects, count] = tileRects;
    count = tileCoords.size();
    rects = new SDL_Rect[count];
    for (int tileCoordNum = 0; tileCoordNum < count; ++tileCoordNum)
        rects[tileCoordNum] = TileToRect(tileCoords[tileCoordNum]);
}