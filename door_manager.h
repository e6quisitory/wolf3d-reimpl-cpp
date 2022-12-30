#pragma once

#include <vector>

#include "game_data.h"

class door_manager {
public:

    void init(game_data* gm_dat) {
        GameData = gm_dat;

        // Collect tile pointers for all doors in the map
        for (int tile_index = 0; tile_index < GameData->Map.num_tiles; ++tile_index) {
            tile* curr_tile = GameData->Map[tile_index];
            if (curr_tile->type() == DOOR)
                doors.push_back(static_cast<door*>(curr_tile));
        }

        // Set door movement and timer speeds based on fps / display refresh rate
        movement_increment = GameData->Multimedia.refresh_rate / 3000.0;
        timer_increment = GameData->Multimedia.refresh_rate / 6000.0;

        GameData->Map.active_doors = 0;
    }

    bool update() {
        bool no_active_doors = GameData->Map.active_doors == 0;
        bool one_active_door = GameData->Map.active_doors == 1;

        if (no_active_doors || (one_active_door && inside_door())) return false;

        for (door* _door : doors) {
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
        return true;
    }

private:
    void move_door(door* const _door) const {
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
            --GameData->Map.active_doors;
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
    std::vector<door*> doors;

    double movement_increment;  // units of door movement per frame
    double timer_increment;     // timer units per frame
};