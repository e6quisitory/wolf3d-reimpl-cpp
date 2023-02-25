#include "DoorManager.h"

#include "../../WorldState/Map/Tile/DoorTile/DoorTile.h"

/*
=========================================================
    Public methods
=========================================================
*/

void DoorManager::Init(WorldState* const _worldState, const int _screenRefreshRate) {
    worldState = _worldState;

    // No doors active at beginning
    worldState->map.anyDoorsAwaitingRendering = false;

    // Set door movement and timer speeds based on fps / display refresh rate
    movementIncrement = _screenRefreshRate / 3000.0;
    timerIncrement    = _screenRefreshRate / 6000.0;

    // Set initial value for to_erase
    ClearActiveDoorForRemoval();
}

void DoorManager::Update() {
    // Check if doors even need updating; if not, then player is idle, so better not waste CPU for nothing
    bool noActiveDoors = worldState->map.activeDoors.empty();
    auto oneActiveDoor = worldState->map.GetLoneActiveDoor();

    if (noActiveDoors)
        worldState->map.anyDoorsAwaitingRendering = false;
    else if (oneActiveDoor.has_value() && InsideAnyDoor()) {
        ResetTimer(oneActiveDoor.value());
        worldState->map.anyDoorsAwaitingRendering = false;
    } else {
        // Cycle through list (std::map) of active doors
        for (auto const [activeDoor, _activeDoor] : worldState->map.activeDoors) {
            switch (activeDoor->status) {
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

        // If any door is done closing, it must be erased from worldState->Map.activeDoors;
        RemoveActiveDoorIfAnyAwaiting();

        worldState->map.anyDoorsAwaitingRendering = true;
    }
}

/*
=========================================================
    Private methods
=========================================================
*/

void DoorManager::MoveDoor(Door *const door) {
    double proposedPosition;

    switch (door->status) {
        case doorStatus_t::CLOSING: proposedPosition = door->position + movementIncrement; break;
        case doorStatus_t::OPENING: proposedPosition = door->position - movementIncrement; break;
    }

    bool fullyOpen  = proposedPosition <= static_cast<double>(doorPosition_t::OPEN);
    bool fullyClose = proposedPosition >= static_cast<double>(doorPosition_t::CLOSED);

    if (fullyOpen) {
        door->position = static_cast<double>(doorPosition_t::OPEN);
        door->status   = doorStatus_t::OPEN;

    } else if (fullyClose) {
        door->position = static_cast<double>(doorPosition_t::CLOSED);
        door->status   = doorStatus_t::CLOSED;
        SetActiveDoorForRemoval(door);

    } else
        door->position = proposedPosition;
}

bool DoorManager::InsideDoor(Door *const door) const {
    auto currTile = worldState->map.GetTile(worldState->player.location);
    if (currTile->type == tileType_t::DOOR) {
        auto currDoorTile = static_cast<DoorTile*>(currTile);
        return currDoorTile->door == door;
    } else
        return false;
}

bool DoorManager::InsideAnyDoor() const {
    return worldState->map.GetTile(worldState->player.location)->type == tileType_t::DOOR;
}

bool DoorManager::ActiveDoorAwaitingRemoval() const {
    return activeDoorToErase.first;
}

Door * DoorManager::GetActiveDoorAwaitingRemove() const {
    return activeDoorToErase.second;
}

void DoorManager::SetActiveDoorForRemoval(Door *const door) {
    activeDoorToErase = {true, door};
}

void DoorManager::ClearActiveDoorForRemoval() {
    activeDoorToErase.first = false;
}

void DoorManager::RemoveActiveDoorIfAnyAwaiting() {
    if (ActiveDoorAwaitingRemoval()) {
        worldState->map.RemoveActiveDoor(activeDoorToErase.second);
        ClearActiveDoorForRemoval();
    }
}

void DoorManager::DecrementTimer(Door *const door) const {
    double proposedTimerVal = door->timerVal - timerIncrement;

    bool timeIsUp = proposedTimerVal <= static_cast<double>(doorTimerVal_t::NO_TIME_LEFT);

    if (timeIsUp) {
        door->status = doorStatus_t::CLOSING;
        ResetTimer(door);
    } else
        door->timerVal = proposedTimerVal;
}

void DoorManager::ResetTimer(Door *const door) const {
    door->timerVal = static_cast<double>(doorTimerVal_t::FULL_TIME_LEFT);
}