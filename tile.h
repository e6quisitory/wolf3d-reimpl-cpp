#pragma once

#include <SDL2/SDL.h>
#include "vec2.h"
#include "ray.h"
#include "dda.h"
#include "global.h"

enum TILE_TYPE {
    EMPTY,
    WALL,
    DOOR
};

struct texture_hit_info {
    texture_hit_info(bool h): hit(h) {}
    texture_hit_info(SDL_Texture* t, SDL_Rect r, double _distance): hit(true), texture(t), rect(r), distance(_distance) {}

    bool hit;
    SDL_Texture* texture;
    SDL_Rect rect;
    double distance;
};

class tile {
public:
    virtual ~tile() {}   // Virtual destructor

    virtual TILE_TYPE type() const = 0;
    virtual texture_hit_info ray_hit(const intersection& curr_inter, const TILE_TYPE& prev_tile_type) const = 0;  // Ray-tile intersection
    virtual bool player_hit() const = 0;  // Player-tile intersection

protected:
    enum WALL_TYPE {
        HORIZONTAL,
        VERTICAL,
        CORNER
    };

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
        double width_percent;
    };

    SDL_Texture* do_texture_lighting(const texture_pair& _texture_pair, const wall_hit_info& _wall_desc) const {
        if (_wall_desc.WallType == VERTICAL)
            return _texture_pair.first;
        else
            return _texture_pair.second;
    }
};

class empty : public tile {
public:
    empty() {}

    virtual TILE_TYPE type() const override {
        return EMPTY;
    }

    virtual texture_hit_info ray_hit(const intersection& curr_inter, const TILE_TYPE& prev_tile_type) const override {
        return texture_hit_info(false);
    }

    virtual bool player_hit() const override {
        return false;
    }
};

class wall : public tile {
public:
    wall(texture_pair _texture, texture_pair _sidewall_texture): texture(_texture), sidewall_texture(_sidewall_texture) {}

    virtual TILE_TYPE type() const override {
        return WALL;
    }

    virtual texture_hit_info ray_hit(const intersection& curr_inter, const TILE_TYPE& prev_tile_type) const override {
        wall_hit_info WallDescription(curr_inter.Point);
        SDL_Rect rect = {static_cast<int>(WallDescription.width_percent * TEXTURE_PITCH), 0, 1, TEXTURE_PITCH};  // One vertical line of pixels
        double distance = curr_inter.dist();

        return texture_hit_info(do_texture_lighting(prev_tile_type == DOOR ? sidewall_texture : texture, WallDescription), rect, distance);
    }

    virtual bool player_hit() const override {
        return true;
    }

private:
    texture_pair texture;
    texture_pair sidewall_texture;
};

enum DOOR_STATUS {
    CLOSED,
    OPENING,
    OPEN,
    CLOSING
};

class door : public tile {
public:
    door(texture_pair _gate_texture, texture_pair _sidewall_texture): gate_texture(_gate_texture), sidewall_texture(_sidewall_texture) {
        status = CLOSED;
        position = 1.0;
        timer = 1.0;
    }

    virtual TILE_TYPE type() const override {
        return DOOR;
    }

    virtual texture_hit_info ray_hit(const intersection& curr_inter, const TILE_TYPE& prev_tile_type) const override {
        wall_hit_info curr_wall_desc(curr_inter.Point);

        point2 centered_pt = center_point(curr_inter, curr_wall_desc);
        bool ray_intersects_gate = ipoint2(centered_pt) == curr_inter.iPoint;

        if (ray_intersects_gate) {
            wall_hit_info middle_hit_desc(centered_pt);
            if (ray_blocked_by_gate(middle_hit_desc)) {
                double gate_width_percent = 1 - (position - middle_hit_desc.width_percent);
                SDL_Rect gate_slice_rect = {static_cast<int>(gate_width_percent * TEXTURE_PITCH), 0, 1, TEXTURE_PITCH};
                double center_pt_dist = curr_inter.Ray.dist_to_pt(centered_pt);
                return texture_hit_info(do_texture_lighting(gate_texture, middle_hit_desc), gate_slice_rect,
                                        center_pt_dist);
            } else
                return texture_hit_info(false);
        } else
            return texture_hit_info(false);
    }

    virtual bool player_hit() const override {
        return position <= 0.2 ? false : true;
    }


private:
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

    bool ray_blocked_by_gate(const wall_hit_info& middle_wall_desc) const {
        return middle_wall_desc.width_percent < position;
    }

public:
    DOOR_STATUS status;
    double position;  // 1 is fully closed, 0 is fully open
    double timer;     // 1 is full-time left, 0 is no time left

private:
    texture_pair gate_texture;
    texture_pair sidewall_texture;
};
