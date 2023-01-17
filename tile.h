/*
 * tile.h:
 *
 * Tile objects constructed using OOP concepts (inheritance, polymorphism).
 *
 * Goal is that when the raycaster "hits" a tile, the tile *itself* spits out:
 *
 *     (i)  whether it is empty of not
 *     (ii) if not empty, then the texture + portion of it that the ray hits
 *
 *  In addition to a ray-tile intersection, we can also calculate player-tile intersections, so that player doesn't pass through non-empty blocks.
 *
 */

#pragma once

#include <SDL2/SDL.h>
#include "vec2.h"
#include "ray.h"
#include "dda.h"
#include "global.h"

/*
================================
 Types of tiles
================================
*/

enum TILE_TYPE {
    EMPTY,
    WALL,
    DOOR
};

/*
=======================================================
 Struct that ray-tile intersection function spits out
=======================================================
*/

struct ray_tile_hit_info {
    ray_tile_hit_info(bool h): hit(h) {}
    ray_tile_hit_info(SDL_Texture* t, SDL_Rect r, double _distance): hit(true), texture(t), texture_rect(r), distance(_distance) {}

    bool hit;                // Indicates if tile hit is non-empty
    SDL_Texture* texture;    // If non-empty, then the texture of the tile hit
    SDL_Rect texture_rect;   // The portion of the texture that the ray intersected with
    double distance;         // Distance of intersection from player location
};

/*
================================
 Tile base class (abstract)
================================
*/

class tile {
public:
    // Virtual destructor
    virtual ~tile() {}

    // Spits out tile's type
    virtual TILE_TYPE type() const = 0;

    // Ray-tile intersection function
    virtual ray_tile_hit_info ray_hit(const intersection& curr_inter, const TILE_TYPE& prev_tile_type) const = 0;

    // Player-tile intersection function
    virtual bool player_hit() const = 0;

protected:
    enum WALL_TYPE {
        HORIZONTAL,
        VERTICAL,
        CORNER
    };

    // Given a point on a line on the map grid, calculates the wall type (horizontal or vertical) + the width percent
    struct wall_hit_info {
        wall_hit_info(point2 pt) {
            bool x_is_int = is_integer(pt.x());
            bool y_is_int = is_integer(pt.y());
            bool x_is_middle = (pt.x() - static_cast<int>(pt.x())) == 0.5;
            bool y_is_middle = (pt.y() - static_cast<int>(pt.y())) == 0.5;

            if ((x_is_int || x_is_middle) && !y_is_int) {
                WallType = VERTICAL;
                width_percent = pt.y() - int(pt.y());
            } else if ((y_is_int || y_is_middle) && !x_is_int) {
                WallType = HORIZONTAL;
                width_percent = pt.x() - int(pt.x());
            } else {
                WallType = CORNER;
                width_percent = 0;
            }
        }

        WALL_TYPE WallType;
        double width_percent;  // Say you hit point (1.45, 2); then the width_percent would be 0.45; if (3, 7.39), width_percent = 0.39
    };

    using gate_hit_info = wall_hit_info;

    // Given a texture pair and a wall description, returns the lit or unlit version of the texture.
    // If vertical wall is hit, we use lit texture, if horizontal wall, then unlit texture.
    SDL_Texture* do_texture_lighting(const texture_pair& _texture_pair, const wall_hit_info& _wall_hit_info) const {
        if (_wall_hit_info.WallType == VERTICAL)
            return _texture_pair.first;
        else
            return _texture_pair.second;
    }
};

/*
================================
 Empty tile
================================
*/

class empty : public tile {
public:
    empty() {}

    virtual TILE_TYPE type() const override {
        return EMPTY;
    }

    virtual ray_tile_hit_info ray_hit(const intersection& curr_inter, const TILE_TYPE& prev_tile_type) const override {
        return ray_tile_hit_info(false);
    }

    virtual bool player_hit() const override {
        return false;
    }
};

/*
================================
 Basic textured wall tile
================================
*/

class wall : public tile {
public:
    wall(texture_pair _texture, texture_pair _sidewall_texture): texture(_texture), gate_sidewall_texture(_sidewall_texture) {}

    virtual TILE_TYPE type() const override {
        return WALL;
    }

    virtual ray_tile_hit_info ray_hit(const intersection& curr_inter, const TILE_TYPE& prev_tile_type) const override {
        wall_hit_info _wall_hit_info(curr_inter.Point);
        SDL_Rect rect = {static_cast<int>(_wall_hit_info.width_percent * TEXTURE_PITCH), 0, 1, TEXTURE_PITCH};  // One vertical line of pixels from texture
        double distance = curr_inter.dist_to_inter();

        if (prev_tile_type == DOOR)
            return ray_tile_hit_info(do_texture_lighting(gate_sidewall_texture,_wall_hit_info), rect, distance);
        else
            return ray_tile_hit_info(do_texture_lighting(texture, _wall_hit_info), rect, distance);

    }

    virtual bool player_hit() const override {
        return true;
    }

private:
    texture_pair texture;
    // To render door sidewall, we see if the type of the *previous* tile that was hit was a door.
    // If so, then simply swap this wall's texture with the door sidewall texture.
    texture_pair gate_sidewall_texture;
};

/*
================================
 Door tile
================================
*/

enum DOOR_STATUS {
    CLOSED,
    OPENING,
    OPEN,
    CLOSING
};

enum DOOR_POSITION {
    OPEN_POSITION = 0,
    CLOSED_POSITION = 1
};

enum TIMER_VALUE {
    NO_TIME_LEFT = 0,
    FULL_TIME_LEFT = 1
};

class door : public tile {
public:
    door(texture_pair _gate_texture, texture_pair _sidewall_texture): gate_texture(_gate_texture), gate_sidewall_texture(_sidewall_texture) {
        // Gate initial status is closed, with timer reset to full-time left (to be decremented when door fully opens)
        status = CLOSED;
        position = CLOSED_POSITION;
        timer = FULL_TIME_LEFT;
    }

    virtual TILE_TYPE type() const override {
        return DOOR;
    }

    virtual ray_tile_hit_info ray_hit(const intersection& curr_inter, const TILE_TYPE& prev_tile_type) const override {

        // Get wall description for point on edge of tile
        wall_hit_info _wall_hit_info(curr_inter.Point);

        // Center this point
        point2 centered_pt = center_point(curr_inter, _wall_hit_info);

        // First check if incoming ray actually intersects with middle of tile (the gate)
        if (ipoint2(centered_pt) == curr_inter.iPoint) {

            // Get width percent of middle point in the tile
            gate_hit_info _gate_hit_info = gate_hit_info(centered_pt);

            // Ray does intersect gate, but now check if the gate *blocks* the ray
            if (_gate_hit_info.width_percent < position) {

                // If ray is blocked by gate, then output the proper gate texture and rect
                double gate_texture_width_percent = position - _gate_hit_info.width_percent;
                SDL_Rect gate_texture_rect = {static_cast<int>(gate_texture_width_percent * TEXTURE_PITCH), 0, 1, TEXTURE_PITCH};
                double center_pt_dist = curr_inter.Ray.dist_to_pt(centered_pt);

                return ray_tile_hit_info(do_texture_lighting(gate_texture, _gate_hit_info), gate_texture_rect, center_pt_dist);

            } else {
                // If gate doesn't block the ray, let ray pass through
                return ray_tile_hit_info(false);
            }
        } else
            // If ray doesn't even intersect the middle of the tile, then let ray pass through
            return ray_tile_hit_info(false);

    }

    virtual bool player_hit() const override {
        return position <= 0.2 ? false : true;  // Allow player to pass through door if door is at least 80% open
    }

private:
    // Takes intersection on the edge of a tile/block and centers it
    point2 center_point(const intersection& curr_inter, const wall_hit_info& curr_wall_desc) const {
        point2 centered_pt = curr_inter.Point;

        if (curr_wall_desc.WallType == VERTICAL) {
            centered_pt.x() += static_cast<double>(curr_inter.Ray.x_dir) / 2;
            centered_pt.y() += static_cast<double>(curr_inter.Ray.y_dir)*(curr_inter.Ray.y_step/2);
        } else {
            centered_pt.y() += static_cast<double>(curr_inter.Ray.y_dir) / 2;
            centered_pt.x() += static_cast<double>(curr_inter.Ray.x_dir)*(curr_inter.Ray.x_step/2);
        }
        return centered_pt;
    }

public:
    DOOR_STATUS status;
    double position;
    double timer;

private:
    texture_pair gate_texture;
    texture_pair gate_sidewall_texture;
};
