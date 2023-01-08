#pragma once

#include <utility>

#include "game_data.h"

class door_manager {
public:

    void init(game_data* gm_dat) {
        GameData = gm_dat;

        // Set door movement and timer speeds based on fps / display refresh rate
        movement_increment = GameData->Multimedia.refresh_rate / 3000.0;
        timer_increment = GameData->Multimedia.refresh_rate / 6000.0;

        // Set initial value for to_erase
        to_erase.first = false;
    }

    bool update() {
        bool no_active_doors = GameData->Map.active_doors.empty();
        bool one_active_door = GameData->Map.active_doors.size() == 1;

        if (no_active_doors || (one_active_door && inside_door())) return false;

        for (auto const& [__door, _door] : GameData->Map.active_doors) {
            switch (_door->status) {
                case OPEN:
                    if (!inside_door(_door)) {
                        update_timer(_door);
                    }
                    break;
                case OPENING:
                case CLOSING: move_door(_door); break;
            }
        }

        // If any doors are done closing, they must be erased from GameData->Map.active_doors;
        if (to_erase.first == true) {
            GameData->Map.active_doors.erase(to_erase.second);
            to_erase.first = false;
        }

        return true;
    }

private:
    void move_door(door* const _door) {
        double proposed_position = _door->position;

        switch (_door->status) {
            case CLOSING: proposed_position += movement_increment; break;
            case OPENING: proposed_position -= movement_increment; break;
        }

        if (proposed_position <= 0.0) {
            _door->position = 0.0;
            _door->status = OPEN;
        } else if (proposed_position >= 1.0) {
            _door->position = 1.0;
            _door->status = CLOSED;
            to_erase = {true, _door};
        } else
            _door->position = proposed_position;
    }

    void update_timer(door* const _door) const {
        double proposed_timestamp = _door->timer - timer_increment;

        if (proposed_timestamp <= 0.0) {
            _door->status = CLOSING;
            _door->timer = 1.0;
        } else
            _door->timer = proposed_timestamp;
    }

    bool inside_door(door* const _door) {
        return GameData->Map(GameData->Player.location) == _door ? true : false;
    }

    bool inside_door() {
        return GameData->Map(GameData->Player.location)->type() == DOOR;
    }

private:
    game_data* GameData;

    double movement_increment;  // units of door movement per frame
    double timer_increment;     // timer units per frame

    std::pair<bool, door*> to_erase;  // Only one door can be opened at once; thus only one door can have closed at any given time
};