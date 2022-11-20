#pragma once

#include <SDL2/SDL.h>

#include "map.h"
#include "player.h"

class updater {
public:
    updater(map* const wm, player* const p, double* const fps):
        world_map(wm), plyr(p), last_fps(fps), last_key_space(false) {}

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
        }

        double swvl_speed = 1.9089/(*last_fps);
        if (keyboard_state[SDL_SCANCODE_RIGHT]) {
            *render_flag = plyr->swivel(-swvl_speed);
        } else if (keyboard_state[SDL_SCANCODE_LEFT]) {
            *render_flag = plyr->swivel(swvl_speed);
        }

        if (keyboard_state[SDL_SCANCODE_SPACE] || (keyboard_state[SDL_SCANCODE_SPACE] && key_down()))
            *render_flag = plyr->open_door();

        if (world_map->doors_opening()) {
            int counter = 0;
            for (const int door_index : world_map->doors_currently_opening) {
                if (world_map->doors_amount_open[door_index] < 100)
                    world_map->doors_amount_open[door_index] += 2;   // this is not final solution; speed must be fps independant
                else {
                    world_map->doors_currently_open.push_back(door_index);
                    world_map->doors_timers[door_index] = 100;
                    world_map->doors_currently_opening.erase(world_map->doors_currently_opening.begin() + counter);
                    //world_map->set_tile(door_index, 0);
                }

                ++counter;
            }
        }

        if (world_map->any_doors_open()) {
            int counter = 0;
            for (const int door_index : world_map->doors_currently_open) {

                int& door_timer = world_map->doors_timers[door_index];
                int& door_amount_open = world_map->doors_amount_open[door_index];

                if (!world_map->inside_door) {
                    if (door_timer > 0) {  // decrement timer if time is not up yet
                        door_timer -= 1;
                    } else if (door_timer == 0 && door_amount_open > 0) {  // if time is up, start moving door to closed again
                        door_amount_open -= 2;
                    } else {
                        door_timer = -1;
                        world_map->doors_currently_open.erase(world_map->doors_currently_open.begin() + counter);
                    }
                }

                ++counter;
            }
        }
    }

private:
    map* world_map;
    player* plyr;
    double* last_fps;
    const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);
    bool last_key_space;
};
