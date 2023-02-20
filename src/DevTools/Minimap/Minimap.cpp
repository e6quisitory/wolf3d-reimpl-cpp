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
    origin = Pixel(tileSize, tileSize);
    down = iVec2(0, tileSize);
    right = iVec2(tileSize, 0);
    bottomLeftCorner = origin + worldState->map.height*down;
    topRightCorner   = origin + worldState->map.width*right;
}

/*
=========================================================
    Destructors
=========================================================
*/

Minimap::~Minimap() {
    SDL_DestroyRenderer(minimapRenderer);
    SDL_DestroyWindow(minimapWindow);
}

/*
=========================================================
    Public methods
=========================================================
*/

void Minimap::Update() const {
    DrawBackground();
    DrawGridlines();
    DrawPlayerTile();
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
    for (Pixel p = origin; p.x() <= topRightCorner.x(); p += right)
        SDL_RenderDrawLine(minimapRenderer, p.x(), p.y(), p.x(), p.y()+ worldState->map.height * tileSize);

    for (Pixel p = origin; p.y() <= bottomLeftCorner.y(); p += down)
        SDL_RenderDrawLine(minimapRenderer, p.x(), p.y(), p.x()+ worldState->map.width * tileSize, p.y());
}

void Minimap::DrawPlayerTile() const {
    iPoint2 playerTile  = iPoint2(worldState->player.location);
    SDL_Rect playerRect = {origin.x()+playerTile.x()*tileSize, origin.y()+playerTile.y()*tileSize, tileSize, tileSize};
    SDL_SetRenderDrawColor(minimapRenderer, 255, 255, 255, 255);
    SDL_RenderFillRect(minimapRenderer, &playerRect);
}