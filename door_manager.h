/*
 * door_manager.h:
 *
 * Manages the doors in the map. Opens them, closes them, and manages the timers for them when they are open.
 *
 */

#pragma once

#include <utility>

#include "game_data.h"

class door_manager {
public:

    void init(game_data* gm_dat) {
        GameData = gm_dat;

        // No doors active at beginning
        GameData->Map.any_active_doors = false;

        // Set door movement and timer speeds based on fps / display refresh rate
        movement_increment = GameData->Multimedia.refresh_rate / 3000.0;
        timer_increment = GameData->Multimedia.refresh_rate / 6000.0;

        // Set initial value for to_erase
        active_door_to_erase.first = false;
    }

    void update() {
        // Check if doors even need updating; if not, then player is idle, so better not waste CPU for nothing
        bool no_active_doors = GameData->Map.active_doors.empty();
        bool one_active_door = GameData->Map.active_doors.size() == 1;

        if (no_active_doors)
            GameData->Map.any_active_doors = false;
        else if (one_active_door && inside_door()) {
            reset_timer(GameData->Map.active_doors.begin()->first);
            GameData->Map.any_active_doors = false;
        } else {
            // Cycle through list (std::map) of active doors
            for (auto const& [door, _door] : GameData->Map.active_doors) {
                switch (_door->status) {
                    case OPEN:          // If door is open, update (decrement) timer, but only if player is not inside that door
                        if (!inside_door(_door)) {
                            decrement_timer(_door);
                        } else
                            reset_timer(_door);
                        break;
                    case OPENING:
                    case CLOSING:        // If door is opening or closing, then move the door accordingly
                        move_door(_door);
                        break;
                }
            }

            // If any door is done closing, it must be erased from GameData->Map.active_doors;
            erase_active_door_if_any();

            GameData->Map.any_active_doors = true;
        }
    }

private:
    // Moves door (modifies door position) based on if door is currently opening or closing.
    // If door is done opening or closing, it changes its status accordingly.
    void move_door(door* const _door) {
        double proposed_position;

        switch (_door->status) {
            case CLOSING: proposed_position = _door->position + movement_increment; break;
            case OPENING: proposed_position = _door->position - movement_increment; break;
        }

        if (proposed_position <= 0.0) {
            _door->position = 0.0;
            _door->status = OPEN;
        } else if (proposed_position >= 1.0) {
            _door->position = 1.0;
            _door->status = CLOSED;
            set_active_door_for_erasing(_door);
        } else
            _door->position = proposed_position;
    }

    // If door is currently open, updates (decrements) timer
    // If timer ticks to 0, sets door status to closing and resets timer
    void decrement_timer(door* const _door) const {
        double proposed_timerval = _door->timer - timer_increment;

        if (proposed_timerval <= 0.0) {
            _door->status = CLOSING;
            reset_timer(_door);
        } else
            _door->timer = proposed_timerval;
    }

    void reset_timer(door* const _door) const {
        _door->timer = 1.0;
    }

    // Returns true if player is currently inside door passed in as argument
    bool inside_door(door* const _door) const {
        return GameData->Map.get_tile(GameData->Player.location) == _door ? true : false;
    }

    // Returns true if player is currently inside *any* door
    bool inside_door() const {
        return GameData->Map.get_tile(GameData->Player.location)->type() == DOOR;
    }

    void set_active_door_for_erasing(door* const _door) {
        active_door_to_erase = {true, _door};
    }

    void erase_active_door_if_any() {
        if (active_door_to_erase.first == true) {
            GameData->Map.remove_active_door(active_door_to_erase.second);
            active_door_to_erase.first = false;
        }
    }

private:
    game_data* GameData;

    double movement_increment;  // units of door movement per frame
    double timer_increment;     // timer units per frame

    std::pair<bool, door*> active_door_to_erase;  // Only one door can be opened at once; thus only one door can have closed at any given time
};