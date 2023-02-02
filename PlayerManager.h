/*
 * player_manager.h:
 *
 * Reads input commands pertaining to the player (movement + looking around) and modifies player attributes accordingly.
 * Also reads door opening command and starts the process of opening whichever door player is facing (and close to)
 *
 */

#pragma once

#include "GameData.h"
#include "Inputs.h"
#include "utils/Conventions.h"
#include "utils/MiscMath.h"


class PlayerManager {
public:
    void Init(Inputs* _inputs, GameData* _game_data) {
        Inputs = _inputs;
        GameData = _game_data;

        // Set movement and swivel speeds based on display refresh rate (assumed that fps = refresh rate)
        movement_coeff = 3.645/GameData->Multimedia.refresh_rate;
        swivel_amount = 0.075/GameData->Multimedia.refresh_rate;
    }

    // Set spawn location and view direction of player
    void SetPlayer(Point2 location, Vec2 view_dir) {
        // If spawn is set for a perfect grid corner (i.e. x and y vals are both integers), there is some weird clipping that happens when you first move
        // Certainly a bug that I will investigate. But for now, if user enters in integers, a quick fix is just to add a little decimal value to them to
        // avoid the bug
        for (int i = 0; i < 2; ++i)
            if (IsInteger(location[i]))
                location[i] += 0.01;

        GameData->Player.location = location;
        GameData->Player.viewDir = UnitVector(view_dir);
    }

    // Scans current input commands for movement, looking around, and door opening.
    // Then, changes player attributes (and/or opens door) as necessary.
    void Update() const {
        switch(Inputs->curr_commands[LOOKING]) {
            case LOOK_RIGHT: swivel(swivelDir_t::CLOCKWISE); break;
            case LOOK_LEFT: swivel(swivelDir_t::COUNTER_CLOCKWISE); break;
        }

        switch(Inputs->curr_commands[MOVEMENT]) {
            case MOVE_EAST:
                move_horizontal(xDir_t::EAST, speed_t::FULL);
                break;
            case MOVE_WEST:
                move_horizontal(xDir_t::WEST, speed_t::FULL);
                break;
            case MOVE_NORTH:
                move_vertical(yDir_t::NORTH, speed_t::FULL);
                break;
            case MOVE_SOUTH:
                move_vertical(yDir_t::SOUTH, speed_t::FULL);
                break;
            case MOVE_NORTHEAST:
                move_vertical(yDir_t::NORTH, speed_t::HALF);
                move_horizontal(xDir_t::EAST, speed_t::HALF);
                break;
            case MOVE_NORTHWEST:
                move_vertical(yDir_t::NORTH, speed_t::HALF);
                move_horizontal(xDir_t::WEST, speed_t::HALF);
                break;
            case MOVE_SOUTHEAST:
                move_vertical(yDir_t::SOUTH, speed_t::HALF);
                move_horizontal(xDir_t::EAST, speed_t::HALF);
                break;
            case MOVE_SOUTHWEST:
                move_vertical(yDir_t::SOUTH, speed_t::HALF);
                move_horizontal(xDir_t::WEST, speed_t::HALF);
                break;
        }

        if (Inputs->curr_commands[DOORS] == OPEN_DOOR)
            open_door();
    }

private:
    enum class speed_t {
        FULL,
        HALF
    };

    double speed_coefficient(speed_t speed) const {
        switch (speed) {
            case speed_t::FULL: return movement_coeff;
            case speed_t::HALF: return 0.70711*movement_coeff;  // Reduce speed in each direction if going diagonally (45-45 triangle)
        }
    }

    void move_horizontal(xDir_t xDir, speed_t speed) const {
        Vec2 mov_vec = static_cast<int>(xDir) * speed_coefficient(speed) * GameData->Player.east;
        Point2 proposed_loc = GameData->Player.location + mov_vec;

        move_if_valid(proposed_loc);
    }

    void move_vertical(yDir_t yDir, speed_t speed) const {
        Vec2 mov_vec = static_cast<int>(yDir) * speed_coefficient(speed) * GameData->Player.viewDir;
        Point2 proposed_loc = GameData->Player.location + mov_vec;

        move_if_valid(proposed_loc);
    }

    void swivel(swivelDir_t swivelDir) const {
        GameData->Player.viewDir = GameData->Player.viewDir.Rotate(swivel_amount * Inputs->mouse_abs_xrel * static_cast<int>(swivelDir));
        GameData->Player.east = GameData->Player.viewDir.Rotate(-PI / 2);
    }

    // Moves player to a proposed location (passed in) only if player will not hit a non-empty block at that location
    void move_if_valid(const Point2& proposed_loc) const {
        Tile* proposed_tile = GameData->Map.GetTile(proposed_loc);
        if (!proposed_tile->PlayerTileHit())
            GameData->Player.location = proposed_loc;
    }

    // Returns a ray that starts at the current player location and with the player's view_dir as it's direction
    Ray view_dir_ray() const {
        return Ray(GameData->Player.location, GameData->Player.viewDir);
    }

    // Checks if player is facing a door and close enough to it, and if so, begins the process of opening the door
    void open_door() const {
        Ray view_dir_r = view_dir_ray();
        HitInfo hitInfo(view_dir_r, view_dir_r.origin);

        while (GameData->Map.within_map(hitInfo.hitTile)) {
            hitInfo.GoToNextHit();
            if (hitInfo.GetDistToHitPoint() > 4.0)
                break;
            else {
                Tile* curr_tile = GameData->Map.GetTile(hitInfo.hitPoint);
                switch (curr_tile->type) {
                    case TILE_TYPE_EMPTY:
                    case TILE_TYPE_SPRITE:
                        continue;
                    case TILE_TYPE_DOOR:
                    {
                        DoorTile* curr_door = static_cast<DoorTile*>(curr_tile);
                        switch (curr_door->status) {
                            case DOOR_STATUS_CLOSED:  GameData->Map.add_active_door(curr_door);
                            case DOOR_STATUS_CLOSING: curr_door->status = DOOR_STATUS_OPENING;
                        }
                    }
                    case TILE_TYPE_WALL:
                        goto exit_loop;
                }
            }
        }
        exit_loop: {}
    }

private:
    Inputs* Inputs;
    GameData* GameData;

    double movement_coeff;
    double swivel_amount;
};
