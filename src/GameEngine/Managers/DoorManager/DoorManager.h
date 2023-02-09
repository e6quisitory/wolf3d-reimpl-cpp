#pragma once

#include <utility>
#include "../../State/GameState/GameState.h"
#include "../../State/GameState/Map/Tile/Tile.h"

/*
=========================================================
    DoorManager
=========================================================
*/

class DoorManager {
private:
    GameState*                  gameState;
    double                      movementIncrement;  // Units of door movement per frame
    double                      timerIncrement;     // Timer units per frame
    std::pair<bool, DoorTile*>  activeDoorToErase;  // Only one door can be opened at once; thus only one door can have closed at any given time

public:
    void       Init                             (GameState *const _gameState, const int _screenRefreshRate);
    void       Update();

private:
    void       MoveDoor                         (DoorTile* const doorTile);

    bool       InsideDoor                       (DoorTile* const doorTile)     const;
    bool       InsideAnyDoor()                                                 const;

    bool       ActiveDoorAwaitingRemoval()                                     const;
    DoorTile*  GetActiveDoorAwaitingRemove()                                   const;
    void       SetActiveDoorForRemoval          (DoorTile* const _door);
    void       ClearActiveDoorForRemoval();
    void       RemoveActiveDoorIfAnyAwaiting();

    void       DecrementTimer                   (DoorTile* const doorTile)     const;
    void       ResetTimer                       (DoorTile* const doorTile)     const;
};