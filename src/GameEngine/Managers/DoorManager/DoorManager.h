#pragma once

#include "../../State/WorldState/WorldState.h"
#include "../../State/WorldState/Map/Tile/DoorTile/DoorTile.h"

/*
=========================================================
    DoorManager
=========================================================
*/

class DoorManager {
private:
    WorldState*                 worldState;
    double                      movementIncrement;  // Units of door movement per frame
    double                      timerIncrement;     // Timer units per frame
    std::pair<bool, DoorTile*>  activeDoorToErase;  // Only one door can be opened at once; thus only one door can have closed at any given time

public:
    void       Init                             (WorldState* const _worldState, const int _screenRefreshRate);
    void       Update();

private:
    void       MoveDoor                         (DoorTile* const doorTile);

    bool       InsideDoor                       (DoorTile* const doorTile)     const;
    bool       InsideAnyDoor()                                                 const;

    bool       ActiveDoorAwaitingRemoval()                                     const;
    DoorTile*  GetActiveDoorAwaitingRemove()                                   const;
    void       SetActiveDoorForRemoval          (DoorTile* const doorTile);
    void       ClearActiveDoorForRemoval();
    void       RemoveActiveDoorIfAnyAwaiting();

    void       DecrementTimer                   (DoorTile* const doorTile)     const;
    void       ResetTimer                       (DoorTile* const doorTile)     const;
};