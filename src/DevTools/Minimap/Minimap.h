#pragma once

#include "../../GameEngine/State/WorldState/WorldState.h"
#include "../../GameEngine/Multimedia/Multimedia.h"

class Minimap {
private:
    WorldState*   worldState;
    Multimedia*   multimedia;

    SDL_Window*   minimapWindow;
    SDL_Renderer* minimapRenderer;
    int  minimapWindowWidth;
    int  minimapWindowHeight;
    int  tileSize;

    Pixel origin;
    iVec2 down;
    iVec2 right;
    Pixel bottomLeftCorner;
    Pixel topRightCorner;

public:
    Minimap(WorldState* const _worldState, Multimedia* const _multimedia, const int _tileSize);
    ~Minimap();

    void Update()         const;

private:
    void DrawBackground() const;
    void DrawGridlines()  const;
    void DrawPlayerTile() const;
};