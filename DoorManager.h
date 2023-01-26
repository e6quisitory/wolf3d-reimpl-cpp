/*
 * door_manager.h:
 *
 * Manages the doors in the map. Opens them, closes them, and manages the timers for them when they are open.
 *
 */

#pragma once

#include <utility>

#include "GameData.h"

class DoorManager {
public:

    void Init(GameData* _game_data) {
        gameData = _game_data;

        // No doors active at beginning
        gameData->Map.anyDoorsAwaitingRendering = false;

        // Set door movement and timer speeds based on fps / display refresh rate
        movementIncrement = gameData->Multimedia.refresh_rate / 3000.0;
        timerIncrement = gameData->Multimedia.refresh_rate / 6000.0;

        // Set initial value for to_erase
        activeDoorToErase.first = false;
    }

    void Update() {
        // Check if doors even need updating; if not, then player is idle, so better not waste CPU for nothing
        bool no_active_doors = gameData->Map.active_doors.empty();
        bool one_active_door = gameData->Map.active_doors.size() == 1;

        if (no_active_doors)
            gameData->Map.anyDoorsAwaitingRendering = false;
        else if (one_active_door && inside_door()) {
            reset_timer(gameData->Map.active_doors.begin()->first);
            gameData->Map.anyDoorsAwaitingRendering = false;
        } else {
            // Cycle through list (std::map) of active doors
            for (auto const& [door, _door] : gameData->Map.active_doors) {
                switch (_door->status) {
                    case DOOR_STATUS_OPEN:          // If door is open, update (decrement) timer, but only if player is not inside that door
                        if (!inside_door(_door)) {
                            decrement_timer(_door);
                        } else
                            reset_timer(_door);
                        break;
                    case DOOR_STATUS_OPENING:
                    case DOOR_STATUS_CLOSING:        // If door is opening or closing, then move the door accordingly
                        move_door(_door);
                        break;
                }
            }

            // If any door is done closing, it must be erased from GameData->Map.active_doors;
            erase_active_door_if_any();

            gameData->Map.anyDoorsAwaitingRendering = true;
        }
    }

private:
    // Moves door (modifies door position) based on if door is currently opening or closing.
    // If door is done opening or closing, it changes its status accordingly.
    void move_door(DoorTile* const _door) {
        double proposed_position;

        switch (_door->status) {
            case DOOR_STATUS_CLOSING: proposed_position = _door->position + movementIncrement; break;
            case DOOR_STATUS_OPENING: proposed_position = _door->position - movementIncrement; break;
        }

        if (proposed_position <= DOOR_POSITION_OPEN) {
            _door->position = DOOR_POSITION_OPEN;
            _door->status = DOOR_STATUS_OPEN;
        } else if (proposed_position >= DOOR_POSITION_CLOSED) {
            _door->position = DOOR_POSITION_CLOSED;
            _door->status = DOOR_STATUS_CLOSED;
            set_active_door_for_erasing(_door);
        } else
            _door->position = proposed_position;
    }

    // If door is currently open, updates (decrements) timer
    // If timer ticks to 0, sets door status to closing and resets timer
    void decrement_timer(DoorTile* const _door) const {
        double proposed_timer_value = _door->timer - timerIncrement;

        if (proposed_timer_value <= TIMER_VALUE_NONE) {
            _door->status = DOOR_STATUS_CLOSING;
            reset_timer(_door);
        } else
            _door->timer = proposed_timer_value;
    }

    void reset_timer(DoorTile* const _door) const {
        _door->timer = TIMER_VALUE_FULL;
    }

    // Returns true if player is currently inside door passed in as argument
    bool inside_door(DoorTile* const _door) const {
        return gameData->Map.GetTile(gameData->Player.location) == _door ? true : false;
    }

    // Returns true if player is currently inside *any* door
    bool inside_door() const {
        return gameData->Map.GetTile(gameData->Player.location)->type == TILE_TYPE_DOOR;
    }

    void set_active_door_for_erasing(DoorTile* const _door) {
        activeDoorToErase = {true, _door};
    }

    void erase_active_door_if_any() {
        if (activeDoorToErase.first == true) {
            gameData->Map.remove_active_door(activeDoorToErase.second);
            activeDoorToErase.first = false;
        }
    }

private:
    GameData* gameData;

    double movementIncrement;  // units of door movement per frame
    double timerIncrement;     // timer units per frame

    std::pair<bool, DoorTile*> activeDoorToErase;  // Only one door can be opened at once; thus only one door can have closed at any given time
};