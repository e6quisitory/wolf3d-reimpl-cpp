#pragma once

#include "game_data.h"
#include "input_manager.h"
#include "global.h"

// Modifies player based on input commands
class player_manager {
public:
    void init(game_data* gm_dat) {
        GameData = gm_dat;
        set_movement_speeds();
    }

    void set_player(point2 location, vec2 view_dir) {
        // If spawn is set for a perfect grid corner (i.e. x and y vals are both integers), there is some weird clipping that happens when you first move
        // Certainly a bug that I will investigate. But for now, if user enters in integers, a quick fix is just to add a little decimal value to them to
        // avoid the bug
        for (int i = 0; i < 2; ++i)
            if (is_integer(location[i]))
                location[i] += 0.01;

        GameData->Player.location = location;
        GameData->Player.view_dir = view_dir;
    }

    void update() const {

        switch(GameData->Inputs.curr_commands[MOVEMENT]) {
            case MOVE_EAST: move_x(EAST, FULL); break;
            case MOVE_WEST: move_x(WEST, FULL); break;
            case MOVE_NORTH: move_y(NORTH, FULL); break;
            case MOVE_SOUTH: move_y(SOUTH, FULL); break;
            case MOVE_NORTHEAST: move_y(NORTH, HALF);
                                 move_x(EAST, HALF);
                                 break;
            case MOVE_NORTHWEST: move_y(NORTH, HALF);
                                 move_x(WEST, HALF);
                                 break;
            case MOVE_SOUTHEAST: move_y(SOUTH, HALF);
                                 move_x(EAST, HALF);
                                 break;
            case MOVE_SOUTHWEST: move_y(SOUTH, HALF);
                                 move_x(WEST, HALF);
                                 break;
        }

        switch(GameData->Inputs.curr_commands[LOOKING]) {
            case LOOK_RIGHT: swivel(CLOCKWISE); break;
            case LOOK_LEFT: swivel(COUNTER_CLOCKWISE); break;
        }

        if (GameData->Inputs.curr_commands[DOORS] == OPEN_DOOR)
            open_door();
    }

private:
    void set_movement_speeds() {
        // Set speeds based on display refresh rate
        movement_coeff = 3.645/GameData->Multimedia.refresh_rate;
        swivel_amount = 1.9089/GameData->Multimedia.refresh_rate;
    }

    enum SPEED {
        FULL,
        HALF
    };

    double speed_coefficient(SPEED _speed) const {
        switch (_speed) {
            case FULL: return movement_coeff;
            case HALF: return 0.70711*movement_coeff;
        }
    }

    void move_x(X_DIR _x_dir, SPEED _speed) const {
        vec2 mov_vec = _x_dir * speed_coefficient(_speed) * GameData->Player.east;
        point2 proposed_loc = GameData->Player.location + mov_vec;

        move_if_valid(proposed_loc);
    }

    void move_y(Y_DIR _y_dir, SPEED _speed) const {
        vec2 mov_vec = _y_dir * speed_coefficient(_speed) * GameData->Player.view_dir;
        point2 proposed_loc = GameData->Player.location + mov_vec;

        move_if_valid(proposed_loc);
    }

    void swivel(SWIVEL_DIR _swivel_dir) const {
        GameData->Player.view_dir = GameData->Player.view_dir.rotate(swivel_amount*_swivel_dir);
        GameData->Player.east = GameData->Player.view_dir.rotate(-PI/2);
    }

    void move_if_valid(const point2& proposed_loc) const {
        tile* proposed_tile = GameData->Map.get_tile(proposed_loc);  // not readable, change to something like "get_tile()"
        if (!proposed_tile->hit())
            GameData->Player.location = proposed_loc;
    }

    ray view_dir_ray() const {
        return ray(GameData->Player.location, GameData->Player.view_dir);
    }

    void open_door() const {
        ray view_dir_r = view_dir_ray();
        intersection curr_inter(view_dir_r, view_dir_r.origin);

        while (GameData->Map.within_map<ipoint2>(curr_inter.iPoint)) {
            curr_inter = next_intersection(curr_inter);
            if (curr_inter.dist() > 4.0)
                break;
            else {
                tile* curr_tile = GameData->Map.get_tile(curr_inter.iPoint);
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
    game_data* GameData;

    double movement_coeff;
    double swivel_amount;
};