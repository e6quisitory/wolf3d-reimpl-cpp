/*
 * player_manager.h:
 *
 * Reads input commands pertaining to the player (movement + looking around) and modifies player attributes accordingly.
 * Also reads door opening command and starts the process of opening whichever door player is facing (and close to)
 *
 */

#pragma once

#include "game_data.h"
#include "inputs.h"
#include "global.h"

class player_manager {
public:
    void init(inputs* _inputs, game_data* _game_data) {
        Inputs = _inputs;
        GameData = _game_data;

        // Set movement and swivel speeds based on display refresh rate (assumed that fps = refresh rate)
        movement_coeff = 3.645/GameData->Multimedia.refresh_rate;
        swivel_amount = 0.075/GameData->Multimedia.refresh_rate;
    }

    // Set spawn location and view direction of player
    void set_player(point2 location, vec2 view_dir) {
        // If spawn is set for a perfect grid corner (i.e. x and y vals are both integers), there is some weird clipping that happens when you first move
        // Certainly a bug that I will investigate. But for now, if user enters in integers, a quick fix is just to add a little decimal value to them to
        // avoid the bug
        for (int i = 0; i < 2; ++i)
            if (is_integer(location[i]))
                location[i] += 0.01;

        GameData->Player.location = location;
        GameData->Player.view_dir = unit_vector(view_dir);
    }

    // Scans current input commands for movement, looking around, and door opening.
    // Then, changes player attributes (and/or opens door) as necessary.
    void update() const {
        switch(Inputs->curr_commands[LOOKING]) {
            case LOOK_RIGHT: swivel(CLOCKWISE); break;
            case LOOK_LEFT: swivel(COUNTER_CLOCKWISE); break;
        }

        switch(Inputs->curr_commands[MOVEMENT]) {
            case MOVE_EAST:
                move_horizontal(EAST, FULL); break;
            case MOVE_WEST:
                move_horizontal(WEST, FULL); break;
            case MOVE_NORTH:
                move_vertical(NORTH, FULL); break;
            case MOVE_SOUTH:
                move_vertical(SOUTH, FULL); break;
            case MOVE_NORTHEAST:
                move_vertical(NORTH, HALF);
                move_horizontal(EAST, HALF);
                break;
            case MOVE_NORTHWEST:
                move_vertical(NORTH, HALF);
                move_horizontal(WEST, HALF);
                break;
            case MOVE_SOUTHEAST:
                move_vertical(SOUTH, HALF);
                move_horizontal(EAST, HALF);
                break;
            case MOVE_SOUTHWEST:
                move_vertical(SOUTH, HALF);
                move_horizontal(WEST, HALF);
                break;
        }

        if (Inputs->curr_commands[DOORS] == OPEN_DOOR)
            open_door();
    }

private:
    enum SPEED {
        FULL,
        HALF
    };

    double speed_coefficient(SPEED _speed) const {
        switch (_speed) {
            case FULL: return movement_coeff;
            case HALF: return 0.70711*movement_coeff;  // Reduce speed in each direction if going diagonally (45-45 triangle)
        }
    }

    void move_horizontal(HORIZONTAL_DIR _h_dir, SPEED _speed) const {
        vec2 mov_vec = _h_dir * speed_coefficient(_speed) * GameData->Player.east;
        point2 proposed_loc = GameData->Player.location + mov_vec;

        move_if_valid(proposed_loc);
    }

    void move_vertical(VERTICAL_DIR _v_dir, SPEED _speed) const {
        vec2 mov_vec = _v_dir * speed_coefficient(_speed) * GameData->Player.view_dir;
        point2 proposed_loc = GameData->Player.location + mov_vec;

        move_if_valid(proposed_loc);
    }

    void swivel(SWIVEL_DIR _swivel_dir) const {
        GameData->Player.view_dir = GameData->Player.view_dir.rotate(swivel_amount*Inputs->mouse_abs_xrel*_swivel_dir);
        GameData->Player.east = GameData->Player.view_dir.rotate(-PI/2);
    }

    // Moves player to a proposed location (passed in) only if player will not hit a non-empty block at that location
    void move_if_valid(const point2& proposed_loc) const {
        tile* proposed_tile = GameData->Map.get_tile(proposed_loc);
        if (!proposed_tile->player_hit())
            GameData->Player.location = proposed_loc;
    }

    // Returns a ray that starts at the current player location and with the player's view_dir as it's direction
    ray view_dir_ray() const {
        return ray(GameData->Player.location, GameData->Player.view_dir);
    }

    // Checks if player is facing a door and close enough to it, and if so, begins the process of opening the door
    void open_door() const {
        ray view_dir_r = view_dir_ray();
        intersection curr_inter(view_dir_r, view_dir_r.origin);

        while (GameData->Map.within_map<ipoint2>(curr_inter.iPoint)) {
            curr_inter = next_intersection(curr_inter);
            if (curr_inter.dist_to_inter() > 4.0)
                break;
            else {
                tile* curr_tile = GameData->Map.get_tile(curr_inter.Point);
                if (curr_tile->type() == DOOR) {
                    door* curr_door = static_cast<door*>(curr_tile);
                    switch (curr_door->status) {
                        case CLOSED:  GameData->Map.add_active_door(curr_door);
                        case CLOSING: curr_door->status = OPENING;
                    }
                } else
                    continue;
            }
        }
    }

private:
    inputs* Inputs;
    game_data* GameData;

    double movement_coeff;
    double swivel_amount;
};