#include "DoorManager.h"

/*
================================
Public Methods
================================
*/

void DoorManager::Init(GameState* const _gameState, const int _screenRefreshRate) {
    gameState = _gameState;

    // No doors active at beginning
    gameState->map.anyDoorsAwaitingRendering = false;

    // Set door movement and timer speeds based on fps / display refresh rate
    movementIncrement = _screenRefreshRate / 3000.0;
    timerIncrement    = _screenRefreshRate / 6000.0;

    // Set initial value for to_erase
    ClearActiveDoorForRemoval();
}

void DoorManager::Update() {
    // Check if doors even need updating; if not, then player is idle, so better not waste CPU for nothing
    bool noActiveDoors = gameState->map.activeDoors.empty();
    auto oneActiveDoor = gameState->map.GetLoneActiveDoor();

    if (noActiveDoors)
        gameState->map.anyDoorsAwaitingRendering = false;
    else if (oneActiveDoor.has_value() && InsideAnyDoor()) {
        ResetTimer(oneActiveDoor.value());
        gameState->map.anyDoorsAwaitingRendering = false;
    } else {
        // Cycle through list (std::map) of active doors
        for (auto const [activeDoor, _activeDoor] : gameState->map.activeDoors) {
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

        // If any door is done closing, it must be erased from gameState->Map.activeDoors;
        RemoveActiveDoorIfAnyAwaiting();

        gameState->map.anyDoorsAwaitingRendering = true;
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

bool DoorManager::InsideDoor(DoorTile* const doorTile) const {
    return gameState->map.GetTile(gameState->player.location) == doorTile ? true : false;
}

bool DoorManager::InsideAnyDoor() const {
    return gameState->map.GetTile(gameState->player.location)->tileType == tileType_t::DOOR;
}

bool DoorManager::ActiveDoorAwaitingRemoval() const {
    return activeDoorToErase.first;
}

DoorTile* DoorManager::GetActiveDoorAwaitingRemove() const {
    return activeDoorToErase.second;
}

void DoorManager::SetActiveDoorForRemoval(DoorTile* const doorTile) {
    activeDoorToErase = {true, doorTile};
}

void DoorManager::ClearActiveDoorForRemoval() {
    activeDoorToErase.first = false;
}

void DoorManager::RemoveActiveDoorIfAnyAwaiting() {
    if (ActiveDoorAwaitingRemoval()) {
        gameState->map.RemoveActiveDoor(activeDoorToErase.second);
        ClearActiveDoorForRemoval();
    }
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