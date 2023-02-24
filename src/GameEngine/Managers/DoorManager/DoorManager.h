#pragma once

#include "../../State/WorldState/WorldState.h"
#include "../../State/WorldState/Map/Tile/DoorTile/Door/Door.h"

/*
=========================================================
    DoorManager
=========================================================
*/

class DoorManager {
private:
    WorldState*               worldState;
    double                    movementIncrement;  // Units of door movement per frame
    double                    timerIncrement;     // Timer units per frame
    std::pair<bool, Door*>    activeDoorToErase;  // Only one door can be opened at once; thus only one door can have closed at any given time

public:
    void    Init                              (WorldState* const _worldState, const int _screenRefreshRate);
    void    Update();

private:
    void    MoveDoor                          (Door* const door);

    bool    InsideDoor                        (Door* const door)   const;
    bool    InsideAnyDoor()                                        const;

    bool    ActiveDoorAwaitingRemoval()                            const;
    Door*   GetActiveDoorAwaitingRemove()                          const;
    void    SetActiveDoorForRemoval           (Door* const door);
    void    ClearActiveDoorForRemoval();
    void    RemoveActiveDoorIfAnyAwaiting();

    void    DecrementTimer                    (Door* const door)   const;
    void    ResetTimer                        (Door* const door)   const;
};