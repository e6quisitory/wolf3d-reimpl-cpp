#pragma once

#include <utility>
#include "../GameData.h"

class DoorManager {
private:
    GameData*                   gameData;
    double                      movementIncrement;  // units of door movement per frame
    double                      timerIncrement;     // timer units per frame
    std::pair<bool, DoorTile*>  activeDoorToErase;  // Only one door can be opened at once; thus only one door can have closed at any given time

public:
    void       Init                             (GameData* _gameData);
    void       Update();

private:
    void       MoveDoor                         (DoorTile* const doorTile);

    bool       InsideDoor                       (DoorTile* const doorTile)     const;
    bool       InsideAnyDoor()                                                 const;

    void       DecrementTimer                   (DoorTile* const doorTile)     const;
    void       ResetTimer                       (DoorTile* const doorTile)     const;

    bool       ActiveDoorAwaitingRemoval()                                     const;
    DoorTile*  GetActiveDoorAwaitingRemove()                                   const;
    void       SetActiveDoorForRemoval          (DoorTile* const _door);
    void       ClearActiveDoorForRemoval();
    void       RemoveActiveDoorIfAnyAwaiting();
};