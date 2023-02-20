#pragma once

#include "../../GameEngine/State/WorldState/WorldState.h"
#include "../../GameEngine/Multimedia/Multimedia.h"

/*
=========================================================
    Relevant type definitions
=========================================================
*/

typedef std::pair<SDL_Rect*, int> tileRects_t;

/*
=========================================================
    Minimap
=========================================================
*/

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

    tileRects_t wallTileRects;
    tileRects_t doorTileRects;
    tileRects_t spriteTileRects;

public:
    Minimap(WorldState* const _worldState, Multimedia* const _multimedia, const int _tileSize);
    ~Minimap();

    void Update()         const;

private:
    void       DrawBackground()                                                                const;
    void       DrawGridlines()                                                                 const;
    void       DrawNonEmptyTiles()                                                             const;
    void       DrawPlayerTile()                                                                const;
    void       DrawPlayerRaycasts()                                                            const;
    SDL_Rect   TileToRect                (const iPoint2 &tileCoord)                            const;
    Pixel      MapCoordToMinimapCoord    (const Point2& mapCoord)                              const;
    void       CollectTileRectsFromMap   (const tileType_t tileType, tileRects_t& tileRects);
};