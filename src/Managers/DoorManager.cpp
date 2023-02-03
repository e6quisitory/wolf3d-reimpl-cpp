#include "DoorManager.h"

/*
================================
Public Methods
================================
*/

void DoorManager::Init(GameData* _gameData) {
    gameData = _gameData;

    // No doors active at beginning
    gameData->Map.anyDoorsAwaitingRendering = false;

    // Set door movement and timer speeds based on fps / display refresh rate
    movementIncrement = gameData->Multimedia.refreshRate / 3000.0;
    timerIncrement    = gameData->Multimedia.refreshRate / 6000.0;

    // Set initial value for to_erase
    ClearActiveDoorForRemoval();
}

void DoorManager::Update() {
    // Check if doors even need updating; if not, then player is idle, so better not waste CPU for nothing
    bool noActiveDoors = gameData->Map.activeDoors.empty();
    auto oneActiveDoor = gameData->Map.GetLoneActiveDoor();

    if (noActiveDoors)
        gameData->Map.anyDoorsAwaitingRendering = false;
    else if (oneActiveDoor.has_value() && InsideAnyDoor()) {
        ResetTimer(oneActiveDoor.value());
        gameData->Map.anyDoorsAwaitingRendering = false;
    } else {
        // Cycle through list (std::map) of active doors
        for (auto const& [activeDoor, _activeDoor] : gameData->Map.activeDoors) {
            switch (activeDoor->doorStatus) {
                case doorStatus_t::OPEN:          // If door is open, update (decrement) timer, but only if player is not inside that door
                    if (!InsideDoor(activeDoor)) {
                        DecrementTimer(activeDoor);
                    } else
                        ResetTimer(activeDoor);
                    break;
                case doorStatus_t::OPENING:
                case doorStatus_t::CLOSING:        // If door is opening or closing, then move the door accordingly
                    MoveDoor(activeDoor);
                    break;
            }
        }

        // If any door is done closing, it must be erased from gameData->Map.activeDoors;
        RemoveActiveDoorIfAnyAwaiting();

        gameData->Map.anyDoorsAwaitingRendering = true;
    }
}

/*
================================
    Private Methods
================================
*/

void DoorManager::MoveDoor(DoorTile* const doorTile) {
    double proposedPosition;

    switch (doorTile->doorStatus) {
        case doorStatus_t::CLOSING: proposedPosition = doorTile->doorPosition + movementIncrement; break;
        case doorStatus_t::OPENING: proposedPosition = doorTile->doorPosition - movementIncrement; break;
    }

    bool fullyOpen  =  proposedPosition <= static_cast<double>(doorPosition_t::OPEN);
    bool fullyClose =  proposedPosition >= static_cast<double>(doorPosition_t::CLOSED);

    if (fullyOpen) {
        doorTile->doorPosition = static_cast<double>(doorPosition_t::OPEN);
        doorTile->doorStatus   = doorStatus_t::OPEN;

    } else if (fullyClose) {
        doorTile->doorPosition = static_cast<double>(doorPosition_t::CLOSED);
        doorTile->doorStatus   = doorStatus_t::CLOSED;
        SetActiveDoorForRemoval(doorTile);

    } else
        doorTile->doorPosition = proposedPosition;
}

void DoorManager::DecrementTimer(DoorTile* const doorTile) const {
    double proposedTimerVal = doorTile->doorTimerVal - timerIncrement;

    bool timeIsUp = proposedTimerVal <= static_cast<double>(doorTimerVal_t::NO_TIME_LEFT);

    if (timeIsUp) {
        doorTile->doorStatus = doorStatus_t::CLOSING;
        ResetTimer(doorTile);
    } else
        doorTile->doorTimerVal = proposedTimerVal;
}

void DoorManager::ResetTimer(DoorTile* const doorTile) const {
    doorTile->doorTimerVal = static_cast<double>(doorTimerVal_t::FULL_TIME_LEFT);
}

bool DoorManager::InsideDoor(DoorTile* const doorTile) const {
    return gameData->Map.GetTile(gameData->Player.location) == doorTile ? true : false;
}

bool DoorManager::InsideAnyDoor() const {
    return gameData->Map.GetTile(gameData->Player.location)->type == TILE_TYPE_DOOR;
}

bool DoorManager::ActiveDoorAwaitingRemoval() const {
    return activeDoorToErase.first;
}

DoorTile* DoorManager::GetActiveDoorAwaitingRemove() const {
    return activeDoorToErase.second;
}

void DoorManager::SetActiveDoorForRemoval(DoorTile* const _door) {
    activeDoorToErase = {true, _door};
}

void DoorManager::ClearActiveDoorForRemoval() {
    activeDoorToErase.first = false;
}

void DoorManager::RemoveActiveDoorIfAnyAwaiting() {
    if (ActiveDoorAwaitingRemoval()) {
        gameData->Map.RemoveActiveDoor(activeDoorToErase.second);
        ClearActiveDoorForRemoval();
    }
}