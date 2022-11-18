#pragma once

#include <SDL2/SDL.h>

#include "map.h"
#include "player.h"

class updater {
public:
    updater(map* const wm, player* const p, double* const fps):
        world_map(wm), plyr(p), last_fps(fps) {}

    updater() {}

    bool key_down() {
        return keyboard_state[SDL_SCANCODE_W]     ||
               keyboard_state[SDL_SCANCODE_A]     ||
               keyboard_state[SDL_SCANCODE_S]     ||
               keyboard_state[SDL_SCANCODE_D]     ||
               keyboard_state[SDL_SCANCODE_RIGHT] ||
               keyboard_state[SDL_SCANCODE_LEFT];
    }
    
    void update_player(bool* const render_flag) {

        if (world_map->doors_opening()) {
            int counter = 0;
            for (const int door_index : world_map->doors_currently_opening) {
                if (world_map->doors_amount_open[door_index] < 100)
                    world_map->doors_amount_open[door_index] += 2;   // this is not final solution; speed must be fps independant
                else {
                    world_map->doors_currently_opening.erase(world_map->doors_currently_opening.begin() + counter);
                    world_map->set_tile(door_index, 0);
                }

                ++counter;
            }
        }

        double mov_speed = 3.645/(*last_fps);
        if (keyboard_state[SDL_SCANCODE_W] && keyboard_state[SDL_SCANCODE_D]) {
            *render_flag = plyr->move_y(0.7071067*mov_speed);
            *render_flag = plyr->move_x(0.7071067*mov_speed);
        } else if (keyboard_state[SDL_SCANCODE_W] && keyboard_state[SDL_SCANCODE_A]) {
            *render_flag = plyr->move_y(0.7071067*mov_speed);
            *render_flag = plyr->move_x(-0.7071067*mov_speed);
        } else if(keyboard_state[SDL_SCANCODE_W]) {
            *render_flag = plyr->move_y(mov_speed);
        } else if (keyboard_state[SDL_SCANCODE_S]) {
            *render_flag = plyr->move_y(-mov_speed);
        } else if (keyboard_state[SDL_SCANCODE_A]) {
            *render_flag = plyr->move_x(-mov_speed);
        } else if (keyboard_state[SDL_SCANCODE_D]) {
            *render_flag = plyr->move_x(mov_speed);
        } else if (keyboard_state[SDL_SCANCODE_SPACE] && !world_map->doors_opening()) {
            *render_flag = plyr->open_door();
        }

        double swvl_speed = 1.9089/(*last_fps);
        if (keyboard_state[SDL_SCANCODE_RIGHT]) {
            *render_flag = plyr->swivel(-swvl_speed);
        } else if (keyboard_state[SDL_SCANCODE_LEFT]) {
            *render_flag = plyr->swivel(swvl_speed);
        }
    }

private:
    map* world_map;
    player* plyr;
    double* last_fps;
    const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);;
};
